#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void swap(int* i, int* j) {
    int t = *i;
    *i = *j;
    *j = t;
}

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

void do_unit_tests(void);

int main() {
    do_unit_tests();
    return 0;
}

// Unit testing here vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// adapted from: https://jera.com/techinfo/jtns/jtn002
#define mu_assert(message, test) \
    do { if (!(test)) return message; } while (0)

#define mu_run(test, sort) \
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
    sorter sorts[] = { ins_sort, sel_sort };
    char* sort_names[] = { "insertion sort", "selection sort" };

    for (int i = 0; i < sizeof(sorts) / sizeof(sorter); i++) {
        printf("testing '%s'\n", sort_names[i]);
        mu_run(t_sort_empty, sorts[i]);
        mu_run(t_sort_single, sorts[i]);
        mu_run(t_sort_pair, sorts[i]);
        mu_run(t_sort_triple, sorts[i]);
        mu_run(t_sort_random, sorts[i]);
    }
    return 0;
}

void do_unit_tests() {
    char* msg = run_tests();
    if (msg) printf("test failed with message %s.\n", msg); \
    else printf("ran %d tests sucessfully.\n", tests_run); \
}

