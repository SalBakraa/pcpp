#undef macro_1
#define macro_2

#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#else
print("no macro is defined");
#endif