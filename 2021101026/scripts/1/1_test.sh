#!/bin/bash

mpic++ -o 1 ../../1/1.cpp

normalize_spaces() {
    sed -e 's/[[:space:]]\+/ /g' -e 's/[[:space:]]*$//' -e '/^$/d' "$1" > "$2"
}

mkdir -p results

total_marks=0

num_test_cases=$(ls testcases/*.in | wc -l)

for i in $(seq 1 $num_test_cases); do
    test_file="testcases/${i}.in"
    expected_output="testcases/${i}.out"

    all_passed=true

    for np in {1..12}; do
        mpiexec -np $np --use-hwthread-cpus --oversubscribe ./1 < $test_file > results/1_${np}_${i}.txt
        normalize_spaces results/1_${np}_${i}.txt results/1_${np}_${i}_normalized.txt
        normalize_spaces $expected_output results/expected_${i}_normalized.txt

        if ! diff -q results/1_${np}_${i}_normalized.txt results/expected_${i}_normalized.txt > /dev/null; then
            all_passed=false
            break
        fi
    done


    if [ "$all_passed" = true ]; then
        printf "Test case $i: \e[32mPASSED\e[0m\n"
        marks=$(grep "^${i} " marks.txt | cut -d ' ' -f 2)
        total_marks=$((total_marks + marks))

    else
        printf "Test case $i: \e[31mFAILED\e[0m\n"
    fi
done

echo -e "Final Score: $total_marks/100"

rm -rf 1 results/