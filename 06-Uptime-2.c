#if 0
    source "${TEST_DIR}/lib/crunner" -linspector
#endif

#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/utsname.h>

#include "helpers.h"
#include "procfs.h"

test_start("Tests the pfs_uptime and pfs_format_uptime functions.");

subtest("Retrieving the uptime in seconds from dummyproc1",
{
    char *dummyproc = build_path(getenv("TEST_DIR"), "/inputs/dummyproc1");
    double uptime = pfs_uptime(dummyproc);
    free(dummyproc);
    test_assert((uptime == 1489135.87));
    test_printf("%f", uptime);
});

subtest("Retrieving the formatted uptime from dummyproc1",
{
    char *dummyproc = build_path(getenv("TEST_DIR"), "/inputs/dummyproc1");
    double uptime = pfs_uptime(dummyproc);
    free(dummyproc);

    char formatted_uptime[1024];
    int return_value = pfs_format_uptime(uptime, formatted_uptime);
    test_assert(return_value == 0);
    test_printf("%d", return_value);

    if (return_value == 0) {
        test_assert_str(
                formatted_uptime,
                "==",
                "17 days, 5 hours, 38 minutes, 55 seconds",
                1024);
        test_printf("%s", formatted_uptime);
    }
});


subtest("Invalid proc location. Return value should be nonzero.",
{
    char invalid[] = "/this/does/not/exist";
    double time = pfs_uptime(invalid);

    test_assert(time == 0.0 || test == -1.0);
    test_printf("%f", time);
});

test_end
