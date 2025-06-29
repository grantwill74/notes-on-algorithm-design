#include "bench.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


BenchResult do_bench(
    volatile int* sink,
    size_t n_iters,
    void* data,
    BenchPrepper prepare,
    BenchRunner run
) {
    if (n_iters == 0) {
        BenchResult empty = {0.0, 0.0};
        return empty;
    }

    uint64_t sum_nanos = 0;
    long double sum_sq_d = 0;

    for (size_t i = 0; i < n_iters; i++) {
        prepare(data);
        
        struct timespec start, end;
        // feel free to use clock_gettime if on a POSIX platform.
        // it has a monotonic timer, which TIME_UTC isn't.
        // however, it's good enough: just don't change your clock in the
        // middle of a benchmark.
        timespec_get(&start, TIME_UTC);
        run(sink, data);
        timespec_get(&end, TIME_UTC);
        
        uint64_t delta = (end.tv_sec - start.tv_sec) * 1000000000ull +
                         (end.tv_nsec - start.tv_nsec);
        sum_nanos += delta;
        sum_sq_d += (long double)delta * (long double)delta;
    }
    long double sum_d = sum_nanos;
    long double mean =  sum_d / (long double)n_iters;
    long double variance = (sum_sq_d - n_iters * mean * mean) / (n_iters - 1);

    BenchResult result = { 
        mean,
        sqrtl(variance),
    };

    return result;
}

