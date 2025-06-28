#include <assert.h>

#include "util.h"
#include "test.h"

// pre: arr has at least n + 1 bytes, arr[0..n) is sorted
void fast_insert(int* arr, size_t n, int val) {
    // find where to insert 'val'
    size_t i = 0;
    // invariant: arr[0..i) < val, arr[1..n) is sorted
    for (; i < n && arr[i] < val; i++)
        ;
    assert(i == n || val <= arr[i]);
    assert(i == 0 || arr[i - 1] < val);

    // push everything from i up to make a hole at i
    // I: j == n or arr[i .. n] is sorted
    for (size_t j = n; i < j; j--)
        arr[j] = arr[j - 1]; // arr_after [j..n] = arr_before [j-1..n)

    assert(i == n || arr[i] == arr[i + 1]); // we've made a duplicate
    // I: arr[0..i] is sorted, arr[i] == arr[i+1], arr[i+1..n+1] is sorted
    //    everything in arr[0..i) < val; val <= everything in arr[i..n]

    // insert val into array, over the duplicate
    arr[i] = val;

    // postcondition: arr[0..= n + 1] is sorted
}

void ins_sort(int* arr, size_t n) {
    for (size_t i = 1; i < n; i++) 
        fast_insert(arr, i, arr[i]);
}

/* Proof of correctness: by induction on n
    case n == 0:
        the loop doesn't run, and arr is unchanged.
        empty arrays are sorted by definition

    case P(n) -> P(n + 1):
        Inductive Hypothesis: arr[..n) is sorted 
        by correctness theorem of fast_insert, arr[..n+1) is sorted
*/

// find the index of the smallest int in an array with length > 0
int arg_min(int* arr, size_t n) {
    assert(n > 0);

    int the_min = arr[0];
    int min_i = 0;

    // invariant: the_min = min(arr[0..i))
    //            min_i = arg_min(arr[0..i))
    for (int i = 1; i < n; i++)
        if (arr[i] < the_min) {
            // update the min and min_i
            the_min = arr[i];
            min_i = i;
        }
        // the_min = min(arr[0..i), arr[i])
        // min_i = arg_min(arr[0..i+1)), 
    
    // min_i = arg_min(arr[0..i+1))

    return min_i;
}

// selection sort on array of size n
void sel_sort(int* arr, size_t n) {
    // base case: arr[0..0) is sorted
    if (n == 0) return;

    // arr[0..i) is sorted
    for (int i = 0; i < n; i++) {
        // add i because the arg_min will assume (arr + i) is index 0
        int min_i = i + arg_min(arr + i, n - i);
        // min_i = i + arg_min(arr[i..]) 
        swap(arr + min_i, arr + i);
        // everything in arr[0..i) <= arr[i] ->
        // arr[0..i + 1) is sorted
    }
    
    // arr[0..n) is sorted
}

void old_ins_sort(int* arr, size_t n) { 
    for (size_t i = 1; i < n; i++) 
        for (size_t j = i; 0 < j && arr[j] < arr[j - 1]; j--)
            swap(arr + j - 1, arr + j);
}

char* run_tests() {
    mu_run_sort(t_sort_empty, ins_sort);
    mu_run_sort(t_sort_single, ins_sort);
    mu_run_sort(t_sort_pair, ins_sort);
    mu_run_sort(t_sort_triple, ins_sort);
    mu_run_sort(t_sort_random, ins_sort);

    return 0;
}

int main() {
    do_unit_tests(run_tests);
}

