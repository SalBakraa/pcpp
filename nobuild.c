#define NOBUILD_IMPLEMENTATION
#include "./nobuild.h"

// Overridable macros

#define _BINARY_NAME "pcpp"
#ifdef BINARY_NAME
#	define _BINARY_NAME BINARY_NAME
#endif

// Paths

#define SRC_DIR PATH(".", "src")
#define BUILD_DIR PATH(".", "build")

// Build macros

#ifdef DEBUG
#	define DEBUG_FLAGS "-ggdb", "-O0"
#else
#	define DEBUG_FLAGS "-O3"
#endif

#define WARNING_FLAGS "-Wall", "-Wextra", "-Wshadow", "-Wconversion", "-Wduplicated-cond", "-Wduplicated-branches", "-Wrestrict", "-Wnull-dereference", "-Wjump-misses-init"
#define C_FLAGS "-std=c18", DEBUG_FLAGS, WARNING_FLAGS
#define BUILD_CMD "cc", C_FLAGS, CONCAT("-o ", PATH(BUILD_DIR, _BINARY_NAME)), PATH(SRC_DIR, "pcpp.c")

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// LSP stuff
static void create_ccls_file(const char *binary_path) {
	if (is_path1_modified_after_path2(binary_path, ".ccls")){
		CHAIN(CHAIN_CMD("echo", JOIN("\n", "clang", BUILD_CMD)), CHAIN_OUT(PATH(SRC_DIR, ".ccls")));
	}
}

// Compile the assets
static void build_tools(void) {
}

// Build the project
static void build(void) {
	if (!PATH_EXISTS(BUILD_DIR)) {
		MKDIRS(BUILD_DIR);
	}

	CMD(BUILD_CMD);
}

int main(int argc, char **argv)
{
	GO_REBUILD_URSELF(argc, argv);

	create_ccls_file(argv[0]);

	build();

	return 0;
}
