#!/bin/bash

# Function to compare the output of a command to the expected result
compare_output() {
    local command="$1"
    local expected_output="$2"

    # Run the command with a 5-minute timeout and redirect the output to output.txt
    echo "Running: $command"
    timeout 5m bash -c "$command" > output.txt

    # Check if the command timed out (exit code 124 is returned by timeout)
    if [ $? -eq 124 ]; then
        echo "Test Failed for $command: Timed out after 5 minutes"
        echo "----------------------------------------"
        return
    fi

    # If the output matched, pass the test

    echo ""
    echo "Actual output:"
    cat output.txt
    echo "----------------------------------------"
}

# List of commands and their corresponding expected output files
# feel free to comment some out if you are trying to run specific tests!
# If you do comment one out, comment out the corresponding output file in expected_outputs!
declare -a commands=(
    "./csim 1 1 4 write-allocate write-through lru < traces/misses.trace"
    "./csim 1 1024 128 no-write-allocate write-through lru < traces/misses.trace"
    "./csim 1 1024 128 write-allocate write-through lru < traces/misses.trace"
    "./csim 1 1024 128 write-allocate write-back lru < traces/misses.trace"
    "./csim 8192 1 16 no-write-allocate write-through lru < traces/misses.trace"
    "./csim 8192 1 16 write-allocate write-through lru < traces/misses.trace"
    "./csim 8192 1 16 write-allocate write-back lru < traces/misses.trace"
    "./csim 256 16 64 no-write-allocate write-through lru < traces/misses.trace"
    "./csim 128 32 64 write-allocate write-through lru < traces/misses.trace"
    "./csim 2048 4 16 no-write-allocate write-through lru < traces/misses.trace"
    "./csim 2048 4 16 write-allocate write-through lru < traces/misses.trace"
    "./csim 2048 4 16 write-allocate write-back lru < traces/misses.trace"
    "./csim 1024 1 128 write-allocate write-through lru < traces/misses.trace"
    "./csim 1024 1 128 write-allocate write-back lru < traces/misses.trace"
    "./csim 256 4 128 no-write-allocate write-through lru < traces/misses.trace"
    "./csim 256 4 128 write-allocate write-through lru < traces/misses.trace"
    "./csim 256 4 128 write-allocate write-back lru < traces/misses.trace"
    
    "./csim 1 1 4 write-allocate write-through lru < traces/hits.trace"
    "./csim 1 1024 128 no-write-allocate write-through lru < traces/hits.trace"
    "./csim 1 1024 128 write-allocate write-through lru < traces/hits.trace"
    "./csim 1 1024 128 write-allocate write-back lru < traces/hits.trace"
    "./csim 8192 1 16 no-write-allocate write-through lru < traces/hits.trace"
    "./csim 8192 1 16 write-allocate write-through lru < traces/hits.trace"
    "./csim 8192 1 16 write-allocate write-back lru < traces/hits.trace"
    "./csim 256 16 64 no-write-allocate write-through lru < traces/hits.trace"
    "./csim 128 32 64 write-allocate write-through lru < traces/hits.trace"
    "./csim 2048 4 16 no-write-allocate write-through lru < traces/hits.trace"
    "./csim 2048 4 16 write-allocate write-through lru < traces/hits.trace"
    "./csim 2048 4 16 write-allocate write-back lru < traces/hits.trace"
    "./csim 1024 1 128 write-allocate write-through lru < traces/hits.trace"
    "./csim 1024 1 128 write-allocate write-back lru < traces/hits.trace"
    "./csim 256 4 128 no-write-allocate write-through lru < traces/hits.trace"
    "./csim 256 4 128 write-allocate write-through lru < traces/hits.trace"
    "./csim 256 4 128 write-allocate write-back lru < traces/hits.trace"


    "./csim 1 1 4 write-allocate write-through lru < traces/mixed.trace"
    "./csim 1 256 128 no-write-allocate write-through lru < traces/mixed.trace"
    "./csim 256 1 16 write-allocate write-through lru < traces/mixed.trace"
    "./csim 256 4 128 no-write-allocate write-through lru < traces/mixed.trace"
    "./csim 256 4 128 write-allocate write-through lru < traces/mixed.trace"
    "./csim 256 4 128 write-allocate write-back lru < traces/mixed.trace"
    
)

declare -a expected_outputs=(
    "expected_results/misses/1_1_4_wa_wt_lru.txt"
    "expected_results/misses/1_1024_128_nwa_wt_lru.txt"
    "expected_results/misses/1_1024_128_wa_wt_lru.txt"
    "expected_results/misses/1_1024_128_wa_wb_lru.txt"
    "expected_results/misses/8192_1_16_nwa_wt_lru.txt"
    "expected_results/misses/8192_1_16_wa_wt_lru.txt"
    "expected_results/misses/8192_1_16_wa_wb_lru.txt"
    "expected_results/misses/256_16_64_nwa_wt_lru.txt"
    "expected_results/misses/128_32_64_wa_wt_lru.txt"
    "expected_results/misses/2048_4_16_nwa_wt_lru.txt"
    "expected_results/misses/2048_4_16_wa_wt_lru.txt"
    "expected_results/misses/2048_4_16_wa_wb_lru.txt"
    "expected_results/misses/1024_1_128_wa_wt_lru.txt"
    "expected_results/misses/1024_1_128_wa_wb_lru.txt"
    "expected_results/misses/256_4_128_nwa_wt_lru.txt"
    "expected_results/misses/256_4_128_wa_wt_lru.txt"
    "expected_results/misses/256_4_128_wa_wb_lru.txt"
    
    "expected_results/hits/1_1_4_wa_wt_lru.txt"
    "expected_results/hits/1_1024_128_nwa_wt_lru.txt"
    "expected_results/hits/1_1024_128_wa_wt_lru.txt"
    "expected_results/hits/1_1024_128_wa_wb_lru.txt"
    "expected_results/hits/8192_1_16_nwa_wt_lru.txt"
    "expected_results/hits/8192_1_16_wa_wt_lru.txt"
    "expected_results/hits/8192_1_16_wa_wb_lru.txt"
    "expected_results/hits/256_16_64_nwa_wt_lru.txt"
    "expected_results/hits/128_32_64_wa_wt_lru.txt"
    "expected_results/hits/2048_4_16_nwa_wt_lru.txt"
    "expected_results/hits/2048_4_16_wa_wt_lru.txt"
    "expected_results/hits/2048_4_16_wa_wb_lru.txt"
    "expected_results/hits/1024_1_128_wa_wt_lru.txt"
    "expected_results/hits/1024_1_128_wa_wb_lru.txt"
    "expected_results/hits/256_4_128_nwa_wt_lru.txt"
    "expected_results/hits/256_4_128_wa_wt_lru.txt"
    "expected_results/hits/256_4_128_wa_wb_lru.txt"

    "expected_results/mixed/1_1_4_wa_wt_lru.txt"
    "expected_results/mixed/1_256_128_nwa_wt_lru.txt"
    "expected_results/mixed/256_1_16_wa_wt_lru.txt"
    "expected_results/mixed/256_4_128_nwa_wt_lru.txt"
    "expected_results/mixed/256_4_128_wa_wt_lru.txt"
    "expected_results/mixed/256_4_128_wa_wb_lru.txt"

)

for i in "${!commands[@]}"; do
    compare_output "${commands[$i]}" "${expected_outputs[$i]}"
done
