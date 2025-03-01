#!/bin/bash

# Test log file
LOG_FILE="test_results.log"
echo "Running Tests - $(date)" > $LOG_FILE

# Function to run a test case
run_test() {
    local executable=$1
    local expected_output=$2
    echo -e "\nRunning $executable..."
    ./$executable > output.log 2>&1

    if grep -q "$expected_output" output.log; then
        echo "[PASS] $executable" | tee -a $LOG_FILE
    else
        echo "[FAIL] $executable - Check output.log" | tee -a $LOG_FILE
    fi
}

echo "Compiling Programs..."

# Compile multi-threading programs
gcc -pthread -o threads_mutex src/threads_mutex.c
gcc -pthread -o threads_deadlock src/threads_deadlock.c
gcc -pthread -o threads_deadlock_resolved src/threads_deadlock_resolved.c

# Compile IPC programs
gcc -o ipc_pipe src/ipc_pipe.c
gcc -o ipc_bi_directional src/ipc_bi_directional.c

echo "Running Tests..."

# Test threading with mutex
run_test "threads_mutex" "Final balance"

# Test deadlock scenario (should freeze, manual check needed)
echo -e "\nRunning threads_deadlock (Expected: Deadlock - Manual Check Required)"
./threads_deadlock &
sleep 3
pkill -9 threads_deadlock
echo "[INFO] threads_deadlock manually terminated (Deadlock expected)" | tee -a $LOG_FILE

# Test resolved deadlock
run_test "threads_deadlock_fix" "Final account balances"

# Test IPC (Parent to Child)
run_test "ipc_pipe" "Child received"

# Test bi-directional IPC (Parent ↔ Child)
run_test "ipc_bi_directional" "Parent received"

echo "Testing Complete. Check test_results.log for details."
