#include <bits/time.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

void swap(int* i, int* j) {
    int t = *i;
    *i = *j;
    *j = t;
}

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

void do_unit_tests(void);

int main() {
    do_unit_tests();
    return 0;
}

// Unit testing here vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// adapted from: https://jera.com/techinfo/jtns/jtn002
#define mu_assert(message, test) \
    do { if (!(test)) return message; } while (0)

#define mu_run_sort(test, sort) \
    do { char *message = test(sort); tests_run++; \
         if (message) return message; } while (0)

typedef void (*sorter)(int* arr, size_t n);

char* t_sort_empty(sorter sort) {
    int arr[3] = { 0xf00d1, 0xf00d2, 0xf00d3 };
    sort(arr + 1, 0);
    mu_assert("empty sort touched elem -1", arr[0] == 0xf00d1);
    mu_assert("empty sort touched elem 0", arr[1] == 0xf00d2);
    mu_assert("empty sort touched elem 1", arr[2] == 0xf00d3);
    return 0;
}

char* t_sort_single(sorter sort) {
    int arr[1] = { 0xabcdef };
    sort(arr, 1);
    mu_assert("bad singleton sort", arr[0]==0xabcdef);
    return 0;
}

char* t_sort_pair(sorter sort) {
    int arr[][2] = {
        {1, 2},
        {2, 1}
    };
    sort(arr[0], 2);
    sort(arr[1], 2);
    mu_assert("bad sort on in-order pair", arr[0][0] < arr[0][1]);
    mu_assert("bad sort on out-of-order pair", arr[1][0] < arr[1][1]);
    return 0;
}

char test_err_buf[1024];
int in_order(int* arr, size_t n) {
    for (int i = 1; i < n; i++)
        if (arr[i] < arr[i - 1])
            return 0;
    return 1;
}
char* t_sort_triple(sorter sort) {
    int arr[][3] = {
        {1, 2, 3},
        {1, 3, 2},
        {2, 1, 3},
        {2, 3, 1},
        {3, 1, 2},
        {3, 2, 1},
    };
    for (int i = 0; i < 6; i++) {
        sort(arr[i], 3);
        sprintf(test_err_buf, "bad sort on triplet %d", i);
        mu_assert(test_err_buf, in_order(arr[i], 3));
    }
    return 0;
}


char* t_sort_random(sorter sort) {
    const int N_RANDOM = 1000;
    srand(0xcafef00d);
    
    for (int rand_i = 0; rand_i < N_RANDOM; rand_i++) {
        int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        // durstenfield shuffle
        for (int i = 0; i < 8; i++) {
            int index = i + rand() % (9 - i);
            swap(arr + i, arr + index);
        }
        sprintf(test_err_buf, 
            "sort failed on random array: %d %d %d %d %d %d %d %d %d",
            arr[0], arr[1], arr[2], arr[3], arr[4],
            arr[5], arr[6], arr[7], arr[8]);
        sort(arr, 9);
        mu_assert(test_err_buf, in_order(arr, 9));
    }
    
    return 0;
}

int tests_run = 0;
char* run_tests() {
    sorter sorts[] = { ins_sort, ins_sort_rec };
    char* sort_names[] = { "insertion sort", "insertion sort (rec)"  };

    for (int i = 0; i < sizeof(sorts) / sizeof(sorter); i++) {
        printf("testing '%s'\n", sort_names[i]);
        mu_run_sort(t_sort_empty, sorts[i]);
        mu_run_sort(t_sort_single, sorts[i]);
        mu_run_sort(t_sort_pair, sorts[i]);
        mu_run_sort(t_sort_triple, sorts[i]);
        mu_run_sort(t_sort_random, sorts[i]);
    }
    return 0;
}

void do_unit_tests() {
    char* msg = run_tests();
    if (msg) printf("test failed with message %s.\n", msg); \
    else printf("ran %d tests sucessfully.\n", tests_run); \
}


// my simple benchmarking
typedef void (*bencher)(void*, int* acc);

typedef struct bench_result_t {
    double mean_nanos;
    double stdev;
} bench_result;

bench_result time_bench(bencher b, void* data, int* acc, size_t n_iters){
    assert (n_iters >= 2);

    uint64_t sum_nanos = 0;
    long double sum_sq_d = 0;
    for (size_t i = 0; i < n_iters; i++) {
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        b(data, acc);
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        uint64_t delta = (end.tv_sec - start.tv_sec) * 1000000000ull +
                         (end.tv_nsec - start.tv_nsec);
        sum_nanos += delta;
        sum_sq_d += (long double)delta * (long double)delta;
    }
    long double sum_d = sum_nanos;
    long double mean =  sum_d / (long double)n_iters;
    long double variance = (sum_sq_d - n_iters * mean * mean) / (n_iters - 1);

    bench_result result = { 
        mean,
        sqrtl(variance),
    };

    return result;
}


int do_benchmarks() {

    return 0;
}