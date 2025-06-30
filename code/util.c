#include "util.h"

#include <stdlib.h>

void shuffle(int* arr, size_t n) {
    // durstenfield shuffle
    for (size_t i = 0; i < n - 1; i++) {
        int index = i + rand() % (n - i);
        swap(arr + i, arr + index);
    }
}

bool arr_eq_i(const int* a, const int* b, size_t n) {
    for(size_t i = 0; i < n; i++) {
        if (*a++ != *b++) return false;
    }

    return true;
}
