#!/bin/bash

if ls ../../3/*.py &>/dev/null; then
    executable="python3 ../../3/*.py"
elif ls ../../3/*.cpp &>/dev/null; then
    mpic++ -std=c++20 -o 3 ../../3/*.cpp
    executable="./3"
elif ls ../../3/*.c &>/dev/null; then
    mpicc -o 3 ../../3/*.c
    executable="./3"
else
    echo "No Python, C, or C++ file found in ../../3/"
    exit 1
fi

mkdir -p results

total_marks=0
num_test_cases=$(ls testcases/*.in | wc -l)

for i in $(seq 1 $num_test_cases); do
    test_file="testcases/${i}.in"
    all_passed=true

    for np in {4..12}; do
        output_file="results/3_${np}_${i}.txt"
        {
            while IFS= read -r line || [[ -n "$line" ]]; do
                echo "$line"
                if [[ "$line" == "failover "* || "$line" == "recover "* ]]; then
                    sleep 3
                fi
            done < "$test_file"
        } | mpiexec -np $np --use-hwthread-cpus --oversubscribe $executable > "$output_file"

        # Replace with interactive_tester executable to test without load balancing
        ./lb_interactive_tester "$test_file" "$output_file" $np
        
        if [ $? -ne 0 ]; then
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

rm -rf 3 results/
