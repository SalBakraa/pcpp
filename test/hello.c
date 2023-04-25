#include <stdio.h>

#include "string.c"

FUNC_MAIN {
	printf("%s ", HELLO);
#undef WORLD
#define WORLD "Test"
	printf("%s\n", WORLD);
}
