#if 0
    source "${TEST_DIR}/lib/crunner" -linspector
#endif

#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/utsname.h>

#include "helpers.h"
#include "procfs.h"

test_start("Verifies CPU model retrieval functionality.");

subtest("Comparing against fake procfs test set",
{
    char *fakeproc = build_path(getenv("TEST_DIR"), "/inputs/fakeproc");
    char model[128] = { 88 };
    int return_value = pfs_cpu_model(fakeproc, model, 64);
    free(fakeproc);

    test_assert_str(model, "==", "AMD EPYC 7281 16-Core Processor",
            128);
    test_printf("'%s'", model);

    test_assert(return_value == 0);
    test_printf("%d", return_value);
});


subtest("Invalid proc location. Return value should be nonzero.",
{
    char invalid[] = "/this/does/not/exist";
    char model[128] = { 88 };
    int return_value = pfs_cpu_model(invalid, model, 128);

    test_assert(return_value != 0);
    test_printf("%d", return_value);
});

test_end
