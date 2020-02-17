source "${TEST_DIR}/lib/funcs.bash"

test_start "Empty/partially-populated procfs" \
"Ensures the inspector can handle malformed, damaged, or missing proc entries without crashing"

./inspector -op ./tests/inputs/dummyproc1

./inspector -op ./tests/inputs/dummyproc2

test_end

