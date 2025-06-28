#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

// given two sorted arrays, merge them together to form a larger sorted array
// requires:    sorted lo,
//              sorted hi,
//              len(merge_buf) >= n_lo + n_hi
void merge(int* lo, int* hi, size_t n_lo, size_t n_hi, int* merge_buf) {
    // pre: sorted lo, sorted hi, len(merge_buf) >= n_lo + n_hi
    
    // which lo and hi element to remove next
    int i_lo = 0, i_hi = 0;

    // which merge buf element to write to
    int i_merge = 0;

    // I: sorted merge_buf [0..i_merge]
    for (;;) {
        // low array is empty, copy rest from hi array
        if (i_lo == n_lo) {
            //n_lo + i_hi = i_merge, 
            // sorted merge_buf[0..n_lo + i_hi], sorted hi,
            // hi[i_hi] > merge_buf[0..i_merge]
            memcpy(merge_buf + i_merge , hi + i_hi, n_hi - i_hi);
            // n_lo + i_hi = i_merge -> sorted merge_buf [0..n_lo + i_hi] -> 
            // sorted merge_buf [0..n_lo + i_hi + (n_hi - i_hi)] ===
            // sorted merge_buf [0..n_lo + n_hi] -> sorted merge_buf
            break;
        }
        // hi array is empty, copy rest from low. proof is similar:
        if (i_hi == n_hi) {
            memcpy(merge_buf + i_merge, lo + i_lo, n_lo - i_lo);
            break;
        }

        if (lo[i_lo] <= hi[i_hi]) {
            
        }
    }

    // post: sorted merge_buf[0 .. n_lo + n_hi)
}

int* merge_sort_slow(int* arr, size_t n) {
    if (n == 0) return NULL;

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
    int* lo = merge_sort_slow(arr, hi_start);
    int* hi = merge_sort_slow(arr + hi_start, n - hi_start);
    // Induction hypothesis: sorted lo, sorted hi

    // pointer to next value to merge
    int *p_lo = lo, *p_hi = hi;
    // pointer to first invalid value
    int* p_end = arr + n;
    
    int* merge_buf = malloc(n * sizeof(int));
    int* p_merge = merge_buf;
    // goal: sorted lo -> sorted hi -> sorted merge_buf
    // I: sorted merge_buf [0..i)
    for (;;) {
        // nothing left to merge from lo or hi
        if (p_lo == hi) 
            { memcpy(p_merge, p_hi, (p_end - p_hi) * sizeof(int)); break; }
            // sorted merge_buf [0..len(lo) + (p_hi - hi)) -> IH ->
            // sorted merge_buf [0..len(lo) + (p_hi - hi) + (p_end - p_hi)]
        if (p_hi == p_end) 
            { memcpy(p_merge, p_lo, (hi - p_lo) * sizeof(int)); break; }
        
    }

    return merge_buf;
}

void merge_sort_zone(int* arr, size_t n, int* zone) {

}