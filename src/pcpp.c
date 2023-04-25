#define NOBUILD_IMPLEMENTATION
#include "../nobuild.h"

#include <stdbool.h>

#include "lexer.h"

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

	char *buffer = malloc(f_size);
	if (buffer == NULL) {
		PANIC("Could not allocate memory: %s", nobuild__strerror(errno));
	}

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

	for (size_t i = 0; i < lines.count; ++i) {
		YY_BUFFER_STATE line_buf = lexer__scan_string(lines.elems[i]);
		INFO("Line %zu: %s", i, lines.elems[i]);
		while (true) {
			C_TOKENS tok = lexer_lex();
			if (tok == DONE) {
				break;
			}
			INFO("\tTOKEN: %s", C_TOKENS_STRING[tok]);
		}

		lexer__delete_buffer(line_buf);
	}
}
