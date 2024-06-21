#!/bin/bash    
function run_tests {
    test_sentence
    test_makewords
    test_lowercase
    test_sort
    test_unique
    test_mismatch
}

function compare {
    test "$1" = "$2" && echo "MATCH" || echo "NO MATCH"
}

function test_general {
    test_name=$1
    expected=$2
    actual_c=$3
    actual_bash=$4

    echo ""
    echo "$test_name"
    echo -ne "C\t" && compare "$expected" "$actual_c"
    echo -ne "Bash\t" && compare "$expected" "$actual_bash"
}

function test_sentence {
    local expected="At Bell Laborotories
UNIX systems privide
more timesharing ports
than all other systems
combined"

    test_general "sentence" "$expected" "$(./sentence)" "$(./sentence.sh)"
}

function test_makewords {
    local expected="At
Bell
Laborotories
UNIX
systems
privide
more
timesharing
ports
than
all
other
systems
combined"

    test_general "makewords" "$expected" "$(./sentence | ./makewords)" "$(./sentence.sh | ./makewords.sh)"
}

function test_lowercase {
    local expected="at
bell
laborotories
unix
systems
privide
more
timesharing
ports
than
all
other
systems
combined"

    test_general "lowercase" "$expected" "$(./sentence | ./makewords | ./lowercase)" "$(./sentence.sh | ./makewords.sh | ./lowercase.sh)"
}

function test_sort {
    local expected="all
at
bell
combined
laborotories
more
other
ports
privide
systems
systems
than
timesharing
unix"

    test_general "sort" "$expected" "$(./sentence | ./makewords | ./lowercase | ./sort)" "$(./sentence.sh | ./makewords.sh | ./lowercase.sh | ./sort.sh)"
}

function test_unique {
    local expected="all
at
bell
combined
laborotories
more
other
ports
privide
systems
than
timesharing
unix"

    test_general "unique" "$expected" "$(./sentence | ./makewords | ./lowercase | ./sort | ./unique)" "$(./sentence.sh | ./makewords.sh | ./lowercase.sh | ./sort.sh | ./unique.sh)"
}

function test_mismatch {
    local expected="laborotories
privide
timesharing
unix"

    test_general "mismatch" "$expected" "$(./sentence | ./makewords | ./lowercase | ./sort | ./unique | ./mismatch)" "$(./sentence.sh | ./makewords.sh | ./lowercase.sh | ./sort.sh | ./unique.sh | ./mismatch.sh)"
}

./build.sh
run_tests 