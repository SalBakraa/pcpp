#!/usr/bin/sh

test_case_name='test_implicit_undef case: implicit undef with ifdef'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro
print("macro is defined");
#endif
EOF

cat << EOF > expected_output
EOF

build/pcpp --process-all --implicitly-undef test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_implicit_undef case: implicit undef with ifndef'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifndef macro
print("macro is defined");
#endif
EOF

cat << EOF > expected_output
print("macro is defined");
EOF

build/pcpp --process-all --implicitly-undef test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_implicit_undef case: implicit undef with ifdef and else'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro
print("macro is defined");
#else
print("no macro is defined");
#endif
EOF

cat << EOF > expected_output
print("no macro is defined");
EOF

build/pcpp --process-all --implicitly-undef test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_implicit_undef case: implicit undef with ifndef and else'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifndef macro
print("macro is defined");
#else
print("no macro is defined");
#endif
EOF

cat << EOF > expected_output
print("macro is defined");
EOF

build/pcpp --process-all --implicitly-undef test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_implicit_undef case: implicit undef with ifdef and elifdef'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro
print("macro is defined");
#elifdef macro2
print("macro2 is defined");
#endif
EOF

cat << EOF > expected_output
EOF

build/pcpp --process-all --implicitly-undef test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_implicit_undef case: implicit undef with ifdef and elifndef'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro
print("macro is defined");
#elifndef macro2
print("macro2 is defined");
#endif
EOF

cat << EOF > expected_output
print("macro2 is defined");
EOF

build/pcpp --process-all --implicitly-undef test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_implicit_undef case: implicit undef with ifndef and elifdef'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifndef macro
print("macro is defined");
#elifdef macro2
print("macro2 is defined");
#endif
EOF

cat << EOF > expected_output
print("macro is defined");
EOF

build/pcpp --process-all --implicitly-undef test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_implicit_undef case: implicit undef with ifndef and elifndef'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifndef macro
print("macro is defined");
#elifndef macro2
print("macro2 is defined");
#endif
EOF

cat << EOF > expected_output
print("macro is defined");
EOF

build/pcpp --process-all --implicitly-undef test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_implicit_undef case: implicit undef with ifdef, elifdef, else'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro
print("macro is defined");
#elifdef macro2
print("macro2 is defined");
#else
print("no macro is defined");
#endif
EOF

cat << EOF > expected_output
print("no macro is defined");
EOF

build/pcpp --process-all --implicitly-undef test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_implicit_undef case: implicit undef with ifdef, elifndef, else'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifdef macro
print("macro is defined");
#elifndef macro2
print("macro2 is defined");
#else
print("no macro is defined");
#endif
EOF

cat << EOF > expected_output
print("macro2 is defined");
EOF

build/pcpp --process-all --implicitly-undef test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_implicit_undef case: implicit undef with ifndef, elifdef, else'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifndef macro
print("macro is defined");
#elifdef macro2
print("macro2 is defined");
#else
print("no macro is defined");
#endif
EOF

cat << EOF > expected_output
print("macro is defined");
EOF

build/pcpp --process-all --implicitly-undef test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_implicit_undef case: implicit undef with ifndef, elifndef, else'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#ifndef macro
print("macro is defined");
#elifndef macro2
print("macro2 is defined");
#else
print("no macro is defined");
#endif
EOF

cat << EOF > expected_output
print("macro is defined");
EOF

build/pcpp --process-all --implicitly-undef test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

rm -f test_input actual_output expected_output
