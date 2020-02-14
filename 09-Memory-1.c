#if 0
    export FAKE_MEMINFO="${TEST_DIR}/inputs/virtualproc/meminfo"
    source "${TEST_DIR}/lib/crunner" -linspector -D_GNU_SOURCE -ldl -lm
#endif

#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <math.h>

#include "helpers.h"
#include "procfs.h"
#include "virtualproc.h"

test_start("Tests memory usage calculation.");

subtest("Predefined memory usages, simulated by dynamically generating "
        "/proc/meminfo contents for each call to open(2). "
        "See tests/inputs/virtualproc/meminfo for the sequence of file states.",
{

    struct mem_stats expected_mems[] = {
        { 971.50, 1245.51 },
        { 0.0, 1245.51},
        { 844.82, 959.41 },
        { 569.45, 1245.51 },
        { 1245.51, 1245.51 },
        { 138.50, 1245.51 },
        { 1244.51, 1245.51 },
    };

    vp_initialize();
    vp_on();

    for (int i = 0; i < 7; ++i) {
        printf("\n\n> --- Test Iteration: %d ---\n", i);
        struct mem_stats mem = pfs_mem_usage("/proc");
        struct mem_stats expected_mem = expected_mems[i];

        test_assert((fabs(mem.used - expected_mem.used) < 0.01));
        test_printf("%.2f", mem.used);
        test_printf("%.2f", expected_mem.used);

        test_assert((fabs(mem.total - expected_mem.total) < 0.01));
        test_printf("%.2f", mem.total);
        test_printf("%.2f", expected_mem.total);
    }
});

subtest("Using fakeproc dataset",
{
    vp_off();

    char *fakeproc = build_path(getenv("TEST_DIR"), "/inputs/fakeproc");
    struct mem_stats mem = pfs_mem_usage(fakeproc);
    free(fakeproc);

    test_assert_floateq(mem.used, 43.32, 0.01);
    test_printf("%.2f", mem.used);

    test_assert_floateq(mem.total, 62.66, 0.01);
    test_printf("%.2f", mem.total);
});


test_end
