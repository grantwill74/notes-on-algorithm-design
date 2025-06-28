#include "test.h"
#include "util.h"

#include <stddef.h>

size_t tests_run;

int do_unit_tests(Tester run_tests) {
    char* msg = run_tests();
    if (msg) printf("test failed with message %s.\n", msg);
    else printf("ran %zu tests sucessfully.\n", tests_run);

    return msg != NULL;
}

char test_err_buf[1024];

char* t_sort_empty(Sorter sort) {
    int arr[3] = { 0xf00d1, 0xf00d2, 0xf00d3 };
    sort(arr + 1, 0);
    mu_assert("empty sort touched elem -1", arr[0] == 0xf00d1);
    mu_assert("empty sort touched elem 0", arr[1] == 0xf00d2);
    mu_assert("empty sort touched elem 1", arr[2] == 0xf00d3);
    return 0;
}

char* t_sort_single(Sorter sort) {
    int arr[1] = { 999 };
    sort(arr, 1);
    mu_assert("bad singleton sort", arr[0]==999);
    return 0;
}

char* t_sort_pair(Sorter sort) {
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
char* t_sort_triple(Sorter sort) {
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


char* t_sort_random(Sorter sort) {
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


