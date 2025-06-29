#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "util.h"
#include "test.h"
#include "bench.h"


void ins_sort(int* arr, size_t n) {
    // I: sorted arr[0..i]
    for (int i = 1; i < n; i++) {
        int t = arr[i];
        int j = i;
        // I: sorted arr[0..j), sorted arr[j + 1..i]
        for (; j > 0 && arr[j - 1] > t; j--) {
            arr[j] = arr[j-1];
        }
        // post: sorted [0 .. j), arr[0..j) < t <= arr[j + 1..i), sorted [j+1..]
        arr[j] = t;
        // sorted arr [0 .. i)
    }
}

void ins_sort_rec(int* arr, size_t n) {
    if (n <= 1) return;

    ins_sort_rec(arr, n - 1);
    // IH: sorted arr[0..n - 1)

    int j = n - 1, t = arr[j];
    // I: sorted arr[0..j), sorted arr[j + 1..n]
    for (; j > 0 && arr[j - 1] > t; j--) {
        arr[j] = arr[j-1];
    }
    // post: sorted [0 .. j), arr[0..j) < t <= arr[j + 1..i), sorted [j+1..]
    arr[j] = t;
    // sorted arr[0..n)
}


// Generate an array of counts.
// assumes that size(counts) >= maximum(arr)
// assumes that counts has been zeroed
void csort_count(int* arr, size_t n, int* counts) {
    // I: forall v, counts[v] = count of v in arr[0..i)
    // I: sum(counts) = length(arr[0..i))
    for (size_t i = 0; i < n; i++) {
        counts[arr[i]]++;
    }

    // now sum(counts) = length(arr)
    // forall v, counts[v] = count of v in arr
}

// Converts an array of counts into an array of numbers
void csort_expand(int* out, int* counts, size_t n_counts) {
    // let 'arr' refer to the original pointer in "out".
    //  let s0(i) = sum[counts[(.. i -1)]]
    //  let s(i) = sum[counts[(..i)]]
    //  arr[s0(i)]  <= arr[s(i)] 
    for (size_t i = 0; i < n_counts; i++) {
        // I: arr[s..j] == [i, i, ..., j times]
        for (size_t j = 0; j < counts[i]; j++) {
            *out++ = i;
        }
        // each arr[s..counts[i]] = i
        // i - 1 <= i, so the array remains sorted
    }
    // forall i, arr[s0(i)] <= arr[s(i)]
    //      arr[s0(i) .. arr[s(i)) == [i, i, i, ...]
}

#define COUNT_MAX_VAL 1024

void count_sort(int* arr, size_t n) {
    int counts[COUNT_MAX_VAL + 1] = {};
    csort_count(arr, n, counts);
    csort_expand(arr, counts, COUNT_MAX_VAL + 1);
}


char* run_tests(void);
int do_benchmarks(void);

int main() {
    do_unit_tests(run_tests);
    do_benchmarks();
    return 0;
}


char* run_tests() {
    Sorter sorts[] = { ins_sort, ins_sort_rec, count_sort };
    char* sort_names[] = { 
        "insertion sort", "insertion sort (rec)", "counting sort"  };

    for (int i = 0; i < sizeof(sorts) / sizeof(Sorter); i++) {
        printf("testing '%s'\n", sort_names[i]);
        mu_run_sort(t_sort_empty, sorts[i]);
        mu_run_sort(t_sort_single, sorts[i]);
        mu_run_sort(t_sort_pair, sorts[i]);
        mu_run_sort(t_sort_triple, sorts[i]);
        mu_run_sort(t_sort_random, sorts[i]);
    }
    return 0;
}




void bench_shuffle(void* data) {
    Slice* slice = data;
    shuffle(slice->arr, slice->n);
}

void bench_ins_sort(volatile int* sink, void* data) {
    Slice* slice = data;
    ins_sort(slice->arr, slice->n);
    *sink += ((int*)slice->arr)[rand() % slice->n];
}

void bench_ins_sort_rec(volatile int* sink, void* data) {
    Slice* slice = data;
    ins_sort_rec(slice->arr, slice->n);
    *sink += ((int*)slice->arr)[rand() % slice->n];
}

void bench_csort(volatile int* sink, void* data) {
    Slice* slice = data;
    count_sort(slice->arr, slice->n);
    *sink += ((int*)slice->arr)[rand() % slice->n];
}

int do_benchmarks() {
    size_t n_iters = 1000;
    size_t lengths[] = {10, 100, 1000};

    BenchRunner benches[] = {
        bench_ins_sort, 
        bench_ins_sort_rec, 
        bench_csort
    };

    char* bench_names[] = {
        "insertion sort", 
        "insertion sort (recursive)", 
        "counting sort"
    };

    size_t n_benches = sizeof benches / sizeof (BenchRunner);
    size_t n_lengths = sizeof lengths / sizeof(size_t);

    volatile int sink = 0;
    int arr[1000];
    srand(time(NULL));

    for(size_t i = 0; i < 1000; i++) {
        arr[i] = i;
    }

    printf("running %zu benches with %zu iterations each:\n",
        n_benches, n_iters);

    puts("");

    for (size_t i_bench = 0; i_bench < n_benches; i_bench++) {
        printf("benching %s\n", bench_names[i_bench]);
        for (size_t i_length = 0; i_length < n_lengths; i_length++) {
            Slice arr_data = {arr, lengths[i_length]};
            BenchRunner bench = benches[i_bench];
            BenchResult res = 
                do_bench(&sink, n_iters, &arr_data, bench_shuffle, bench);
            printf("length %zu: %lf mean microseconds, stdev: %lf\n", 
                lengths[i_length], res.mean_nanos / 1e3, res.stdev / 1e3);
        }
        puts("");
    }

    printf("\nsink: %xu\n", sink);

    DONT_OPTIMIZE(sink);

    return 0;
}