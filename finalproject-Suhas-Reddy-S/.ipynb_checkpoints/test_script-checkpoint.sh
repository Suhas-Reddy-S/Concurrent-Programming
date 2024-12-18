#!/bin/bash

make

# List of input files
input_files=("10.txt" "500.txt" "2500000.txt" "500000000.txt")

# List of container types
container_types=("sgl_Q" "ms_Q" "fc_sgl_Q" "fc_ms_Q" "sgl_stk" "t_stk" "e_sgl_stk" "e_t_stk" "fc_sgl_stk" "fc_t_stk")

# List of thread counts
threads=(1 2 4 8)

# Counter for passing tests
pass_count=0
total_tests=0

# Iterate over each input file
for input_file in "${input_files[@]}"; do
    # Iterate over each container type
    for container in "${container_types[@]}"; do
        # Iterate over each thread count
        for thread_count in "${threads[@]}"; do
            # Generate output file name
            output_file="./test_cases/$(basename "$input_file" .txt).$container.$thread_count.ans"
            
            # Run the concurrent_containers program
            ./concurrent_containers -i "./test_cases/$input_file" -o "$output_file" -t "$thread_count" --container="$container"

            # Sort both input and output files before comparing
            sorted_input=$(mktemp)
            sorted_output=$(mktemp)

            sort "./test_cases/$input_file" > "$sorted_input"
            sort "$output_file" > "$sorted_output"

            # Compare sorted files
            if cmp -s "$sorted_input" "$sorted_output"; then
                echo "PASS: $input_file vs $output_file"
                pass_count=$((pass_count + 1))
            else
                echo "FAIL: $input_file vs $output_file"
            fi
            total_tests=$((total_tests + 1))

            # Clean up temporary files
            rm "$sorted_input" "$sorted_output"
        done
    done
done

# Display total pass count and total tests
echo "Tests Passed: $pass_count/$total_tests"

make clean
