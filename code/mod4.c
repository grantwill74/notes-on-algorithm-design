#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#include "util.h"
#include "bench.h"
#include "test.h"

// given two sorted arrays, merge them together to form a larger sorted array
// requires:    sorted lo,
//              sorted hi,
//              len(merge_buf) >= n_lo + n_hi
void merge(
    const int* lo, const int* hi, 
    size_t n_lo, size_t n_hi, 
    int* restrict merge_buf
) {
    // pre: sorted lo, sorted hi, len(merge_buf) >= n_lo + n_hi
    
    // which lo and hi element to remove next
    int i_lo = 0, i_hi = 0;

    // which merge buf element to write to
    int i_merge = 0;

    // I: sorted merge_buf [0..i_merge], sorted lo, sorted hi,
    //  : merge_buf[0..i_merge) < lo[..), merge_buf[0..i_merge) < hi[..)
    for (;;) {
        // low array is empty, copy rest from hi array
        if (i_lo == n_lo) {
            //n_lo + i_hi = i_merge, 
            // sorted merge_buf[0..n_lo + i_hi], sorted hi,
            // hi[i_hi] > merge_buf[0..i_merge]
            memcpy(merge_buf + i_merge, hi + i_hi, (n_hi - i_hi) * sizeof(int));
            // n_lo + i_hi = i_merge -> sorted merge_buf [0..n_lo + i_hi] -> 
            // sorted merge_buf [0..n_lo + i_hi + (n_hi - i_hi)] ===
            // sorted merge_buf [0..n_lo + n_hi] -> sorted merge_buf
            break;
        }
        // hi array is empty, copy rest from low. proof is similar:
        if (i_hi == n_hi) {
            memcpy(merge_buf + i_merge, lo + i_lo, (n_lo - i_lo) * sizeof(int));
            break;
        }
        
        if (lo[i_lo] <= hi[i_hi]) {
            // take from i_lo, because lo[i_lo] is next smallest unmerged elem:
            // lo[i_lo] < lo[i_lo ..), lo[i_lo] < hi[i_hi..), 
            // merge_buf[.. i_merge) < lo[i_lo]
            merge_buf[i_merge++] = lo[i_lo++];
        } else {
            merge_buf[i_merge++] = hi[i_hi++];
        }
    }

    // post: sorted merge_buf[0 .. n_lo + n_hi)
}

void merge_sort_slow(int* arr, size_t n) {
    if (n <= 1) return;

    // first index of second partition
    // we want both partitions to have the same length if even,
    // and for length(part 1) = length(part 2) + 1 if length is odd
    // goal: show part 2 start index = ((n + 1) >> 1) -> the above property
    // proof:
    //      if n is even, n = 2*k -> n + 1 = 2*k + 1
    //          (2k + 1) >> 1 = floor((2k + 1) / 2) = floor(2k/2 + 1/2)
    //          = floor(2k/2 + 0) = k
    //          part 1 = [0..k), length k - 0 = k
    //          part 2 = [k..n), length n - k
    //          n = 2k -> k = 2k - k = k
    //      if n is odd, n = 2*k+1 -> n + 1 = 2k + 2 = 2(k + 1)
    //          2(k + 1) >> 1 = k + 1
    //      part 1 = [0..k+1), part 2 = [k + 1..n)
    //      len 1 = k + 1, len 2 = n - k - 1
    //          n = 2k + 1 -> len 2 = 2k + 1 - k - 1 = k
    //      -> len 1 = len 2 + 1
    int hi_start = ((n + 1) >> 1); 
    merge_sort_slow(arr, hi_start);
    merge_sort_slow(arr + hi_start, n - hi_start);
    // Induction hypothesis: sorted lo, sorted hi

    // the slow part:
    int* merge_buf = malloc(n * sizeof(int));
    merge(arr, arr + hi_start, hi_start, n - hi_start, merge_buf);
    memcpy(arr, merge_buf, sizeof(int) * n);
    free(merge_buf);
}


void merge_sort(int* arr, size_t n, int* merge_buf) {
    if (n <= 1) return;

    int hi_start = ((n + 1) >> 1); 
    merge_sort(arr, hi_start, merge_buf);
    merge_sort(arr + hi_start, n - hi_start, merge_buf);
    // Induction hypothesis: sorted lo, sorted hi

    // the slow part:
    merge(arr, arr + hi_start, hi_start, n - hi_start, merge_buf);
    memcpy(arr, merge_buf, sizeof(int) * n);
}

int merge_sort_static_buf[0x10000];
void merge_sort_static(int* arr, size_t n) {
    assert("enough elements for static merge sort buffer" && 
        n < sizeof(merge_sort_static_buf) / sizeof(int));
    
    merge_sort(arr, n, merge_sort_static_buf);
}

// post: Return the pivot. let r = arr[pivot]
//       arr[0 .. pivot) < r <= arr[pivot + 1..n) 
size_t partition(int* arr, size_t n) {
    if (n <= 1) return 0;

    // choose the last element to be the pivot
    int r = arr[n - 1];
    
    size_t i_lo = 0, i_hi = n - 2;
    for (;;) {
        // find the first value bigger than the pivot, and the last smaller
        while (i_lo < n && arr[i_lo] < r) i_lo++; // could remove 1st check
        while (i_hi > 0 && arr[i_hi] >= r) i_hi--;

        if (i_lo < i_hi) swap(arr + i_lo, arr + i_hi);
        else break;
    }

    // put the pivot into position
    swap(arr + i_lo, arr + n - 1);

    return i_lo;
}

// find the k + 1th smallest value.
// k = 0 means min
// k = 1 means second smallest, etc.
int quickselect(int* arr, size_t n, size_t k) {
    size_t part = partition(arr, n);

    if (part < k)
        return quickselect(arr + part + 1, n - part - 1, k - part - 1);
    else if (part == k)
        return arr[part];
    else
        return quickselect(arr, part, k);
}

// unit tests //////////////////////////////////////////////////////////////////


char* test_merge_basics(void) {
    int arr[] = {1, 2, 3, 4}; 
    int sorted[8] = {};

    merge(arr, arr + 2, 0, 0, sorted);
    mu_assert("merge handles empty array", sorted[0] == 0 && sorted[1] == 0);

    merge(arr, arr + 2, 2, 0, sorted);
    mu_assert("merge from only lo", sorted[0] == 1 && sorted[1] == 2);

    merge(arr + 2, arr, 0, 2, sorted);
    mu_assert("merge from only hi", sorted[0] == 1 && sorted[1] == 2);

    merge(arr, arr, 4, 4, sorted);
    mu_assert("identical merge", arr_eq_i(sorted, (int[]){1,1,2,2,3,3,4,4}, 8));

    int arr2[] = {-1, 2, 2, 5};
    merge(arr, arr2, 4, 4, sorted);
    mu_assert("scrambled", arr_eq_i(sorted, (int[]){-1,1,2,2,2,3,4,5}, 8));

    int arr3[] = {5, 6, 7, 8};
    merge(arr, arr3, 4, 4, sorted);
    mu_assert("all lo then hi", arr_eq_i(sorted, (int[]){1,2,3,4,5,6,7,8}, 8));

    merge(arr3, arr, 4, 4, sorted);
    mu_assert("all hi then lo", arr_eq_i(sorted, (int[]){1,2,3,4,5,6,7,8}, 8));

    return 0;
}

char* test_partition_basic() {
    int arr1[] = {1, 2};
    int arr2[] = {2, 1};
    int arr3[] = {1, 2, 3};
    int arr4[] = {3, 2, 1};

    mu_assert("partition empty returns 0", partition(arr1, 0) == 0);
    mu_assert("partition size 1 return 0", partition((int[]){1}, 0) == 0);

    partition(arr1, 2);
    partition(arr2, 2);
    partition(arr3, 3);
    partition(arr4, 3);

    mu_assert("partition in order pair", arr_eq_i(arr1, (int[]){1, 2}, 2));
    mu_assert("partition out of order pair", arr_eq_i(arr2, (int[]){1, 2}, 2));

    return 0;
}

char* test_partition_random_one() {
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    shuffle(arr, 10);

    size_t res = partition(arr, 10);

    for (size_t i = 0; i < res; i++) {
        mu_assert("value before partition is less", arr[i] < arr[res]);
    }

    for (size_t i = res + 1; i < 10; i++) {
        mu_assert("partition below all remaining", arr[res] <= arr[i]);
    }

    return 0;
}

char* test_partition_random() {
    for (int i = 0; i < 1000; i++) {
        char* r = test_partition_random_one();
        if (r) return r;
    }

    return 0;
}

char* test_qsel_basic() {
    int arr1[] = {1, 2};
    int arr1c[] = {1, 2};
    int arr2[] = {2, 1};
    int arr2c[] = {2, 1};

    mu_assert("qsel on empty", quickselect(arr1, 0, 0) == 1);
    mu_assert("qsel on single", quickselect(arr1, 1, 0) == 1);
    mu_assert("qsel in order pair", quickselect(arr1, 2, 0) == 1);
    mu_assert("qsel in order pair 2", quickselect(arr1c, 2, 1) == 2);
    mu_assert("qsel out of order pair", quickselect(arr2, 2, 0) == 1);
    mu_assert("qsel out of order pair 2", quickselect(arr2c, 2, 1) == 2);

    return 0;
}

char* test_qsel_random_one() {
    int arr[10] = {0,1,2,3,4,5,6,7,8,9};
    shuffle(arr, 10);

    int k = rand() % 10;
    int res = quickselect(arr, 10, k);

    mu_assert("qsel random k", res == k);


    return 0;
}

char* test_qsel_random() {
    for(int i = 0; i < 1000; i++) {
        char* res = test_qsel_random_one();
        if (res) return res;
    }

    return 0;
}


char* do_tests(void) {
    mu_run(test_merge_basics);

    mu_run_sort(t_sort_empty, merge_sort_slow);
    mu_run_sort(t_sort_single, merge_sort_slow);
    mu_run_sort(t_sort_pair, merge_sort_slow);
    mu_run_sort(t_sort_triple, merge_sort_slow);
    mu_run_sort(t_sort_random, merge_sort_slow);

    mu_run_sort(t_sort_empty, merge_sort_static);
    mu_run_sort(t_sort_single, merge_sort_static);
    mu_run_sort(t_sort_pair, merge_sort_static);
    mu_run_sort(t_sort_triple, merge_sort_static);
    mu_run_sort(t_sort_random, merge_sort_static);

    srand(42);
    mu_run(test_partition_basic);
    mu_run(test_partition_random);
    mu_run(test_qsel_basic);
    mu_run(test_qsel_random);

    return 0;
}

void scramble_array(void* params) {
    Slice* slice = params;

    shuffle(slice->arr, slice->n);
}

void bench_merge_sort_slow(volatile int* sink, void* params) {
    Slice* slice = params;
    merge_sort_slow(slice->arr, slice->n);

    *sink += ((int*)(slice->arr))[rand() % slice->n];
}

void bench_merge_sort(volatile int* sink, void* params) {
    Slice* slice = params;
    merge_sort_static(slice->arr, slice->n);

    *sink += ((int*)(slice->arr))[rand() % slice->n];
}

int main() {
    if (do_unit_tests(do_tests))
        return 1;

    // bench
    int sink = 0;
    srand(time(NULL));
    int arr[1024];
    for (int i = 0; i < 1024; i++)
        arr[i] = i;
    Slice slice = {arr, 1024};

    print_bench(&sink, "merge sort slow", 1000, &slice,
        scramble_array, bench_merge_sort_slow, NULL);
    puts("");

    print_bench(&sink, "merge sort with pre-allocated merge_buf", 1000, &slice,
        scramble_array, bench_merge_sort, NULL);
    puts("");

    printf("sink: %d\n", sink);

    return 0;
}

