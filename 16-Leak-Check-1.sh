source "${TEST_DIR}/lib/funcs.bash"

test_start "Memory Leak Check"

valgrind ./inspector -o -p "${TEST_DIR}/fakeproc" 2>&1 \
    | grep 'no leaks are possible'

test_end
