#pragma once
#include <stdlib.h>
#include <stdio.h>

extern size_t tests_run;

// Unit testing here vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// adapted from: https://jera.com/techinfo/jtns/jtn002
#define mu_assert(message, test) \
    do { if (!(test)) return message; } while (0)

#define mu_run_sort(test, sort) \
    do { char *message = test(sort); tests_run++; \
         if (message) return message; } while (0)

typedef char* (*Tester)(); // returns message on error
typedef void (*Sorter)(int* arr, size_t n);

int do_unit_tests(Tester tester);

int in_order(int* arr, size_t n);

char* t_sort_empty(Sorter sort);
char* t_sort_single(Sorter sort);
char* t_sort_pair(Sorter sort);
char* t_sort_triple(Sorter sort);
char* t_sort_random(Sorter sort);
