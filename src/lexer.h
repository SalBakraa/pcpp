#ifndef lexer_HEADER_H
#define lexer_HEADER_H 1
#define lexer_IN_HEADER 1

#line 5 "src/lexer.h"
#if defined(_MSC_VER) && !defined(__clang__) && !defined(WARNING_DISABLE) && !defined(WARNING_ENABLE)
#	define WARNING_DISABLE           \
		__pragma(warning(push, 0))       \
		__pragma(warning(disable: 4701)) \
		__pragma(warning(disable: 4702))
#	define WARNING_ENABLE \
		__pragma(warning(default: 4701)) \
		__pragma(warning(default: 4702)) \
		__pragma(warning(pop ))
#elif (defined(__GNUC__) || defined(__clang__)) && !defined(WARNING_DISABLE) && !defined(WARNING_ENABLE)
#	define WARNING_DISABLE \
		_Pragma("GCC diagnostic push") \
		_Pragma("GCC diagnostic ignored \"-Wunused-result\"") \
		_Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"") \
		_Pragma("GCC diagnostic ignored \"-Wformat\"") \
		_Pragma("GCC diagnostic ignored \"-Wswitch\"") \
		_Pragma("GCC diagnostic ignored \"-Wmisleading-indentation\"") \
		_Pragma("GCC diagnostic ignored \"-Wunused-but-set-variable\"") \
		_Pragma("GCC diagnostic ignored \"-Wint-in-bool-context\"") \
		_Pragma("GCC diagnostic ignored \"-Wunused-variable\"") \
		_Pragma("GCC diagnostic ignored \"-Wpointer-arith\"") \
		_Pragma("GCC diagnostic ignored \"-Wunused-function\"") \
		_Pragma("GCC diagnostic ignored \"-Wunused-variable\"") \
		_Pragma("GCC diagnostic ignored \"-Wnull-dereference\"")
#	define WARNING_ENABLE _Pragma("GCC diagnostic pop")
#elif !defined(WARNING_DISABLE) && !defined(WARNING_ENABLE)
#	define WARNING_DISABLE
#	define WARNING_ENABLE
#endif

WARNING_DISABLE

#define FOREACH_C_TOKENS(GEN_FUNC) \
	GEN_FUNC(DONE)                 \
	GEN_FUNC(IDENTIFIER)           \
	GEN_FUNC(PREPROCESSING_NUMBER) \
	GEN_FUNC(STRING_LITERAL)       \
	GEN_FUNC(CHAR_LITERAL)         \
	GEN_FUNC(HEADER_LITERAL)       \
	GEN_FUNC(COMMENT)              \
	GEN_FUNC(ELLIPSIS)             \
	GEN_FUNC(ASSIGN_L_SHIFT)       \
	GEN_FUNC(ASSIGN_R_SHIFT)       \
	GEN_FUNC(L_SHIFT)              \
	GEN_FUNC(R_SHIFT)              \
	GEN_FUNC(ARROW)                \
	GEN_FUNC(INCREMENT)            \
	GEN_FUNC(DECREMENT)            \
	GEN_FUNC(ASSIGN_PLUS)          \
	GEN_FUNC(ASSIGN_MINUS)         \
	GEN_FUNC(ASSIGN_STAR)          \
	GEN_FUNC(ASSIGN_DIV)           \
	GEN_FUNC(ASSIGN_MOD)           \
	GEN_FUNC(EQUAL_LESS)           \
	GEN_FUNC(EQUAL_GREATER)        \
	GEN_FUNC(EQUAL)                \
	GEN_FUNC(EQUAL_NOT)            \
	GEN_FUNC(AND)                  \
	GEN_FUNC(OR)                   \
	GEN_FUNC(ASSIGN_BIT_AND)       \
	GEN_FUNC(ASSIGN_BIT_OR)        \
	GEN_FUNC(ASSIGN_BIT_XOR)       \
	GEN_FUNC(DOUBLE_HASH)          \
	GEN_FUNC(HASH)                 \
	GEN_FUNC(L_BRACE)              \
	GEN_FUNC(R_BRACE)              \
	GEN_FUNC(L_BRACKET)            \
	GEN_FUNC(R_BRACKET)            \
	GEN_FUNC(L_PAREN)              \
	GEN_FUNC(R_PAREN)              \
	GEN_FUNC(L_ANGLE)              \
	GEN_FUNC(R_ANGLE)              \
	GEN_FUNC(SEMICOLON)            \
	GEN_FUNC(COMMA)                \
	GEN_FUNC(PERIOD)               \
	GEN_FUNC(MINUS)                \
	GEN_FUNC(PLUS)                 \
	GEN_FUNC(STAR)                 \
	GEN_FUNC(DIV)                  \
	GEN_FUNC(MOD)                  \
	GEN_FUNC(QUESTION_MARK)        \
	GEN_FUNC(COLON)                \
	GEN_FUNC(NOT_OP)               \
	GEN_FUNC(BIT_AND)              \
	GEN_FUNC(BIT_OR)               \
	GEN_FUNC(BIT_XOR)              \
	GEN_FUNC(BIT_NOT)              \
	GEN_FUNC(ASSIGN)               \
	GEN_FUNC(WHITESPACE)           \
	GEN_FUNC(OTHER)

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

typedef enum C_TOKENS {
    FOREACH_C_TOKENS(GENERATE_ENUM)
} C_TOKENS;

static const char *C_TOKENS_STRING[] = {
    FOREACH_C_TOKENS(GENERATE_STRING)
};

C_TOKENS lexer_lex(void);
#define YY_DECL C_TOKENS lexer_lex(void)

#include <stdio.h>
int fileno(FILE *stream);

#line 114 "src/lexer.h"

#define  YY_INT_ALIGNED short int

/* A lexical scanner generated by flex */

#define FLEX_SCANNER
#define YY_FLEX_MAJOR_VERSION 2
#define YY_FLEX_MINOR_VERSION 6
#define YY_FLEX_SUBMINOR_VERSION 4
#if YY_FLEX_SUBMINOR_VERSION > 0
#define FLEX_BETA
#endif

#ifdef yy_create_buffer
#define lexer__create_buffer_ALREADY_DEFINED
#else
#define yy_create_buffer lexer__create_buffer
#endif

#ifdef yy_delete_buffer
#define lexer__delete_buffer_ALREADY_DEFINED
#else
#define yy_delete_buffer lexer__delete_buffer
#endif

#ifdef yy_scan_buffer
#define lexer__scan_buffer_ALREADY_DEFINED
#else
#define yy_scan_buffer lexer__scan_buffer
#endif

#ifdef yy_scan_string
#define lexer__scan_string_ALREADY_DEFINED
#else
#define yy_scan_string lexer__scan_string
#endif

#ifdef yy_scan_bytes
#define lexer__scan_bytes_ALREADY_DEFINED
#else
#define yy_scan_bytes lexer__scan_bytes
#endif

#ifdef yy_init_buffer
#define lexer__init_buffer_ALREADY_DEFINED
#else
#define yy_init_buffer lexer__init_buffer
#endif

#ifdef yy_flush_buffer
#define lexer__flush_buffer_ALREADY_DEFINED
#else
#define yy_flush_buffer lexer__flush_buffer
#endif

#ifdef yy_load_buffer_state
#define lexer__load_buffer_state_ALREADY_DEFINED
#else
#define yy_load_buffer_state lexer__load_buffer_state
#endif

#ifdef yy_switch_to_buffer
#define lexer__switch_to_buffer_ALREADY_DEFINED
#else
#define yy_switch_to_buffer lexer__switch_to_buffer
#endif

#ifdef yypush_buffer_state
#define lexer_push_buffer_state_ALREADY_DEFINED
#else
#define yypush_buffer_state lexer_push_buffer_state
#endif

#ifdef yypop_buffer_state
#define lexer_pop_buffer_state_ALREADY_DEFINED
#else
#define yypop_buffer_state lexer_pop_buffer_state
#endif

#ifdef yyensure_buffer_stack
#define lexer_ensure_buffer_stack_ALREADY_DEFINED
#else
#define yyensure_buffer_stack lexer_ensure_buffer_stack
#endif

#ifdef yylex
#define lexer_lex_ALREADY_DEFINED
#else
#define yylex lexer_lex
#endif

#ifdef yyrestart
#define lexer_restart_ALREADY_DEFINED
#else
#define yyrestart lexer_restart
#endif

#ifdef yylex_init
#define lexer_lex_init_ALREADY_DEFINED
#else
#define yylex_init lexer_lex_init
#endif

#ifdef yylex_init_extra
#define lexer_lex_init_extra_ALREADY_DEFINED
#else
#define yylex_init_extra lexer_lex_init_extra
#endif

#ifdef yylex_destroy
#define lexer_lex_destroy_ALREADY_DEFINED
#else
#define yylex_destroy lexer_lex_destroy
#endif

#ifdef yyget_debug
#define lexer_get_debug_ALREADY_DEFINED
#else
#define yyget_debug lexer_get_debug
#endif

#ifdef yyset_debug
#define lexer_set_debug_ALREADY_DEFINED
#else
#define yyset_debug lexer_set_debug
#endif

#ifdef yyget_extra
#define lexer_get_extra_ALREADY_DEFINED
#else
#define yyget_extra lexer_get_extra
#endif

#ifdef yyset_extra
#define lexer_set_extra_ALREADY_DEFINED
#else
#define yyset_extra lexer_set_extra
#endif

#ifdef yyget_in
#define lexer_get_in_ALREADY_DEFINED
#else
#define yyget_in lexer_get_in
#endif

#ifdef yyset_in
#define lexer_set_in_ALREADY_DEFINED
#else
#define yyset_in lexer_set_in
#endif

#ifdef yyget_out
#define lexer_get_out_ALREADY_DEFINED
#else
#define yyget_out lexer_get_out
#endif

#ifdef yyset_out
#define lexer_set_out_ALREADY_DEFINED
#else
#define yyset_out lexer_set_out
#endif

#ifdef yyget_leng
#define lexer_get_leng_ALREADY_DEFINED
#else
#define yyget_leng lexer_get_leng
#endif

#ifdef yyget_text
#define lexer_get_text_ALREADY_DEFINED
#else
#define yyget_text lexer_get_text
#endif

#ifdef yyget_lineno
#define lexer_get_lineno_ALREADY_DEFINED
#else
#define yyget_lineno lexer_get_lineno
#endif

#ifdef yyset_lineno
#define lexer_set_lineno_ALREADY_DEFINED
#else
#define yyset_lineno lexer_set_lineno
#endif

#ifdef yywrap
#define lexer_wrap_ALREADY_DEFINED
#else
#define yywrap lexer_wrap
#endif

#ifdef yyalloc
#define lexer_alloc_ALREADY_DEFINED
#else
#define yyalloc lexer_alloc
#endif

#ifdef yyrealloc
#define lexer_realloc_ALREADY_DEFINED
#else
#define yyrealloc lexer_realloc
#endif

#ifdef yyfree
#define lexer_free_ALREADY_DEFINED
#else
#define yyfree lexer_free
#endif

#ifdef yytext
#define lexer_text_ALREADY_DEFINED
#else
#define yytext lexer_text
#endif

#ifdef yyleng
#define lexer_leng_ALREADY_DEFINED
#else
#define yyleng lexer_leng
#endif

#ifdef yyin
#define lexer_in_ALREADY_DEFINED
#else
#define yyin lexer_in
#endif

#ifdef yyout
#define lexer_out_ALREADY_DEFINED
#else
#define yyout lexer_out
#endif

#ifdef yy_flex_debug
#define lexer__flex_debug_ALREADY_DEFINED
#else
#define yy_flex_debug lexer__flex_debug
#endif

#ifdef yylineno
#define lexer_lineno_ALREADY_DEFINED
#else
#define yylineno lexer_lineno
#endif

/* First, we deal with  platform-specific or compiler-specific issues. */

/* begin standard C headers. */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

/* end standard C headers. */

/* flex integer type definitions */

#ifndef FLEXINT_H
#define FLEXINT_H

/* C99 systems have <inttypes.h>. Non-C99 systems may or may not. */

#if defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L

/* C99 says to define __STDC_LIMIT_MACROS before including stdint.h,
 * if you want the limit (max/min) macros for int types. 
 */
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS 1
#endif

#include <inttypes.h>
typedef int8_t flex_int8_t;
typedef uint8_t flex_uint8_t;
typedef int16_t flex_int16_t;
typedef uint16_t flex_uint16_t;
typedef int32_t flex_int32_t;
typedef uint32_t flex_uint32_t;
#else
typedef signed char flex_int8_t;
typedef short int flex_int16_t;
typedef int flex_int32_t;
typedef unsigned char flex_uint8_t; 
typedef unsigned short int flex_uint16_t;
typedef unsigned int flex_uint32_t;

/* Limits of integral types. */
#ifndef INT8_MIN
#define INT8_MIN               (-128)
#endif
#ifndef INT16_MIN
#define INT16_MIN              (-32767-1)
#endif
#ifndef INT32_MIN
#define INT32_MIN              (-2147483647-1)
#endif
#ifndef INT8_MAX
#define INT8_MAX               (127)
#endif
#ifndef INT16_MAX
#define INT16_MAX              (32767)
#endif
#ifndef INT32_MAX
#define INT32_MAX              (2147483647)
#endif
#ifndef UINT8_MAX
#define UINT8_MAX              (255U)
#endif
#ifndef UINT16_MAX
#define UINT16_MAX             (65535U)
#endif
#ifndef UINT32_MAX
#define UINT32_MAX             (4294967295U)
#endif

#ifndef SIZE_MAX
#define SIZE_MAX               (~(size_t)0)
#endif

#endif /* ! C99 */

#endif /* ! FLEXINT_H */

/* begin standard C++ headers. */

/* TODO: this is always defined, so inline it */
#define yyconst const

#if defined(__GNUC__) && __GNUC__ >= 3
#define yynoreturn __attribute__((__noreturn__))
#else
#define yynoreturn
#endif

/* Size of default input buffer. */
#ifndef YY_BUF_SIZE
#ifdef __ia64__
/* On IA-64, the buffer size is 16k, not 8k.
 * Moreover, YY_BUF_SIZE is 2*YY_READ_BUF_SIZE in the general case.
 * Ditto for the __ia64__ case accordingly.
 */
#define YY_BUF_SIZE 32768
#else
#define YY_BUF_SIZE 16384
#endif /* __ia64__ */
#endif

#ifndef YY_TYPEDEF_YY_BUFFER_STATE
#define YY_TYPEDEF_YY_BUFFER_STATE
typedef struct yy_buffer_state *YY_BUFFER_STATE;
#endif

#ifndef YY_TYPEDEF_YY_SIZE_T
#define YY_TYPEDEF_YY_SIZE_T
typedef size_t yy_size_t;
#endif

extern int yyleng;

extern FILE *yyin, *yyout;

#ifndef YY_STRUCT_YY_BUFFER_STATE
#define YY_STRUCT_YY_BUFFER_STATE
struct yy_buffer_state
	{
	FILE *yy_input_file;

	char *yy_ch_buf;		/* input buffer */
	char *yy_buf_pos;		/* current position in input buffer */

	/* Size of input buffer in bytes, not including room for EOB
	 * characters.
	 */
	int yy_buf_size;

	/* Number of characters read into yy_ch_buf, not including EOB
	 * characters.
	 */
	int yy_n_chars;

	/* Whether we "own" the buffer - i.e., we know we created it,
	 * and can realloc() it to grow it, and should free() it to
	 * delete it.
	 */
	int yy_is_our_buffer;

	/* Whether this is an "interactive" input source; if so, and
	 * if we're using stdio for input, then we want to use getc()
	 * instead of fread(), to make sure we stop fetching input after
	 * each newline.
	 */
	int yy_is_interactive;

	/* Whether we're considered to be at the beginning of a line.
	 * If so, '^' rules will be active on the next match, otherwise
	 * not.
	 */
	int yy_at_bol;

    int yy_bs_lineno; /**< The line count. */
    int yy_bs_column; /**< The column count. */

	/* Whether to try to fill the input buffer when we reach the
	 * end of it.
	 */
	int yy_fill_buffer;

	int yy_buffer_status;

	};
#endif /* !YY_STRUCT_YY_BUFFER_STATE */

void yyrestart ( FILE *input_file  );
void yy_switch_to_buffer ( YY_BUFFER_STATE new_buffer  );
YY_BUFFER_STATE yy_create_buffer ( FILE *file, int size  );
void yy_delete_buffer ( YY_BUFFER_STATE b  );
void yy_flush_buffer ( YY_BUFFER_STATE b  );
void yypush_buffer_state ( YY_BUFFER_STATE new_buffer  );
void yypop_buffer_state ( void );

YY_BUFFER_STATE yy_scan_buffer ( char *base, yy_size_t size  );
YY_BUFFER_STATE yy_scan_string ( const char *yy_str  );
YY_BUFFER_STATE yy_scan_bytes ( const char *bytes, int len  );

void *yyalloc ( yy_size_t  );
void *yyrealloc ( void *, yy_size_t  );
void yyfree ( void *  );

/* Begin user sect3 */

#define lexer_wrap() (/*CONSTCOND*/1)
#define YY_SKIP_YYWRAP

extern int yylineno;

extern char *yytext;
#ifdef yytext_ptr
#undef yytext_ptr
#endif
#define yytext_ptr yytext

#ifdef YY_HEADER_EXPORT_START_CONDITIONS
#define INITIAL 0

#endif

#ifndef YY_NO_UNISTD_H
/* Special case for "unistd.h", since it is non-ANSI. We include it way
 * down here because we want the user's section 1 to have been scanned first.
 * The user has a chance to override it with an option.
 */
#include <unistd.h>
#endif

#ifndef YY_EXTRA_TYPE
#define YY_EXTRA_TYPE void *
#endif

/* Accessor methods to globals.
   These are made visible to non-reentrant scanners for convenience. */

int yylex_destroy ( void );

int yyget_debug ( void );

void yyset_debug ( int debug_flag  );

YY_EXTRA_TYPE yyget_extra ( void );

void yyset_extra ( YY_EXTRA_TYPE user_defined  );

FILE *yyget_in ( void );

void yyset_in  ( FILE * _in_str  );

FILE *yyget_out ( void );

void yyset_out  ( FILE * _out_str  );

			int yyget_leng ( void );

char *yyget_text ( void );

int yyget_lineno ( void );

void yyset_lineno ( int _line_number  );

/* Macros after this point can all be overridden by user definitions in
 * section 1.
 */

#ifndef YY_SKIP_YYWRAP
#ifdef __cplusplus
extern "C" int yywrap ( void );
#else
extern int yywrap ( void );
#endif
#endif

#ifndef yytext_ptr
static void yy_flex_strncpy ( char *, const char *, int );
#endif

#ifdef YY_NEED_STRLEN
static int yy_flex_strlen ( const char * );
#endif

#ifndef YY_NO_INPUT

#endif

/* Amount of stuff to slurp up with each read. */
#ifndef YY_READ_BUF_SIZE
#ifdef __ia64__
/* On IA-64, the buffer size is 16k, not 8k */
#define YY_READ_BUF_SIZE 16384
#else
#define YY_READ_BUF_SIZE 8192
#endif /* __ia64__ */
#endif

/* Number of entries by which start-condition stack grows. */
#ifndef YY_START_STACK_INCR
#define YY_START_STACK_INCR 25
#endif

/* Default declaration of generated scanner - a define so the user can
 * easily add parameters.
 */
#ifndef YY_DECL
#define YY_DECL_IS_OURS 1

extern int yylex (void);

#define YY_DECL int yylex (void)
#endif /* !YY_DECL */

/* yy_get_previous_state - get the state just before the EOB char was reached */

#undef YY_NEW_FILE
#undef YY_FLUSH_BUFFER
#undef yy_set_bol
#undef yy_new_buffer
#undef yy_set_interactive
#undef YY_DO_BEFORE_ACTION

#ifdef YY_DECL_IS_OURS
#undef YY_DECL_IS_OURS
#undef YY_DECL
#endif

#ifndef lexer__create_buffer_ALREADY_DEFINED
#undef yy_create_buffer
#endif
#ifndef lexer__delete_buffer_ALREADY_DEFINED
#undef yy_delete_buffer
#endif
#ifndef lexer__scan_buffer_ALREADY_DEFINED
#undef yy_scan_buffer
#endif
#ifndef lexer__scan_string_ALREADY_DEFINED
#undef yy_scan_string
#endif
#ifndef lexer__scan_bytes_ALREADY_DEFINED
#undef yy_scan_bytes
#endif
#ifndef lexer__init_buffer_ALREADY_DEFINED
#undef yy_init_buffer
#endif
#ifndef lexer__flush_buffer_ALREADY_DEFINED
#undef yy_flush_buffer
#endif
#ifndef lexer__load_buffer_state_ALREADY_DEFINED
#undef yy_load_buffer_state
#endif
#ifndef lexer__switch_to_buffer_ALREADY_DEFINED
#undef yy_switch_to_buffer
#endif
#ifndef lexer_push_buffer_state_ALREADY_DEFINED
#undef yypush_buffer_state
#endif
#ifndef lexer_pop_buffer_state_ALREADY_DEFINED
#undef yypop_buffer_state
#endif
#ifndef lexer_ensure_buffer_stack_ALREADY_DEFINED
#undef yyensure_buffer_stack
#endif
#ifndef lexer_lex_ALREADY_DEFINED
#undef yylex
#endif
#ifndef lexer_restart_ALREADY_DEFINED
#undef yyrestart
#endif
#ifndef lexer_lex_init_ALREADY_DEFINED
#undef yylex_init
#endif
#ifndef lexer_lex_init_extra_ALREADY_DEFINED
#undef yylex_init_extra
#endif
#ifndef lexer_lex_destroy_ALREADY_DEFINED
#undef yylex_destroy
#endif
#ifndef lexer_get_debug_ALREADY_DEFINED
#undef yyget_debug
#endif
#ifndef lexer_set_debug_ALREADY_DEFINED
#undef yyset_debug
#endif
#ifndef lexer_get_extra_ALREADY_DEFINED
#undef yyget_extra
#endif
#ifndef lexer_set_extra_ALREADY_DEFINED
#undef yyset_extra
#endif
#ifndef lexer_get_in_ALREADY_DEFINED
#undef yyget_in
#endif
#ifndef lexer_set_in_ALREADY_DEFINED
#undef yyset_in
#endif
#ifndef lexer_get_out_ALREADY_DEFINED
#undef yyget_out
#endif
#ifndef lexer_set_out_ALREADY_DEFINED
#undef yyset_out
#endif
#ifndef lexer_get_leng_ALREADY_DEFINED
#undef yyget_leng
#endif
#ifndef lexer_get_text_ALREADY_DEFINED
#undef yyget_text
#endif
#ifndef lexer_get_lineno_ALREADY_DEFINED
#undef yyget_lineno
#endif
#ifndef lexer_set_lineno_ALREADY_DEFINED
#undef yyset_lineno
#endif
#ifndef lexer_get_column_ALREADY_DEFINED
#undef yyget_column
#endif
#ifndef lexer_set_column_ALREADY_DEFINED
#undef yyset_column
#endif
#ifndef lexer_wrap_ALREADY_DEFINED
#undef yywrap
#endif
#ifndef lexer_get_lval_ALREADY_DEFINED
#undef yyget_lval
#endif
#ifndef lexer_set_lval_ALREADY_DEFINED
#undef yyset_lval
#endif
#ifndef lexer_get_lloc_ALREADY_DEFINED
#undef yyget_lloc
#endif
#ifndef lexer_set_lloc_ALREADY_DEFINED
#undef yyset_lloc
#endif
#ifndef lexer_alloc_ALREADY_DEFINED
#undef yyalloc
#endif
#ifndef lexer_realloc_ALREADY_DEFINED
#undef yyrealloc
#endif
#ifndef lexer_free_ALREADY_DEFINED
#undef yyfree
#endif
#ifndef lexer_text_ALREADY_DEFINED
#undef yytext
#endif
#ifndef lexer_leng_ALREADY_DEFINED
#undef yyleng
#endif
#ifndef lexer_in_ALREADY_DEFINED
#undef yyin
#endif
#ifndef lexer_out_ALREADY_DEFINED
#undef yyout
#endif
#ifndef lexer__flex_debug_ALREADY_DEFINED
#undef yy_flex_debug
#endif
#ifndef lexer_lineno_ALREADY_DEFINED
#undef yylineno
#endif
#ifndef lexer_tables_fload_ALREADY_DEFINED
#undef yytables_fload
#endif
#ifndef lexer_tables_destroy_ALREADY_DEFINED
#undef yytables_destroy
#endif
#ifndef lexer_TABLES_NAME_ALREADY_DEFINED
#undef yyTABLES_NAME
#endif

#line 182 "src/lexer.l"


#line 816 "src/lexer.h"
#undef lexer_IN_HEADER
#endif /* lexer_HEADER_H */
