#!/usr/bin/sh

test_case_name='test_comments case: simple comments'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#define/** this is a comment **/macro// Testmacro
#ifdef/* the macro has a comment */macro
print("tested");
#/*endif*/
#endif
EOF

cat << EOF > expected_output
print("tested");
#/*endif*/
EOF

build/pcpp --process-all test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_comments case: multiline comments 1'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#define/**This is a multiline comment
**/macro
#ifdef/* the macro has a comment */macro
print/*
	comment here as
* well
	     */("tested");
#/*endif*/
#endif
EOF

cat << EOF > expected_output
print/*
	comment here as
* well
	     */("tested");
#/*endif*/
EOF

build/pcpp --process-all test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_comments case: multiline comments 2'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#define/**This is a multiline comment
**/macro
#ifdef/* the macro has a comment */macro
print/*
	comment here as
* well
	     */("tested");
#/*endif*/
#endif
EOF

cat << EOF > expected_output
#define/**This is a multiline comment
**/macro
#ifdef/* the macro has a comment */macro
print/*
	comment here as
* well
	     */("tested");
#/*endif*/
#endif
EOF

build/pcpp --include-all test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi


printf '\n=========================================================================\n\n'

rm -f test_input actual_output expected_output
