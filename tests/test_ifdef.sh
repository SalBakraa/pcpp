#!/usr/bin/sh

test_case_name='test_ifdef case: ifdef_undetermined'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#endif
EOF

cat << EOF > expected_output
#ifdef macro_1
print("macro_1 is defined");
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

test_case_name='test_ifdef case: ifdef_defined'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#endif
EOF

cat << EOF > expected_output
print("macro_1 is defined");
EOF

build/pcpp -Dmacro_1 --only-process=macro_1 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_ifdef case: ifdef_undefined'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#endif
EOF

cat << EOF > expected_output
EOF

build/pcpp -Umacro_1 --only-process=macro_1 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_ifdef case: ifdef_undetermined_else'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#else
print("no macro is defined");
#endif
EOF

cat << EOF > expected_output
#ifdef macro_1
print("macro_1 is defined");
#else
print("no macro is defined");
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

test_case_name='test_ifdef case: ifdef_defined_else'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#else
print("no macro is defined");
#endif
EOF

cat << EOF > expected_output
print("macro_1 is defined");
EOF

build/pcpp -Dmacro_1 --only-process=macro_1 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_ifdef case: ifdef_undefined_else'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#else
print("no macro is defined");
#endif
EOF

cat << EOF > expected_output
print("no macro is defined");
EOF

build/pcpp -Umacro_1 --only-process=macro_1 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_ifdef case: ifdef_undetermined_elif_defined'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#endif
EOF

cat << EOF > expected_output
#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#endif
EOF

build/pcpp -Dmacro_2 --only-process=macro_2 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_ifdef case: ifdef_undetermined_elif_undefined'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#endif
EOF

cat << EOF > expected_output
#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#endif
EOF

build/pcpp -Umacro_2 --only-process=macro_2 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_ifdef case: ifdef_undetermined_elif_defined_else'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#else
print("no macro is defined");
#endif
EOF

cat << EOF > expected_output
#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#else
print("no macro is defined");
#endif
EOF

build/pcpp -Dmacro_2 --only-process=macro_2 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi


printf '\n=========================================================================\n\n'

test_case_name='test_ifdef case: ifdef_undetermined_elif_undefined_else'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#else
print("no macro is defined");
#endif
EOF

cat << EOF > expected_output
#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#else
print("no macro is defined");
#endif
EOF

build/pcpp -Umacro_2 --only-process=macro_2 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_ifdef case: ifdef_defined_elif_defined'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#endif
EOF

cat << EOF > expected_output
print("macro_1 is defined");
EOF

build/pcpp -Dmacro_1 -Dmacro_2 --only-process=macro_1,macro_2 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_ifdef case: ifdef_defined_elif_defined_else'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#else
print("no macro is defined");
#endif
EOF

cat << EOF > expected_output
print("macro_1 is defined");
EOF

build/pcpp -Dmacro_1 -Dmacro_2 --only-process=macro_1,macro_2 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_ifdef case: ifdef_defined_elif_undefined'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#endif
EOF

cat << EOF > expected_output
print("macro_1 is defined");
EOF

build/pcpp -Dmacro_1 -Umacro_2 --only-process=macro_1,macro_2 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_ifdef case: ifdef_defined_elif_undefined_else'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#else
print("no macro is defined");
#endif
EOF

cat << EOF > expected_output
print("macro_1 is defined");
EOF

build/pcpp -Dmacro_1 -Umacro_2 --only-process=macro_1,macro_2 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_ifdef case: ifdef_undefined_elif_defined'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#endif
EOF

cat << EOF > expected_output
print("macro_2 is defined");
EOF

build/pcpp -Umacro_1 -Dmacro_2 --only-process=macro_1,macro_2 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_ifdef case: ifdef_undefined_elif_defined_else'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#else
print("no macro is defined");
#endif
EOF

cat << EOF > expected_output
print("macro_2 is defined");
EOF

build/pcpp -Umacro_1 -Dmacro_2 --only-process=macro_1,macro_2 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_ifdef case: ifdef_undefined_elif_undefined'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#endif
EOF

cat << EOF > expected_output
EOF

build/pcpp -Umacro_1 -Umacro_2 --only-process=macro_1,macro_2 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_ifdef case: ifdef_undefined_elif_undefined_else'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro_1
print("macro_1 is defined");
#elifdef macro_2
print("macro_2 is defined");
#else
print("no macro is defined");
#endif
EOF

cat << EOF > expected_output
print("no macro is defined");
EOF

build/pcpp -Umacro_1 -Umacro_2 --only-process=macro_1,macro_2 test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

rm -f test_input actual_output expected_output
