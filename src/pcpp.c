#define NOBUILD_IMPLEMENTATION
#include "../nobuild.h"

#ifndef _WIN32
static Fd fd_stdin = STDIN_FILENO;
static Fd fd_stdout = STDOUT_FILENO;
static Fd fd_stderr = STDERR_FILENO;
#else
static Fd fd_stdin = GetStdHandle(STD_INPUT_HANDLE);
static Fd fd_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
static Fd fd_stderr = GetStdHandle(STD_ERROR_HANDLE);
#endif

#include <limits.h>
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
	PCPP_MULTILINE_COMMENT,

	PCPP_NON_DIRECTIVE,

	PCPP_DIRECTIVE,

	PCPP_DIRECTIVE_UNDEF,
	PCPP_DIRECTIVE_UNDEF_IDENTIFIER,

	PCPP_DIRECTIVE_DEF,
	PCPP_DIRECTIVE_DEF_IDENTIFIER,
	PCPP_DIRECTIVE_DEF_IDENTIFIER_ARGS,
	PCPP_DIRECTIVE_DEF_IDENTIFIER_ARGS_CMD,
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

	PCPP_DIRECTIVE_INCLUDE,
	PCPP_DIRECTIVE_INCLUDE_FILE,
} PCPP_STATE ;


void append_to_line(Cstr_Array *cstrs, Cstr cstr) {
	size_t len = strlen(cstr);
	char *temp = malloc(sizeof *temp * (len + 1));
	if (temp == NULL) {
		PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
	}

	temp = memcpy(temp, cstr, len+1);

	if (cstrs->capacity < 1) {
		cstrs->elems = realloc(cstrs->elems, sizeof *cstrs->elems * (cstrs->count + 10));
		cstrs->capacity += 10;
		if (cstrs->elems == NULL) {
			PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
		}
	}

	cstrs->elems[cstrs->count++] = temp;
	cstrs->capacity--;
}

bool cstr_array_contains(Cstr_Array *arr, Cstr val) {
	if (arr == NULL) {
		return false;
	}

	for (size_t i = 0; i < arr->count; ++i) {
		if (strcmp(val, arr->elems[i]) == 0) {
			return true;
		}
	}
	return false;
}

// List of identifiers allowed to be expanded and defined/undefined
Cstr_Array allowed_identifiers = {0};

// Flag that overrides allowed_identifiers
bool process_all_identifiers = false;

// List of file names allowed to be expanded into the final output
Cstr_Array allowed_files = {0};

// Flag that overrides allowed_identifiers
bool include_all_files = false;

// Flag to surround include derictives with line directives
bool surround_includes_with_line = false;

// User table of definitions
macro_table *user_symbol_table = NULL;

// Methods to resolve conflict between the user defined symbols, and source defined symbols
typedef enum CONFLICT_RESOLUTION_STRATEGIES {
	KEEP_USER, KEEP_SOURCE, REMOVE_BOTH
} CONFLICT_RESOLUTION_STRATEGIES;

// Avoid overwriting user definitions when processing
CONFLICT_RESOLUTION_STRATEGIES conflict_strat = KEEP_USER;

void pre_process_file(Cstr filename, macro_table *symbol_table, scope_stack *scopes, unsigned int depth) {
	if (depth > 200) {
		return;
	}

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
	fd_close(f);

	// Replace all null bytes with whitespace excluding the terminating byte
	for (size_t i = 0; i < read_count; ++i) {
		buffer[i] = buffer[i] == '\0' ? ' ' : buffer[i];
	}

	Cstr_Array lines = cstr_split_newline(buffer);
	free(buffer);

	// Change directory to that of the current file
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) == NULL) {
		PANIC("Could not retrieve current working directory.");
	}

	Cstr filedir = path_dirname(filename);
	if (chdir(filedir) < 0) {
		PANIC("Could not change current directory to '%s': %s", filedir, nobuild__strerror(errno));
	}

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


	bool is_inside_comment = false;
	PCPP_STATE stored_state = PCPP_INITIAL; // Used when exiting multi-line comments
	Cstr_Array stored_output_line = cstr_array_make(NULL);
	bool stored_append_current_token = true;
	bool stored_current_line_was_processed = false;


	// The last line is an empty extra line
	for (size_t i = 0; i < lines.count-1; ++i) {
		YY_BUFFER_STATE line_buf = lexer__scan_string(lines.elems[i]);
		scope_item *curr_scope = scope_stack_peek(scopes);

		// Line that will be outputted
		Cstr_Array output_line = cstr_array_make(NULL);
		bool append_current_token = true;
		bool current_line_was_processed = false;

		PCPP_STATE state = !is_inside_comment ? PCPP_INITIAL : PCPP_MULTILINE_COMMENT;
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
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
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

				/* Inside multi-line comments */
				case PCPP_MULTILINE_COMMENT:
					switch (tok) {
						case COMMENT_END:
							is_inside_comment = false;

							state = stored_state;
							append_current_token = stored_append_current_token;
							current_line_was_processed = stored_current_line_was_processed;

							output_line = cstr_array_concat(cstr_array_make(NULL), stored_output_line);
							stored_output_line = cstr_array_make(NULL);
							break;
						default:
							break;
					}
					break;

				/*****************************************************************************************************************/

				/* Regular C code */
				case PCPP_NON_DIRECTIVE:
					switch (tok) {
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
							break;
						default:
							break;
					}
					break;

				/*****************************************************************************************************************/

				/* Directives */
				case PCPP_DIRECTIVE:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
							break;
						case IDENTIFIER:
							if (strcmp(lexer_text, "undef") == 0) {
								state = PCPP_DIRECTIVE_UNDEF;
							} else if (strcmp(lexer_text, "define") == 0) {
								state = PCPP_DIRECTIVE_DEF;
							} else if (strcmp(lexer_text, "if") == 0) {
								// Copy parent scopes values
								scope_item *top = scope_stack_push(scopes);
								top->conditional_was_processed = false;
								top->conditional_was_resolved = false;
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
								if (curr_scope->conditional_was_processed) {
									TODO_SAFE("Confirm tha else is the last branch.");
									current_line_was_processed = true;
									curr_scope->should_process = !curr_scope->conditional_was_resolved;
									curr_scope->should_output = !curr_scope->conditional_was_resolved;
									curr_scope->conditional_was_resolved = !curr_scope->conditional_was_resolved;
								}
							} else if (strcmp(lexer_text, "endif") == 0) {
								if (curr_scope->conditional_was_processed) {
									current_line_was_processed = true;
								}

								scope_stack_pop(scopes);
								curr_scope = scope_stack_peek(scopes);
								state = PCPP_DIRECTIVE_ENDIF;
							} else if (strcmp(lexer_text, "include") == 0) {
								state = PCPP_DIRECTIVE_INCLUDE;
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
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
							break;
						case IDENTIFIER: {
							if (!curr_scope->should_process || !(process_all_identifiers || cstr_array_contains(&allowed_identifiers, lexer_text))) {
								state = PCPP_DIRECTIVE_UNDEF_IDENTIFIER;
								break;
							}

							current_line_was_processed = true;
							switch(conflict_strat) {
								case KEEP_USER: {
									if (macro_table_get_def(user_symbol_table, lexer_text)->status != MACRO_UNDETERMINED) {
										macro_table_get_def(symbol_table, lexer_text)->status = MACRO_UNDETERMINED;
									} else {
										macro_table_get_def(symbol_table, lexer_text)->status = MACRO_UNDEFINED;
									}
									break;
								}
								case KEEP_SOURCE: {
									macro_table_get_def(user_symbol_table, lexer_text)->status = MACRO_UNDETERMINED;
									macro_table_get_def(symbol_table, lexer_text)->status = MACRO_UNDEFINED;
									break;
								}
								case REMOVE_BOTH: {
									macro_table_get_def(user_symbol_table, lexer_text)->status = MACRO_UNDETERMINED;
									macro_table_get_def(symbol_table, lexer_text)->status = MACRO_UNDETERMINED;
									break;
								}
							}
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
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
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
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
							break;
						case IDENTIFIER:
							if (!curr_scope->should_process || !(process_all_identifiers || cstr_array_contains(&allowed_identifiers, lexer_text))) {
								state = PCPP_DIRECTIVE_DEF_IDENTIFIER;
								break;
							}

							current_line_was_processed = true;
							switch(conflict_strat) {
								case KEEP_USER: {
									if (macro_table_get_def(user_symbol_table, lexer_text)->status != MACRO_UNDETERMINED) {
										macro_table_push(symbol_table, lexer_text)->status = MACRO_UNDETERMINED;
									} else {
										macro_table_push(symbol_table, lexer_text)->status = MACRO_DEFINED;
									}
									break;
								}
								case KEEP_SOURCE: {
									macro_table_get_def(user_symbol_table, lexer_text)->status = MACRO_UNDETERMINED;
									macro_table_push(symbol_table, lexer_text)->status = MACRO_UNDEFINED;
									break;
								}
								case REMOVE_BOTH: {
									macro_table_get_def(user_symbol_table, lexer_text)->status = MACRO_UNDETERMINED;
									macro_table_push(symbol_table, lexer_text)->status = MACRO_UNDETERMINED;
									break;
								}
							}
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
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
							break;
						case WHITESPACE:
							state = PCPP_DIRECTIVE_DEF_IDENTIFIER_REPLACEMENT;
							break;
						case L_PAREN:
							if (!curr_scope->should_process || !(process_all_identifiers || cstr_array_contains(&allowed_identifiers, lexer_text))) {
								state = PCPP_DIRECTIVE_DEF_IDENTIFIER_ARGS;
								break;
							}

							macro_table_peek(symbol_table)->is_function = true;
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
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
							break;
						case R_PAREN:
							state = PCPP_DIRECTIVE_DEF_IDENTIFIER_REPLACEMENT;
							break;
						case IDENTIFIER:
							if (curr_scope->should_process && (process_all_identifiers || cstr_array_contains(&allowed_identifiers, lexer_text))) {
								macro_definition_push_args(macro_table_peek(symbol_table), lexer_text);
							}
							break;
						case ELLIPSIS:
							if (curr_scope->should_process && (process_all_identifiers || cstr_array_contains(&allowed_identifiers, lexer_text))) {
								macro_definition_push_args(macro_table_peek(symbol_table), lexer_text);
								TODO_SAFE("Verify that the ellipsis is the last argument.");
							}
							break;
						default:
							PANIC("Illegal token in place of argument: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;
				case PCPP_DIRECTIVE_DEF_IDENTIFIER_REPLACEMENT:
					if (curr_scope->should_process && (process_all_identifiers || cstr_array_contains(&allowed_identifiers, lexer_text))) {
						macro_definition_push_replacement(macro_table_peek(symbol_table), lexer_text);
					}
					break;

				/*****************************************************************************************************************/

				/* IFDEF */
				case PCPP_DIRECTIVE_IFDEF:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
							break;
						case IDENTIFIER: {
							// User table has higher priority
							macro_definition *def = macro_table_get_def(user_symbol_table, lexer_text);
							if (def->status == MACRO_UNDETERMINED) {
								def = macro_table_get_def(symbol_table, lexer_text);
							}

							scope_item *top = scope_stack_push(scopes);
							if (!(process_all_identifiers || cstr_array_contains(&allowed_identifiers, lexer_text)) || def->status == MACRO_UNDETERMINED) {
								top->conditional_was_processed = false;
								top->should_process = curr_scope->should_process;
								top->should_output = curr_scope->should_process;
								state = PCPP_DIRECTIVE_IFDEF_IDENTIFIER;
								break;
							}

							if (!curr_scope->should_process) {
								state = PCPP_DIRECTIVE_IFDEF_IDENTIFIER;
								break;
							}
							current_line_was_processed = true;
							top->conditional_was_processed = true;
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
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
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
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
							break;
						case IDENTIFIER: {
							// User table has higher priority
							macro_definition *def = macro_table_get_def(user_symbol_table, lexer_text);
							if (def->status == MACRO_UNDETERMINED) {
								def = macro_table_get_def(symbol_table, lexer_text);
							}

							scope_item *top = scope_stack_push(scopes);
							if (!(process_all_identifiers || cstr_array_contains(&allowed_identifiers, lexer_text)) || def->status == MACRO_UNDETERMINED) {
								top->conditional_was_processed = false;
								top->should_process = curr_scope->should_process;
								top->should_output = curr_scope->should_process;
								state = PCPP_DIRECTIVE_IFNDEF_IDENTIFIER;
								break;
							}

							if (!curr_scope->should_process) {
								state = PCPP_DIRECTIVE_IFNDEF_IDENTIFIER;
								break;
							}

							current_line_was_processed = true;
							top->conditional_was_processed = true;
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
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
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
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
							break;
						case IDENTIFIER: {
							if (!curr_scope->conditional_was_processed) {
								state = PCPP_DIRECTIVE_ELIFDEF_IDENTIFIER;
								break;
							}

							current_line_was_processed = true;
							if (curr_scope->conditional_was_resolved) {
								curr_scope->should_process = false;
								curr_scope->should_output = false;
								state = PCPP_DIRECTIVE_ELIFDEF_IDENTIFIER;
								break;
							}

							// User table has higher priority
							macro_definition *def = macro_table_get_def(user_symbol_table, lexer_text);
							if (def->status == MACRO_UNDETERMINED) {
								def = macro_table_get_def(symbol_table, lexer_text);
							}

							if (def->status == MACRO_UNDETERMINED) {
								PANIC("Macros used in `elifdef` must me explcitly defined/undefined: %s", lexer_text);
								break;
							}

							curr_scope->conditional_was_resolved = def->status == MACRO_DEFINED;
							curr_scope->should_process = def->status == MACRO_DEFINED;
							curr_scope->should_output = def->status == MACRO_DEFINED;
							state = PCPP_DIRECTIVE_ELIFDEF_IDENTIFIER;
							break;
						}
						default:
							PANIC("Directive `elifdef` followed by non-identifier: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;
				case PCPP_DIRECTIVE_ELIFDEF_IDENTIFIER:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
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
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
							break;
						case IDENTIFIER: {
							if (!curr_scope->conditional_was_processed) {
								state = PCPP_DIRECTIVE_ELIFNDEF_IDENTIFIER;
								break;
							}

							current_line_was_processed = true;
							if (curr_scope->conditional_was_resolved) {
								curr_scope->should_process = false;
								curr_scope->should_output = false;
								state = PCPP_DIRECTIVE_ELIFNDEF_IDENTIFIER;
								break;
							}

							// User table has higher priority
							macro_definition *def = macro_table_get_def(user_symbol_table, lexer_text);
							if (def->status == MACRO_UNDETERMINED) {
								def = macro_table_get_def(symbol_table, lexer_text);
							}

							if (def->status == MACRO_UNDETERMINED) {
								PANIC("Macros used in `elifndef` must me explcitly defined/undefined: %s", lexer_text);
								break;
							}

							curr_scope->conditional_was_resolved = def->status == MACRO_UNDEFINED;
							curr_scope->should_process = def->status == MACRO_UNDEFINED;
							curr_scope->should_output = def->status == MACRO_UNDEFINED;
							state = PCPP_DIRECTIVE_ELIFNDEF_IDENTIFIER;
							break;
						}
						default:
							PANIC("Directive `elifndef` followed by non-identifier: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;
				case PCPP_DIRECTIVE_ELIFNDEF_IDENTIFIER:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
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
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
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
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
							break;
						default:
							PANIC("Extra token at end of `endif` derictives: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;

				/*****************************************************************************************************************/

				/* INCLUDE */
				case PCPP_DIRECTIVE_INCLUDE:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
							break;
						case IDENTIFIER:
							TODO_SAFE("Expand `include` derictives: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
							break;
						case STRING_LITERAL: {
							if (!curr_scope->should_process) {
								state = PCPP_DIRECTIVE_INCLUDE_FILE;
								break;
							}
							current_line_was_processed = true;

							// trim quotes from around filename
							size_t len = strlen(lexer_text) - 2;
							if (len == 0) {
								PANIC("Empty filename passed to `include` directive.");
							}

							char *included_file = malloc(sizeof *included_file * (len + 1));
							if (included_file == NULL) {
								PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
							}

							included_file[len] = '\0';
							strncpy(included_file, lexer_text + 1, len);
							if (!(include_all_files || cstr_array_contains(&allowed_files, included_file))) {
								current_line_was_processed = false;
								state = PCPP_DIRECTIVE_INCLUDE_FILE;
								break;
							}

							if (surround_includes_with_line) {
								fd_printf(fd_stdout, "#line %d \"%s\"\n", 0, included_file);
							}
							pre_process_file(included_file, symbol_table, scopes, depth + 1);
							if (surround_includes_with_line) {
								fd_printf(fd_stdout, "#line %zu \"%s\"\n", i, filename);
							}
							lexer__switch_to_buffer(line_buf);
							__attribute__ ((fallthrough));
						}
						case HEADER_LITERAL:
							state = PCPP_DIRECTIVE_INCLUDE_FILE;
							break;
						default:
							PANIC("Illegal token at end of `include` derictives: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;
				case PCPP_DIRECTIVE_INCLUDE_FILE:
					switch (tok) {
						case COMMENT:
						case WHITESPACE:
							break;
						case COMMENT_START:
							is_inside_comment = true;

							stored_state = state;
							stored_output_line = cstr_array_concat(stored_output_line, output_line);

							stored_append_current_token = append_current_token;
							stored_current_line_was_processed = current_line_was_processed;

							state = PCPP_MULTILINE_COMMENT;
							break;
						case COMMENT_END:
							PANIC("Multi-line comment terminator outside of multi-line comment.");
							break;
						default:
							PANIC("Extra token at end of `include` derictives: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
					}
					break;

				/*****************************************************************************************************************/

				case PCPP_DIRECTIVE_DEF_IDENTIFIER_ARGS_CMD:
					// Case is only used in `macro_table_push_from_cmd`
					__builtin_unreachable();
			}
			if (is_inside_comment) {
				append_to_line(&stored_output_line, lexer_text);
			}
			if (append_current_token) {
				append_to_line(&output_line, lexer_text);
			}
		}
		if (is_inside_comment) {
			append_to_line(&stored_output_line, "\n");
		}
		if (!is_inside_comment && curr_scope->should_output && !current_line_was_processed) {
			fd_printf(fd_stdout, "%s\n", cstr_array_join("", output_line));
		}
		lexer__delete_buffer(line_buf);
	}
	if (chdir(cwd) < 0) {
		PANIC("Could not restore current directory to '%s': %s", cwd, nobuild__strerror(errno));
	}
}

// Used only to parse the argument of `-D`
macro_definition *macro_table_push_from_cmd(macro_table *table, Cstr str) {
	YY_BUFFER_STATE line_buf = lexer__scan_string(str);
	PCPP_STATE state = PCPP_DIRECTIVE_DEF;
	while (true) {
		C_TOKENS tok = lexer_lex();
		if (tok == DONE) {
			break;
		}

		switch (state) {
			case PCPP_DIRECTIVE_DEF:
				switch (tok) {
					case IDENTIFIER:
						macro_table_push(table, lexer_text)->status = MACRO_DEFINED;
						state = PCPP_DIRECTIVE_DEF_IDENTIFIER;
						break;
					default:
						PANIC("Argument to `-D` must start with with an identifier: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
				}
				break;
			case PCPP_DIRECTIVE_DEF_IDENTIFIER:
				switch (tok) {
					case ASSIGN:
						state = PCPP_DIRECTIVE_DEF_IDENTIFIER_REPLACEMENT;
						break;
					case L_PAREN:
						macro_table_peek(table)->is_function = true;
						state = PCPP_DIRECTIVE_DEF_IDENTIFIER_ARGS;
						break;
					default:
						PANIC("`-D` identifier must either be followed by `=` or `(`: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
				}
				break;
			case PCPP_DIRECTIVE_DEF_IDENTIFIER_ARGS:
				switch (tok) {
					case COMMA:
					case WHITESPACE:
						TODO_SAFE("Verify that each argument is delimited only by comma.");
						break;
					case R_PAREN:
						state = PCPP_DIRECTIVE_DEF_IDENTIFIER_REPLACEMENT;
						break;
					case IDENTIFIER:
						macro_definition_push_args(macro_table_peek(table), lexer_text);
						break;
					case ELLIPSIS:
						macro_definition_push_args(macro_table_peek(table), lexer_text);
						TODO_SAFE("Verify that the ellipsis is the last argument.");
						break;
					default:
						PANIC("Illegal token in place of argument: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
						break;
				}
				break;
			case PCPP_DIRECTIVE_DEF_IDENTIFIER_ARGS_CMD:
				switch (tok) {
					case ASSIGN:
						state = PCPP_DIRECTIVE_DEF_IDENTIFIER_REPLACEMENT;
						break;
					default:
						PANIC("Macro argument list must be only followed by `=`: %s -> %s", C_TOKENS_STRING[tok], lexer_text);
						break;
				}
				break;
			case PCPP_DIRECTIVE_DEF_IDENTIFIER_REPLACEMENT:
				macro_definition_push_replacement(macro_table_peek(table), lexer_text);
				break;
			default:
				__builtin_unreachable();
		}
	}
	lexer__delete_buffer(line_buf);
	return macro_table_peek(table);
}


int main(int argc, char **argv) {
	if (argc < 2) {
		PANIC("No arguments were specified.");
	}

	user_symbol_table = macro_table_make();

	Cstr *filename = NULL;
	for (int i = 1; i < argc; ++i) {
		if (STARTS_WITH(argv[i], "--only-process")) {
			Cstr id_list;
			if (STARTS_WITH(argv[i], "--only-process=")) {
				id_list = argv[i] + strlen("--only-process=");
			} else {
				if ((i + 1) >= argc) {
					PANIC("Missing argument to `--only-process`.");
				}
				id_list = argv[++i];
			}

			allowed_identifiers = SPLIT(id_list, ",");
			continue;
		}

		if (STARTS_WITH(argv[i], "--process-all")) {
			process_all_identifiers = true;
			continue;
		}

		if (STARTS_WITH(argv[i], "--only-include")) {
			Cstr file_list;
			if (STARTS_WITH(argv[i], "--only-include=")) {
				file_list = argv[i] + strlen("--only-include=");
			} else {
				if ((i + 1) >= argc) {
					PANIC("Missing argument to `--only-include`.");
				}
				file_list = argv[++i];
			}

			allowed_files = SPLIT(file_list, ",");
			continue;
		}

		if (STARTS_WITH(argv[i], "--include-all")) {
			include_all_files = true;
			continue;
		}

		if (STARTS_WITH(argv[i], "-U")) {
			if (strlen(argv[i]) < 3) {
				PANIC("Missing argument to `-U`.");
			}
			Cstr id = argv[i] + strlen("-U");
			macro_table_push(user_symbol_table, id)->status = MACRO_UNDEFINED;
			continue;
		}

		if (STARTS_WITH(argv[i], "--undef")) {
			Cstr id;
			if (STARTS_WITH(argv[i], "--undef=")) {
				id = argv[i] + strlen("--undef=");
			} else {
				if ((i + 1) >= argc) {
					PANIC("Missing argument to `--undef`.");
				}
				id = argv[++i];
			}

			macro_table_push(user_symbol_table, id)->status = MACRO_UNDEFINED;
			continue;
		}

		if (STARTS_WITH(argv[i], "-D")) {
			if (strlen(argv[i]) < 3) {
				PANIC("Missing argument to `-D`.");
			}
			Cstr id = argv[i] + strlen("-D");
			macro_table_push_from_cmd(user_symbol_table, id)->status = MACRO_DEFINED;
			continue;
		}

		if (STARTS_WITH(argv[i], "--define")) {
			Cstr id;
			if (STARTS_WITH(argv[i], "--define=")) {
				id = argv[i] + strlen("--define=");
			} else {
				if ((i + 1) >= argc) {
					PANIC("Missing argument to `--define`.");
				}
				id = argv[++i];
			}

			macro_table_push_from_cmd(user_symbol_table, id)->status = MACRO_DEFINED;
			continue;
		}

		if (STARTS_WITH(argv[i], "--conflict")) {
			Cstr strat;
			if (STARTS_WITH(argv[i], "--conflict=")) {
				strat = argv[i] + strlen("--conflict=");
			} else {
				if ((i + 1) >= argc) {
					PANIC("Missing argument to `--conflict`.");
				}
				strat = argv[++i];
			}

			if (strcmp(strat, "user") == 0) {
				conflict_strat = KEEP_USER;
			} else if (strcmp(strat, "source") == 0) {
				conflict_strat = KEEP_SOURCE;
			} else if (strcmp(strat, "ignore") == 0) {
				conflict_strat = REMOVE_BOTH;
			} else {
				PANIC("Invalid value to `--conflict`. Only `user`, `source`, and `ignore` are allowed.");
			}
			continue;
		}

		if (STARTS_WITH(argv[i], "--line-around-include")) {
			surround_includes_with_line = true;
			continue;
		}

		if (filename != NULL) {
			PANIC("Input file has already been specified: '%s' replaces '%s'", argv[i], *filename);
		}

		filename = (Cstr *) &argv[i];
	}

	if (filename == NULL) {
		PANIC("Input file has not been specified.");
	}

	// Simply output the file if no identifiers are allowed to used
	if (!process_all_identifiers && !include_all_files && allowed_identifiers.count == 0 && allowed_files.count == 0) {
		char buf[4096];
		Fd fd = fd_open_for_read(*filename);
		size_t read_bytes = 0;
		while ((read_bytes = fd_read(fd, buf, 4096)) != 0) {
			fd_write(fd_stdout, buf, read_bytes);
		}
		fd_close(fd);
		return 0;
	}

	// Disable TODO and INFO messages.
	logLevel = LOG_LEVELS_WARN;

	pre_process_file(*filename, macro_table_make(), scope_stack_make(), 0);
}
