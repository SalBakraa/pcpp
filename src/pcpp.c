#define NOBUILD_IMPLEMENTATION
#include "../nobuild.h"

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
}
