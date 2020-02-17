#if 0
    export FAKE_STAT="${TEST_DIR}/inputs/virtualproc/stat"
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

test_start("Tests CPU usage calculation.");

subtest("Predefined CPU usages, simulated by dynamically generating /proc/stat "
        " contents for each call to open(2). "
        "See tests/inputs/virtualproc/stat for the sequence of file states. "
        "Remember: error conditions (such as negative values) should yield a "
        "CPU usage of 0.0%",
{
    double expected_percentages[] = {
        0.302853,
        16.838167,
        33.727175,
        33.502538,
        60.378913,
        26.276771,
        2.421525,
        0.331492,
        7.835052,
        52.478283,
        54.259851,
        56.460674,
        4.151752,
        100.000000,
        100.000000,
        0.000000,
        0.302853,
        16.838167,
        33.727175,
        33.502538,
    };

    vp_initialize();
    vp_on();
    struct cpu_stats prev_cpu = { 0 };
    pfs_cpu_usage("/proc", &prev_cpu, &prev_cpu);
    for (int i = 0; i < 20; ++i) {
        struct cpu_stats curr_cpu;
        double cpu_perc = pfs_cpu_usage("/proc", &prev_cpu, &curr_cpu);
        cpu_perc = cpu_perc * 100.0;

        /* We're testing equality with a threshold of 0.0001: */
        test_assert((fabs(cpu_perc - expected_percentages[i]) < 0.0001));
        test_printf("%.2f", cpu_perc);
        test_printf("%.2f", expected_percentages[i]);

        prev_cpu = curr_cpu;
    }
});

subtest("Rapid calls with no sleep() between to pfs_cpu_usage should logically "
        "almost always yield 0% or 100% usage.",
{
    vp_off();
    struct cpu_stats prev_cpu = { 0 };
    pfs_cpu_usage("/proc", &prev_cpu, &prev_cpu);
    for (int i = 0; i < 3; ++i) {
        struct cpu_stats curr_cpu;
        double cpu_perc = pfs_cpu_usage("/proc", &prev_cpu, &curr_cpu);
        test_assert((cpu_perc == 0.0 || cpu_perc == 1.0));
        test_printf("%.2f", cpu_perc);
        prev_cpu = curr_cpu;
    }
});

test_end
