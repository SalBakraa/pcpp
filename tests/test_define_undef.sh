#!/usr/bin/sh

test_case_name='test_define_undefine case: define without replacement'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#define macro
EOF

cat << EOF > expected_output
EOF

build/pcpp --only-define=macro test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_define_undefine case: define macro with raplacement'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#define macro test
EOF

cat << EOF > expected_output
EOF

build/pcpp -Dmacro --only-define=macro test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_define_undefine case: define with multiline replacement'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#define macro test \
test 2 \
test 3 \
test 4
EOF

cat << EOF > expected_output
EOF

build/pcpp --only-define=macro test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_define_undefine case: redefining defined macro'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#define macro test
#define macro not test
EOF

cat << EOF > expected_output
EOF

build/pcpp -Dmacro --only-define=macro test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_define_undefine case: define function call macro'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#define macro()
#define macro(test) print test
#define macro(test, test, test) print test test test
EOF

cat << EOF > expected_output
EOF

build/pcpp -Dmacro --only-define=macro test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_define_undefine case: redefine macro as function call macro'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#define macro
#define macro(test) print test
EOF

cat << EOF > expected_output
EOF

build/pcpp -Dmacro --only-define=macro test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_define_undefine case: undefine unencountered macro'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#undef macro
EOF

cat << EOF > expected_output
EOF

build/pcpp --only-undef=macro test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_define_undefine case: define then undefine'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#define macro
#undef macro
EOF

cat << EOF > expected_output
EOF

build/pcpp --only=macro test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_define_undefine case: undefine then define'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#undef macro
#define macro
EOF

cat << EOF > expected_output
EOF

build/pcpp --only=macro test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

rm -f test_input actual_output expected_output
