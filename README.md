# PCPP &mdash; The Pre-C Preprocessor

The Pre-C Preprocessor is a lightweight tool designed to partially preprocess C code. This tool is intended to simplify
C code that is littered with large amounts of derictives.

## Behaviour

Like `coan`, `pcpp` treats any macro that has not been explicitly defined or undefined as *UNDETERMINED*. This mostly
affects conditionals. If a conditional has any undetermined macros, the conditional is not processed, but the contents
of all the branches are processed. Therefore, a define directive inside an undetermined conditional will be resolved,
which might lead to undesired behavior. To turn off this behavior, use `--ignore-undetermined`. As an example:

```c
#ifdef SOME_MACRO
#	define OTHER_MACRO
#endif
```

Running `pcpp --define-all input` leads to:

```c
#ifdef SOME_MACRO
#endif
```

## Building

1. Bootstrap build system: `cc -o nobuild nobuild.c`
2. Run build system: `./nobuild`

## Examples

The file `string.h`:

```c
#ifndef string_h
#define string_h

#ifdef MACRO
void macro_func();
#elifdef
void macro_func_undef();
#endif

void void_func();

#endif  // string_h

#ifdef string_implementation
#ifndef string_i
#define string_i

void void_func() {
}

#endif  // string_i
#endif  // string_implementation
```

The file `main.c`:

```c
#define MACRO
#include "string.h"

#ifdef X
#	ifndef MACRO
		do_thing();
#	endif
#	ifdef Y
		do_thing_with_Y_and_X();
#	endif
#elifdef Y
#	ifndef Z
#		define Z
		do_thing_with_Y_and_Z();
#	endif
#elifndef Z
	do_setup();
#else
	crash();
#endif

#define string_implementation
#include "string.h"
```

Commands:

1. `pcpp --implicitly-undef --only=string_h,string_i,string_implementation --include-all main.c`:
	```c
	#define MACRO

	#ifdef MACRO
	void macro_func();
	#elifdef
	void macro_func_undef();
	#endif

	void void_func();



	int main(void)
	{
	#ifdef X
	#	ifndef MACRO
			do_thing();
	#	endif
	#	ifdef Y
			do_thing_with_Y_and_X();
	#	endif
	#elifdef Y
	#	ifndef Z
	#		define Z
			do_thing_with_Y_and_Z();
	#	endif
	#elifndef Z
		do_setup();
	#else
		crash();
	#endif
	}



	void void_func() {
	}
	```

2. `pcpp --implicitly-undef --only-process=MACRO,Y main.c `:
	```c
	#define MACRO
	#include "string.h"

	int main(void)
	{
	#ifdef X
			do_thing();
	#elifdef Y
	#	ifndef Z
	#		define Z
			do_thing_with_Y_and_Z();
	#	endif
	#elifndef Z
		do_setup();
	#else
		crash();
	#endif
	}

	#define string_implementation
	#include "string.h"
	```

3. `pcpp -DZ --only=MACRO,Z main.c`:
	```c
	#include "string.h"

	int main(void)
	{
	#ifdef X
	#	ifdef Y
			do_thing_with_Y_and_X();
	#	endif
	#elifdef Y
	#elifndef Z
		do_setup();
	#else
		crash();
	#endif
	}

	#define string_implementation
	#include "string.h"
	```

4. `pcpp -DX -DY --only=Y main.c`
	```c
	#define MACRO
	#include "string.h"

	int main(void)
	{
	#ifdef X
	#	ifndef MACRO
			do_thing();
	#	endif
			do_thing_with_Y_and_X();
	#elifdef Y
	#	ifndef Z
	#		define Z
			do_thing_with_Y_and_Z();
	#	endif
	#elifndef Z
		do_setup();
	#else
		crash();
	#endif
	}

	#define string_implementation
	#include "string.h"
	```
