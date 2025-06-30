#pragma once

#include <stddef.h>
#include <stdbool.h>

inline void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void shuffle(int* arr, size_t n);

typedef struct slice_t {
    void* arr;
    size_t n;
} Slice;

bool arr_eq_i(const int* a, const int* b, size_t n);
