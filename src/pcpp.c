#define NOBUILD_IMPLEMENTATION
#include "../nobuild.h"

#include <stdbool.h>

#include "lexer.c"
#include "macro.c"
#include "conditional.c"

bool parse_new_line(const char *input, size_t in_len, size_t *index) {
	int state = 0;
	size_t i = *index;
	while (i < in_len) {
		char current_char = input[i];
		switch (state) {
			case 0: // initial state
				if (current_char == '\r') {
					state = 1;
					i++;
					break;
				}

				(*index) = i + (current_char == '\n');
				return current_char == '\n';
			case 1: // after a '\r' character
				(*index) = i + (current_char == '\n');
				return true;
		}
	}

	(*index) = i;
	return state == 1;
}

Cstr_Array cstr_split_newline(Cstr cstr)
{
	size_t len = strlen(cstr);
	size_t substr_count = 1;
	for (size_t i = 0; i < len;) {
		if (!parse_new_line(cstr, len,  &i)) {
			++i;
			continue;
		}

		++substr_count;
	}

	// if no new lines
	if (substr_count == 1) {
		return CSTR_ARRAY_MAKE(cstr);
	}

	Cstr_Array ret = { .count = substr_count };
	ret.elems = malloc(sizeof *ret.elems * ret.count);
	if (ret.elems == NULL) {
		PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
	}

	size_t substr_start = 0;
	size_t substr_index = 0;
	for (size_t i = 0; i < len;) {
		size_t delim_end = i;
		if (!parse_new_line(cstr, len,  &delim_end)) {
			i++;
			continue;
		}

		size_t substr_len = i - substr_start;
		char *substr = malloc(sizeof *substr * (substr_len + 1));
		if (substr == NULL) {
			PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
		}

		substr[substr_len] = '\0';
		ret.elems[substr_index++] = memcpy(substr, (cstr + substr_start), sizeof *substr * substr_len);
		substr_start = delim_end;
		i = delim_end;
	}

	// Add the last substring
	size_t substr_len = len - substr_start;
	char *substr = malloc(sizeof *substr * (substr_len + 1));
	if (substr == NULL) {
		PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
	}

	substr[substr_len] = '\0';
	ret.elems[substr_index++] = memcpy(substr, (cstr + substr_start), sizeof *substr * substr_len);
	return ret;
}

typedef enum PCPP_STATE {
	PCPP_INITIAL,
	PCPP_NON_DIRECTIVE,

	PCPP_DIRECTIVE,

	PCPP_DIRECTIVE_UNDEF,
	PCPP_DIRECTIVE_UNDEF_IDENTIFIER,

	PCPP_DIRECTIVE_DEF,
	PCPP_DIRECTIVE_DEF_IDENTIFIER,
	PCPP_DIRECTIVE_DEF_IDENTIFIER_ARGS,
	PCPP_DIRECTIVE_DEF_IDENTIFIER_REPLACEMENT,

	PCPP_DIRECTIVE_IFDEF,
	PCPP_DIRECTIVE_IFDEF_IDENTIFIER,

	PCPP_DIRECTIVE_IFNDEF,
	PCPP_DIRECTIVE_IFNDEF_IDENTIFIER,

	PCPP_DIRECTIVE_ELIFDEF,
	PCPP_DIRECTIVE_ELIFDEF_IDENTIFIER,

	PCPP_DIRECTIVE_ELIFNDEF,
	PCPP_DIRECTIVE_ELIFNDEF_IDENTIFIER,

	PCPP_DIRECTIVE_ELSE,

	PCPP_DIRECTIVE_ENDIF,
} PCPP_STATE ;

int main(int argc, char **argv) {
	if (argc < 2) {
		PANIC("No argument was specified.");
	}

	Cstr filename = argv[1];
	Fd f = fd_open_for_read(filename);
#ifndef _WIN32
	struct stat st = {0};
	if (fstat(f, &st) < 0) {
        PANIC("Could not stat %s: %s", filename, nobuild__strerror(errno));
	}

	off_t filesize = st.st_size;
	if (filesize < 0) {
		PANIC("File size is less than 0");
	}

	size_t f_size = (size_t) filesize;
#else
	LARGE_INTEGER filesize;
	if (!GetFileSizeEx(f, &filesize)) {
		PANIC("Could not get file size %s: %s", filename, GetLastErrorAsString());
	}

	size_t f_size = (size_t) filesize.QuadPart;
#endif

	char *buffer = malloc(f_size + 1);
	if (buffer == NULL) {
		PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
	}

	buffer[f_size] = '\0';
	size_t read_count = fd_read(f, buffer, f_size);
	if (read_count < f_size) {
		PANIC("Could not read full file into memory");
	}

	// Replace all null bytes with whitespace excluding the terminating byte
	for (size_t i = 0; i < read_count; ++i) {
		buffer[i] = buffer[i] == '\0' ? ' ' : buffer[i];
	}

	Cstr_Array lines = cstr_split_newline(buffer);
	free(buffer);

	// Merge lines that end in '\'
	for (size_t i = 0; i < lines.count;) {
		size_t line_len = strlen(lines.elems[i]);
		// Avoid lines ending in \\<newline>
		if (!(lines.elems[i][line_len-1] == '\\' && lines.elems[i][line_len-2] != '\\')) {
			i++;
			continue;
		}

		Cstr next_line = cstr_array_remove(&lines, lines.elems[i+1]);
		size_t next_line_len = strlen(next_line);

		size_t total_line_len = next_line_len + (line_len - 1); // don't count the '\' character
		lines.elems[i] = realloc((char *) lines.elems[i], sizeof *lines.elems[i] * (total_line_len + 1));
		if (lines.elems[i] == NULL) {
			PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
		}

		memcpy((char *) lines.elems[i] + (line_len-1), next_line, next_line_len +1);
		free((char *)next_line);
	}

	macro_table *symbol_table = macro_table_make();
	scope_stack *scopes = scope_stack_make();
	for (size_t i = 0; i < lines.count; ++i) {
		YY_BUFFER_STATE line_buf = lexer__scan_string(lines.elems[i]);
		INFO("Line %zu: %s", i, lines.elems[i]);

		scope_item *curr_scope = scope_stack_peek(scopes);
		PCPP_STATE state = curr_scope->should_process ? PCPP_INITIAL : PCPP_NON_DIRECTIVE;
		while (true) {
			C_TOKENS tok = lexer_lex();
			if (tok == DONE) {
				break;
			}

			switch (state) {
				case PCPP_INITIAL:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						case HASH:
							state = PCPP_DIRECTIVE;
							break;
						default:
							state = PCPP_NON_DIRECTIVE;
							TODO_SAFE("Deal with parsing C code: '%s'.", lines.elems[i]);
					}
					break;

				/*****************************************************************************************************************/

				/* Regular C code */
				case PCPP_NON_DIRECTIVE:
					break;

				/*****************************************************************************************************************/

				/* Directives */
				case PCPP_DIRECTIVE:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						case IDENTIFIER:
							if (strcmp(lexer_text, "define") == 0) {
								state = PCPP_DIRECTIVE_DEF;
							} else if (strcmp(lexer_text, "undef") == 0) {
								state = PCPP_DIRECTIVE_UNDEF;
							} else if (strcmp(lexer_text, "if") == 0) {
								// Copy parent scopes values
								scope_item *top = scope_stack_push(scopes);
								top->conditional_is_undetermined = true;
								top->should_process = curr_scope->should_process;
								top->should_output = curr_scope->should_output;
								TODO_SAFE("Actually process expressions passed to `#if`");
							} else if (strcmp(lexer_text, "ifdef") == 0) {
								state = PCPP_DIRECTIVE_IFDEF;
							} else if (strcmp(lexer_text, "ifndef") == 0) {
								state = PCPP_DIRECTIVE_IFNDEF;
							} else if (strcmp(lexer_text, "elifdef") == 0) {
								state = PCPP_DIRECTIVE_ELIFDEF;
							} else if (strcmp(lexer_text, "elifndef") == 0) {
								state = PCPP_DIRECTIVE_ELIFNDEF;
							} else if (strcmp(lexer_text, "else") == 0) {
								state = PCPP_DIRECTIVE_ELSE;
								if (!curr_scope->conditional_is_undetermined && !curr_scope->conditional_was_resolved) {
									curr_scope->should_process = true;
									curr_scope->should_output = true;
								}
							} else if (strcmp(lexer_text, "endif") == 0) {
								scope_stack_pop(scopes);
								state = PCPP_DIRECTIVE_ENDIF;
							} else {
								TODO_SAFE("Process %s -> %d: %s", lexer_text, state, C_TOKENS_STRING[tok]);
							}
							break;
						default:
							TODO_SAFE("Deal with parse error in state '%d' -> '%s'.", state, C_TOKENS_STRING[tok]);
					}
					break;

				/*****************************************************************************************************************/

				/* UNDEFINE */
				case PCPP_DIRECTIVE_UNDEF:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						case IDENTIFIER: {
							macro_definition *def = macro_table_get_def(symbol_table, lexer_text);
							if (def == NULL) {
								def = macro_table_push(symbol_table, lexer_text);
							}
							def->status = MACRO_UNDEFINED;
							state = PCPP_DIRECTIVE_UNDEF_IDENTIFIER;
							break;
						}
						default:
							PANIC("Undefine directive must be be followed by an identifier: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;
				case PCPP_DIRECTIVE_UNDEF_IDENTIFIER:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						default:
							PANIC("Undefine directive must only be followed by a single token: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;

				/*****************************************************************************************************************/

				/* DEFINE */
				case PCPP_DIRECTIVE_DEF:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						case IDENTIFIER:
							macro_table_push(symbol_table, lexer_text)->status = MACRO_DEFINED;
							state = PCPP_DIRECTIVE_DEF_IDENTIFIER;
							break;
						default:
							PANIC("Define directive must be be followed by an identifier: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;
				case PCPP_DIRECTIVE_DEF_IDENTIFIER:
					switch (tok) {
						case COMMENT:
							break;
						case WHITESPACE:
							state = PCPP_DIRECTIVE_DEF_IDENTIFIER_REPLACEMENT;
							break;
						case L_PAREN:
							macro_table_peek(symbol_table)->takes_args = true;
							state = PCPP_DIRECTIVE_DEF_IDENTIFIER_ARGS;
							break;
						default:
							break;
					}
					break;
				case PCPP_DIRECTIVE_DEF_IDENTIFIER_ARGS:
					switch (tok) {
						case COMMA:
						case COMMENT:
						case WHITESPACE:
							TODO_SAFE("Verify that each argument is delimited only by comma.");
							break;
						case R_PAREN:
							state = PCPP_DIRECTIVE_DEF_IDENTIFIER_REPLACEMENT;
							break;
						default:
							macro_definition_push_args(macro_table_peek(symbol_table), lexer_text);
							break;
					}
					break;
				case PCPP_DIRECTIVE_DEF_IDENTIFIER_REPLACEMENT:
					macro_definition_push_replacement(macro_table_peek(symbol_table), lexer_text);
					break;

				/*****************************************************************************************************************/

				/* IFDEF */
				case PCPP_DIRECTIVE_IFDEF:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						case IDENTIFIER: {
							macro_definition *def = macro_table_get_def(symbol_table, lexer_text);
							scope_item *top = scope_stack_push(scopes);
							if (def->status == MACRO_UNDETERMINED) {
								top->conditional_is_undetermined = true;
								top->should_process = curr_scope->should_process;
								top->should_output = curr_scope->should_process;
								break;
							}

							top->conditional_was_resolved = def->status == MACRO_DEFINED;
							top->should_process = def->status == MACRO_DEFINED;
							top->should_output = def->status == MACRO_DEFINED;
							state = PCPP_DIRECTIVE_IFDEF_IDENTIFIER;
							break;
						}
						default:
							PANIC("Directive `ifdef` followed by non-identifier: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;
				case PCPP_DIRECTIVE_IFDEF_IDENTIFIER:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						default:
							PANIC("Extra token at end of `ifdef` derictives: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;

				/*****************************************************************************************************************/

				/* IFDEF */
				case PCPP_DIRECTIVE_IFNDEF:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						case IDENTIFIER: {
							macro_definition *def = macro_table_get_def(symbol_table, lexer_text);
							scope_item *top = scope_stack_push(scopes);
							if (def->status == MACRO_UNDETERMINED) {
								top->conditional_is_undetermined = true;
								top->should_process = curr_scope->should_process;
								top->should_output = curr_scope->should_process;
								break;
							}

							top->conditional_was_resolved = def->status == MACRO_UNDEFINED;
							top->should_process = def->status == MACRO_UNDEFINED;
							top->should_output = def->status == MACRO_UNDEFINED;
							state = PCPP_DIRECTIVE_IFNDEF_IDENTIFIER;
							break;
						}
						default:
							PANIC("Directive `ifdef` followed by non-identifier: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;
				case PCPP_DIRECTIVE_IFNDEF_IDENTIFIER:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						default:
							PANIC("Extra token at end of `ifdef` derictives: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;

				/*****************************************************************************************************************/

				/* ELIFDEF */
				case PCPP_DIRECTIVE_ELIFDEF:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						case IDENTIFIER:
							if (curr_scope->conditional_is_undetermined) {
								state = PCPP_DIRECTIVE_ELIFDEF_IDENTIFIER;
								break;
							}

							if (curr_scope->conditional_was_resolved) {
								curr_scope->should_process = false;
								curr_scope->should_output = false;
								state = PCPP_DIRECTIVE_ELIFDEF_IDENTIFIER;
								break;
							}

							macro_definition *def = macro_table_get_def(symbol_table, lexer_text);
							if (def->status == MACRO_UNDETERMINED) {
								PANIC("Macros used in `elifdef` must me explcitly defined/undefined: %s", lexer_text);
								break;
							}

							curr_scope->conditional_was_resolved = def->status == MACRO_DEFINED;
							curr_scope->should_process = def->status == MACRO_DEFINED;
							curr_scope->should_output = def->status == MACRO_DEFINED;
							state = PCPP_DIRECTIVE_ELIFDEF_IDENTIFIER;
							break;
						default:
							PANIC("Directive `elifdef` followed by non-identifier: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;
				case PCPP_DIRECTIVE_ELIFDEF_IDENTIFIER:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						default:
							PANIC("Extra token at end of `elifdef` derictives: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;

				/*****************************************************************************************************************/

				/* ELIFNDEF */
				case PCPP_DIRECTIVE_ELIFNDEF:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						case IDENTIFIER:
							if (curr_scope->conditional_is_undetermined) {
								state = PCPP_DIRECTIVE_ELIFNDEF_IDENTIFIER;
								break;
							}

							if (curr_scope->conditional_was_resolved) {
								curr_scope->should_process = false;
								curr_scope->should_output = false;
								state = PCPP_DIRECTIVE_ELIFNDEF_IDENTIFIER;
								break;
							}

							macro_definition *def = macro_table_get_def(symbol_table, lexer_text);
							if (def->status == MACRO_UNDETERMINED) {
								PANIC("Macros used in `elifndef` must me explcitly defined/undefined: %s", lexer_text);
								break;
							}

							curr_scope->conditional_was_resolved = def->status == MACRO_UNDEFINED;
							curr_scope->should_process = def->status == MACRO_UNDEFINED;
							curr_scope->should_output = def->status == MACRO_UNDEFINED;
							state = PCPP_DIRECTIVE_ELIFNDEF_IDENTIFIER;
							break;
						default:
							PANIC("Directive `elifndef` followed by non-identifier: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;
				case PCPP_DIRECTIVE_ELIFNDEF_IDENTIFIER:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						default:
							PANIC("Extra token at end of `elifndef` derictives: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;

				/*****************************************************************************************************************/

				/* ELSE */
				case PCPP_DIRECTIVE_ELSE:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						default:
							PANIC("Extra token at end of `else` derictives: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;

				/*****************************************************************************************************************/

				/* ENDIF */
				case PCPP_DIRECTIVE_ENDIF:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						default:
							PANIC("Extra token at end of `endif` derictives: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;
			}
		}
		lexer__delete_buffer(line_buf);
	}
}
