#if 0
    source "${TEST_DIR}/lib/crunner" -linspector
#endif

#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/utsname.h>

#include "helpers.h"
#include "procfs.h"

test_start(
        "Verifies that the pfs_kernel_version function retrieves the kernel "
        "version with correct formatting.");

subtest("Comparing output against host machine via _SC_NPROCESSORS_ONLN",
{
    int expected_units = sysconf(_SC_NPROCESSORS_ONLN);
    int units = pfs_cpu_units("/proc");
    test_assert((units == expected_units));
    test_printf("%d", units);
    test_printf("%d", expected_units);
});


subtest("Comparing against fake procfs test set",
{
    char *fakeproc = build_path(getenv("TEST_DIR"), "/inputs/fakeproc");
    int units = pfs_cpu_units(fakeproc);
    free(fakeproc);

    test_assert((units == 64));
    test_printf("%d", units);
});


subtest("Invalid proc location. Return value should be nonzero.",
{
    char invalid[] = "/this/does/not/exist";
    int units = pfs_cpu_units(invalid);

    test_assert(units == 0);
    test_printf("%d", units);
});

test_end
