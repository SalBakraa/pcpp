%option 8bit
%option outfile="src/lexer.c" header-file="src/lexer.h" prefix="lexer_"
%option noyywrap

%top{
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
}

UNICODE_ESCAPE \\(u[[:digit:]]{4}|U[[:digit:]]{6})

%%

([_[:alpha:]$]|{UNICODE_ESCAPE})([_[:alnum:]$]|{UNICODE_ESCAPE})*   { return IDENTIFIER; }

\.?[[:digit:]](.|{UNICODE_ESCAPE})*                                 { return PREPROCESSING_NUMBER; }

\".*\"                                                              { return STRING_LITERAL; }
\'.*\'                                                              { return CHAR_LITERAL; }
\<.*\>                                                              { return HEADER_LITERAL; }

"/*".*"*/"                                                          { return COMMENT; }
"//".*                                                              { return COMMENT; }

"..."                                                               { return ELLIPSIS; }
"<<="                                                               { return ASSIGN_L_SHIFT; }
">>="                                                               { return ASSIGN_R_SHIFT; }
"<<"                                                                { return L_SHIFT; }
">>"                                                                { return R_SHIFT; }
"->"                                                                { return ARROW; }
"++"                                                                { return INCREMENT; }
"--"                                                                { return DECREMENT; }
"+="                                                                { return ASSIGN_PLUS; }
"-="                                                                { return ASSIGN_MINUS; }
"*="                                                                { return ASSIGN_STAR; }
"/="                                                                { return ASSIGN_DIV; }
"%="                                                                { return ASSIGN_MOD; }
"<="                                                                { return EQUAL_LESS; }
">="                                                                { return EQUAL_GREATER; }
"=="                                                                { return EQUAL; }
"!="                                                                { return EQUAL_NOT; }
"&&"                                                                { return AND; }
"||"                                                                { return OR; }
"&="                                                                { return ASSIGN_BIT_AND; }
"|="                                                                { return ASSIGN_BIT_OR; }
"^="                                                                { return ASSIGN_BIT_XOR; }
"##"|"%:%:"                                                         { return DOUBLE_HASH; }
"#"|"%:"                                                            { return HASH; }
"{"|"<%"                                                            { return L_BRACE; }
"}"|"%>"                                                            { return R_BRACE; }
"["|"<:"                                                            { return L_BRACKET; }
"]"|":>"                                                            { return R_BRACKET; }
"("                                                                 { return L_PAREN; }
")"                                                                 { return R_PAREN; }
"<"                                                                 { return L_ANGLE; }
">"                                                                 { return R_ANGLE; }
";"                                                                 { return SEMICOLON; }
","                                                                 { return COMMA; }
"."                                                                 { return PERIOD; }
"-"                                                                 { return MINUS; }
"+"                                                                 { return PLUS; }
"*"                                                                 { return STAR; }
"/"                                                                 { return DIV; }
"%"                                                                 { return MOD; }
"?"                                                                 { return QUESTION_MARK; }
":"                                                                 { return COLON; }
"!"                                                                 { return NOT_OP; }
"&"                                                                 { return BIT_AND; }
"|"                                                                 { return BIT_OR; }
"^"                                                                 { return BIT_XOR; }
"~"                                                                 { return BIT_NOT; }
"="                                                                 { return ASSIGN; }

[ \t]                                                               { return WHITESPACE; }
.                                                                   { return OTHER; }

%%
WARNING_ENABLE
