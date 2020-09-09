source "${TEST_DIR}/lib/funcs.bash"

test_start "Memory Leak Check"

leak_output=$(valgrind --track-fds=yes --track-origins=yes --leak-check=full \
    ./inspector -o -p "${TEST_DIR}/inputs/fakeproc" 2>&1)
echo "${leak_output}"

# Check for open FDs
grep -i '==[0-9]*==\s*Open file descriptor.*tests/inputs' \
    <<< "${leak_output}" && test_end 1

# Make sure there were no blocks definitely lost
grep -i '==[0-9]*==\s*definitely lost' \
    <<< "${leak_output}" && test_end 1

# If none of the conditions were triggered above, the test passes.
test_end 0
