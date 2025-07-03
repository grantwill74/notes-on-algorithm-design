#include "test.h"
#include "util.h"
#include <stddef.h>
#include <assert.h>
#include <string.h>

//#include "bench.h"

void heap_up(int* heap, size_t i) {
    if (i == 0) return;

    size_t parent = (i - 1) >> 1;
    if (heap[parent] > heap[i]) {
        swap(heap + parent, heap + i);
        heap_up(heap, parent);
    }
}

// heap must have room for n + 1
void heap_ins(int* heap, size_t n, int v) {
    heap[n] = v;
    heap_up(heap, n);
}

void heap_down(int* heap, size_t n, size_t i) {
    size_t l_child = (i << 1) + 1;
    size_t r_child = l_child + 1;
    
    if (l_child >= n) return;

    // 1 child case
    if (r_child >= n) {
        if (heap[i] > heap[l_child])
            swap(heap + i, heap + l_child);
    }
    // 2 child case
    else {
        size_t min_child = heap[l_child] <= heap[r_child] ? l_child : r_child;
        if (heap[i] > heap[min_child]) {
            swap(heap + i, heap + min_child);
            heap_down(heap, n, min_child);
        }
    }
}

// requires heap has at least one element
int heap_pop(int* heap, size_t n) {
    assert(n > 0);

    int top = heap[0];
    heap[0] = heap[n - 1];
    heap_down(heap, n - 1, 0);

    return top;
}

void heapify(int* data, size_t n) {
    for (size_t i = 0; i < n; i++) {
        heap_ins(data, i, data[i]);
    }
}

void heapsort(int* data, size_t n) {
    heapify(data, n);
    int* buf = malloc(n * sizeof(int));
    for (size_t i = 0; i < n; i++) {
        buf[i] = heap_pop(data, n - i);
    }
    memcpy(data, buf, n * sizeof(int));
    free(buf);
}


// unit testing ////////////////////////////////////////////////////////////////
#include <stdbool.h>

static bool is_heap(int* arr, size_t n, size_t i) {
    if (i >= n) return true;

    bool result = true;
    size_t l_child = (i << 1) + 1;
    size_t r_child = l_child + 1;

    if (l_child < n) {
        result = result && arr[i] <= arr[l_child];
        result = result && is_heap(arr, n, l_child);

        if (r_child < n) {
            result = result && arr[i] <= arr[r_child];
            result = result && is_heap(arr, n, r_child);
        }
    }

    return result;
}

static char* test_heap_up() {
    int heap[3] = {};
    size_t size = 0;

    heap_ins(heap, size++, 2);
    mu_assert("heap has 1 val", heap[0] == 2);

    heap_ins(heap, size++, 3);
    mu_assert("inserted below", heap[1] == 3);

    heap_ins(heap, size++, 1);
    mu_assert("inserted first", heap[0] == 1 && heap[1] == 3 && heap[2] == 2);

    return 0;
}

static char* test_heap_pop() {
    int heap[5] = {10, 1, 4, 5, 11};
    size_t n = 5;

    heapify(heap, n);

    mu_assert("pop 1", heap_pop(heap, n--) == 1);
    mu_assert("pop 4", heap_pop(heap, n--) == 4);
    mu_assert("pop 5", heap_pop(heap, n--) == 5);
    mu_assert("pop 10", heap_pop(heap, n--) == 10);
    mu_assert("pop 11", heap_pop(heap, n--) == 11);

    return 0;
}


static char err_buf[128];
static char* test_failed_array() {
    int heap[9] = {1, 9, 8, 4, 6, 7, 2, 5, 3};
    int sorted[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    size_t n = 9;
    
    heapify(heap, 9);

    for (size_t i = 0; i < 9; i++) {
        int popped = heap_pop(heap, n - i);
        sprintf(err_buf, "expected %d, got %d", sorted[i], popped);
        mu_assert_str(err_buf, popped == sorted[i]);
    }

    return 0;
}

static char* test_heapify() {
    int heap[5] = {10, 1, 4, 5, 11};
    int expected[5] = {1, 5, 4, 10, 11};
    
    size_t size = 5;

    heapify(heap, size);
    mu_assert("heapify works", arr_eq_i(heap, expected, size));

    return 0;
}

static char* test_heapify_random() {
    int random[] = {1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,5,4,3,2};
    
    for (size_t i = 0; i < 1000; i++) {
        shuffle(random, 20);
        heapify(random, 20);
        mu_assert("random heapify is a heap", is_heap(random, 20, 0));
    }

    return 0;
}


static char* test_heap() {
    mu_run(test_heap_up);
    mu_run(test_heapify);
    mu_run(test_heap_pop);
    mu_run(test_failed_array);
    mu_run(test_heapify_random);

    mu_run_sort(t_sort_empty, heapsort);
    mu_run_sort(t_sort_single, heapsort);
    mu_run_sort(t_sort_pair, heapsort);
    mu_run_sort(t_sort_triple, heapsort);
    mu_run_sort(t_sort_random, heapsort);

    return 0;
}


int main() {
    return do_unit_tests(test_heap);
}
