#include "string.c"

#define baz
#define bar(a) do{(a)+1} \
               while(0)
#ifdef X
#	ifdef Y
#		ifdef Z
			so_far_so_good = true;
#		endif
#		ifdef baz
			something();
#		endif
#	else
		otherthing();
#	endif
#else
#	ifdef Z
		wow().this.is.bad;
#	endif
#endif
