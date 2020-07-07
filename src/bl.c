#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    // Pinebook Pro is dual Cortex-A72 (big cluster) / quad ARM Cortex-A53 (little cluster)
    // `cat /proc/cpuinfo` says cores 0-3 one type and 4-5 are a different type.
    // Thus, I'm assuming 0-3 are "little" and 4-5 are big.
    // TODO: See if this can be determined automatically, somehow.
    cpu_set_t big;
    cpu_set_t little;

    CPU_ZERO(&big);
    CPU_SET(4, &big);
    CPU_SET(5, &big);

    CPU_ZERO(&little);
    CPU_SET(0, &little);
    CPU_SET(1, &little);
    CPU_SET(2, &little);
    CPU_SET(3, &little);

    if (argc < 3) {
        fputs("usage: bl -b COMMAND [ARGS...]\n", stderr);
        fputs("       bl -l COMMAND [ARGS...]\n", stderr);
        return 1;
    }

    printf("sched_getcpu = %d\n", sched_getcpu());


    cpu_set_t *mask = NULL;

    if (argv[1][0] == '-' && argv[1][1] == 'b') {
        mask = &big;
    } else if (argv[1][0] == '-' && argv[1][1] == 'l') {
        mask = &little;
    } else {
        fputs("error: first argument must be -b or -l. See -h for help.", stderr);
        return 1;
    }

    if (sched_setaffinity(0, sizeof(cpu_set_t), mask) == -1) {
        perror("sched_setaffinity");
        return 1;
    }

    execvp(argv[2], argv + 2);

    return 0;
}
