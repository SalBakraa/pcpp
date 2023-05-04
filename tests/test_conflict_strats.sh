#!/usr/bin/sh

test_case_name='test_conflict_strats case: ignore source override'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#undef macro
#ifdef macro
print("Ignored source");
#endif
EOF

cat << EOF > expected_output
print("Ignored source");
EOF

build/pcpp --all -Dmacro --conflict=user test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_conflict_strats case: ignore user defintions'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#undef macro
#ifndef macro
print("Ignored command line");
#endif
EOF

cat << EOF > expected_output
print("Ignored command line");
EOF

build/pcpp --all -Dmacro --conflict=source test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_conflict_strats case: ignore both definitions'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#undef macro
#ifndef macro
print("Ignored both");
#endif
EOF

cat << EOF > expected_output
#ifndef macro
print("Ignored both");
#endif
EOF

build/pcpp --all -Dmacro --conflict=ignore test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

rm -f test_input actual_output expected_output
