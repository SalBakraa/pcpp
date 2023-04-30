#include <stdio.h>

#include "string.c"

FUNC_MAIN {
#ifdef HELLO
	printf("%s ", HELLO);
#endif

#undef WORLD
#define WORLD "Test"
	printf("%s\n", WORLD);
}
