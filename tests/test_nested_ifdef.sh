#!/usr/bin/sh

test_case_name='test_nested_ifdef case: no macro is defined'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
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
EOF

cat << EOF > expected_output
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
EOF

build/pcpp test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_nested_ifdef case: top ifdef succeeded'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#define macro1

#ifdef macro1
	print("macro1 is defined");
#	ifdef macro1_a
		print("macro1_a is defined");
#		ifndef macro1_a_a
			print("macro1_a_a is not defined");
#		endif
		print("tested for macro1_a_a");
#		ifdef macro1_a_b
			print("macro1_a_b is not defined");
#		endif
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
EOF

cat << EOF > expected_output

	print("macro1 is defined");
#	ifdef macro1_a
		print("macro1_a is defined");
#		ifndef macro1_a_a
			print("macro1_a_a is not defined");
#		endif
		print("tested for macro1_a_a");
#		ifdef macro1_a_b
			print("macro1_a_b is not defined");
#		endif
		print("tested for macro1_a_b");
#	elifdef macro1_b
		print("macro1_b is defined instead of macro1_a");
#	endif
EOF

build/pcpp --only-define=macro1 --only-process=macro1  test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_nested_ifdef case: top ifdef failed'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#undef macro1

#ifdef macro1
	print("macro1 is defined");
#	ifdef macro1_a
		print("macro1_a is defined");
#		ifndef macro1_a_a
			print("macro1_a_a is not defined");
#		endif
		print("tested for macro1_a_a");
#		ifdef macro1_a_b
			print("macro1_a_b is not defined");
#		endif
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
EOF

cat << EOF > expected_output

	print("macro1 not is not defined");
#	ifdef macro2
		print("macro2 not is not defined");
#	endif
EOF

build/pcpp --only-undef=macro1 --only-process=macro1 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_nested_ifdef case: only nested ifdef is resolved'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#define macro1
#define macro1_a_b

#ifdef macro1
	print("macro1 is defined");
#	ifdef macro1_a
		print("macro1_a is defined");
#		ifndef macro1_a_a
			print("macro1_a_a is not defined");
#		endif
		print("tested for macro1_a_a");
#		ifdef macro1_a_b
			print("macro1_a_b is not defined");
#		endif
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
EOF

cat << EOF > expected_output
#define macro1

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
EOF

build/pcpp --only-define=macro1_a_b --process-all test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_nested_ifdef case: else chain never processed'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro1
	print("macro1 is defined");
#	ifdef macro1_a
		print("macro1_a is defined");
#		ifndef macro1_a_a
			print("macro1_a_a is not defined");
#		endif
		print("tested for macro1_a_a");
#		ifdef macro1_a_b
			print("macro1_a_b is not defined");
#		endif
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
EOF

cat << EOF > expected_output
	print("macro1 is defined");
#	ifdef macro1_a
		print("macro1_a is defined");
#		ifndef macro1_a_a
			print("macro1_a_a is not defined");
#		endif
		print("tested for macro1_a_a");
#		ifdef macro1_a_b
			print("macro1_a_b is not defined");
#		endif
		print("tested for macro1_a_b");
#	elifdef macro1_b
		print("macro1_b is defined instead of macro1_a");
#	endif
EOF

build/pcpp -Dmacro1 -Umacro2 --only-process=macro1,macro2 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_nested_ifdef case: elif gets processed'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro1
	print("macro1 is defined");
#	ifdef macro1_a
		print("macro1_a is defined");
#		ifndef macro1_a_a
			print("macro1_a_a is not defined");
#		endif
		print("tested for macro1_a_a");
#		ifdef macro1_a_b
			print("macro1_a_b is not defined");
#		endif
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
EOF

cat << EOF > expected_output
#ifdef macro1
	print("macro1 is defined");
		print("macro1_b is defined instead of macro1_a");
#else
	print("macro1 not is not defined");
#	ifdef macro2
		print("macro2 not is not defined");
#	endif
#endif
EOF

build/pcpp -Dmacro1_b -Umacro1_a --only-process=macro1_a,macro1_b test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

rm -f test_input actual_output expected_output
