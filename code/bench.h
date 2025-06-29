#pragma once 

#include <stddef.h>

// my simple benchmarking suite

#define DONT_OPTIMIZE(x) \
    __asm__ __volatile__("" : "+m"(x) : : "memory")
// The optimizer is a wily creature. Be sure to look at
// the assembly to make sure the function is still there.

struct timespec;

// prepares the data before each run (e.g., to shuffle it)
typedef void (*BenchPrepper)(void* params);

// actually does the thing we're benchmarking
typedef void (*BenchRunner)(volatile int* sink, void* data);

typedef struct bench_result_t {
    double mean_nanos;
    double stdev;
} BenchResult;

BenchResult do_bench(
    volatile int* sink,
    size_t n_iters,
    void* data,
    BenchPrepper prepare,
    BenchRunner run
);
