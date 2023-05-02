#!/usr/bin/sh

test_case_name='test_include case: no include'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test.c
#define ZERO 0

int main() {
	return ZERO;
}
EOF

cat << EOF > test_input
#include <stdio.h>
#include "test.c"
EOF

cat << EOF > expected_output
#include <stdio.h>
#include "test.c"
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

test_case_name='test_include case: include only 1 file'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test.c
#define ZERO 0

int main() {
	return ZERO;
}
EOF

cat << EOF > other_test.c
#define TWO_FIFTY 250

int not_main() {
	return TWO_FIFTY ;
}
EOF

cat << EOF > test_input
#include <stdio.h>
#include "test.c"
#include "other_test.c"
EOF

cat << EOF > expected_output
#include <stdio.h>
#define ZERO 0

int main() {
	return ZERO;
}
#include "other_test.c"
EOF

build/pcpp --only-include=test.c test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_include case: include 2 files'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test.c
#define ZERO 0

int main() {
	return ZERO;
}
EOF

cat << EOF > other_test.c
#define TWO_FIFTY 250

int not_main() {
	return TWO_FIFTY ;
}
EOF

cat << EOF > test_input
#include <stdio.h>
#include "test.c"
#include "other_test.c"
EOF

cat << EOF > expected_output
#include <stdio.h>
#define ZERO 0

int main() {
	return ZERO;
}
#define TWO_FIFTY 250

int not_main() {
	return TWO_FIFTY ;
}
EOF

build/pcpp --only-include=test.c,other_test.c test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_include case: included code is processed'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test.c
#define ZERO 0

int main() {
	return ZERO;
}
EOF

cat << EOF > other_test.c
#define TWO_FIFTY 250

int not_main() {
	return TWO_FIFTY ;
}
EOF

cat << EOF > test_input
#include <stdio.h>
#include "test.c"
#include "other_test.c"
EOF

cat << EOF > expected_output
#include <stdio.h>

int main() {
	return ZERO;
}

int not_main() {
	return TWO_FIFTY ;
}
EOF

build/pcpp --only-process=ZERO,TWO_FIFTY --only-include=test.c,other_test.c test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

test_case_name='test_include case: include is surronded by line deritives'
printf 'Run %s\n' "$test_case_name"

cat << EOF > test.c
#define ZERO 0

int main() {
	return ZERO;
}
EOF

cat << EOF > other_test.c
#define TWO_FIFTY 250

int not_main() {
	return TWO_FIFTY ;
}
EOF

cat << EOF > test_input
#include <stdio.h>
#include "test.c"
#include "other_test.c"
EOF

cat << EOF > expected_output
#include <stdio.h>
#line 0 "test.c"
#define ZERO 0

int main() {
	return ZERO;
}
#line 1 "test_input"
#line 0 "other_test.c"
#define TWO_FIFTY 250

int not_main() {
	return TWO_FIFTY ;
}
#line 2 "test_input"
EOF

build/pcpp --line-around-include --include-all test_input > actual_output

if diff --color=always -y actual_output expected_output > /dev/null ; then
	printf "\033[0;32m%s PASSED\033[0m\n" "$test_case_name"
else
	printf "\033[0;31m%s FAILED\033[0m\n" "$test_case_name"
	printf 'diff --color=always -y actual_output expected_output\n'
	diff --color=always -y actual_output expected_output
fi

printf '\n=========================================================================\n\n'

rm -f test.c other_test.c test_input actual_output expected_output
