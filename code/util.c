#include "util.h"

#include <stdlib.h>

void shuffle(int* arr, size_t n) {
    // durstenfield shuffle
    for (size_t i = 0; i < n - 1; i++) {
        int index = i + rand() % (n - i);
        swap(arr + i, arr + index);
    }
}
