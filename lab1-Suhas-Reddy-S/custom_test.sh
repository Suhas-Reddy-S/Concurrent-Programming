#!/bin/bash

# Create a directory to store the test cases and results
mkdir -p test_cases
mkdir -p results

# Generate 14 random test cases
for i in $(seq 1 14); do
    # Generate a random number of integers (between 100 and 1000) for each test case
    num_integers=$(shuf -i 100-1000 -n 1)
    # Generate random integers and save to a test case file
    shuf -i 0-2147483643 -n $num_integers > "test_cases/case${i}.txt"
    
    # Sort using the standard sort command
    sort -n "test_cases/case${i}.txt" > "results/soln${i}.txt"

    # Run the mysort program using forkjoin
    ./mysort -i "test_cases/case${i}.txt" -o "results/your_forkjoin${i}.txt" --alg=forkjoin
    
    # Compare results for forkjoin
    cmp --silent "results/your_forkjoin${i}.txt" "results/soln${i}.txt" && echo "Pass (forkjoin) for case ${i}" || echo "Fail (forkjoin) for case ${i}"

    # Run the mysort program using lkbucket
    ./mysort -i "test_cases/case${i}.txt" -o "results/your_lkbucket${i}.txt" --alg=lkbucket
    
    # Compare results for lkbucket
    cmp --silent "results/your_lkbucket${i}.txt" "results/soln${i}.txt" && echo "Pass (lkbucket) for case ${i}" || echo "Fail (lkbucket) for case ${i}"
done

echo "All test cases completed."
