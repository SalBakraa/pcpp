#!/usr/bin/sh

test_case_name='test_comments case: parsing backslash newline'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#def\\
ine\\
 macr\\
Test\\
macro

#und\\
ef\\
 mac\\
ro

#define macro do {         \\
	printf(test);          \\
	printf(                \\
		long_long_long_id, \\
		long_long_long_id  \\
	);                     \\
} while(0)
EOF

cat << EOF > expected_output
#def\\
ine\\
 macr\\
Test\\
macro

#und\\
ef\\
 mac\\
ro

#define macro do {         \\
	printf(test);          \\
	printf(                \\
		long_long_long_id, \\
		long_long_long_id  \\
	);                     \\
} while(0)
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

test_case_name='test_comments case: parsing derictives with backslash newline'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test_input
#def\\
ine\\
 macr\\
Test\\
macro

#und\\
ef\\
 mac\\
ro

#define macro do {         \\
	printf(test);          \\
	printf(                \\
		long_long_long_id, \\
		long_long_long_id  \\
	);                     \\
} while(0)
EOF

cat << EOF > expected_output
#def\\
ine\\
 macr\\
Test\\
macro

#und\\
ef\\
 mac\\
ro

EOF

build/pcpp --all test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

rm -f test_input actual_output expected_output
