#ifdef macro1
	print("macro1 is defined");
#	ifdef macro1_a
		print("macro1_a is defined");
#		ifndef macro1_a_a
			print("macro1_a_a is not defined");
#		endif
		print("tested for macro1_a_a");
#	elifdef macro1_b
		print("macro1_b is defined instead of macro1_a");
#	endif
#else
	print("macro1 not is not defined");
#	ifdef macro2
		print("macro2 not is not defined");
#	endif
#endif



#ifdef macro1
	print("macro1 is defined");
#	ifdef macro1_a
		print("macro1_a is defined");
#		ifndef macro1_a_a
			print("macro1_a_a is not defined");
#		endif
		print("tested for macro1_a_a");
			print("macro1_a_b is not defined");
		print("tested for macro1_a_b");
#	elifdef macro1_b
		print("macro1_b is defined instead of macro1_a");
#	endif
#else
	print("macro1 not is not defined");
#	ifdef macro2
		print("macro2 not is not defined");
#	endif
#endif



	print("macro1 is defined");
#	ifdef macro1_a
		print("macro1_a is defined");
#		ifndef macro1_a_a
			print("macro1_a_a is not defined");
#		endif
		print("tested for macro1_a_a");
#	elifdef macro1_b
		print("macro1_b is defined instead of macro1_a");
#	endif



	print("macro1 is defined");
#	ifdef macro1_a
		print("macro1_a is defined");
#		ifndef macro1_a_a
			print("macro1_a_a is not defined");
#		endif
		print("tested for macro1_a_a");
#	elifdef macro1_b
		print("macro1_b is defined instead of macro1_a");
#	endif



	print("macro1 is defined");
		print("macro1_a is defined");
#		ifndef macro1_a_a
			print("macro1_a_a is not defined");
#		endif
		print("tested for macro1_a_a");



	print("macro1 is defined");
		print("macro1_a is defined");
			print("macro1_a_a is not defined");
		print("tested for macro1_a_a");



	print("macro1 is defined");
		print("macro1_b is defined instead of macro1_a");



	print("macro1 not is not defined");
#	ifdef macro2
		print("macro2 not is not defined");
#	endif



	print("macro1 not is not defined");
		print("macro2 not is not defined");
