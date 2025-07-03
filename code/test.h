#pragma once
#include <stdlib.h>
#include <stdio.h>

extern size_t tests_run;

// Unit testing here vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// adapted from: https://jera.com/techinfo/jtns/jtn002

// stringize from:
// https://stackoverflow.com/questions/2670816/how-can-i-use-the-compile-time-constant-line-in-a-string
#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)

#define mu_assert(message, test) \
    do { if (!(test)) \
        return __FILE__ ": " STRINGIZE(__LINE__) "; " message ": " #test; }\
         while (0)

// supports non literal string
#define mu_assert_str(message, test) \
    do { if (!(test)) return message; } while (0)

#define mu_run(test)\
    do { char *message = test(); tests_run++; \
         if (message) return message; } while (0)

#define mu_run_sort(test, sort) \
    do { char *message = test(sort); tests_run++; \
         if (message) return message; } while (0)

typedef char* (*Tester)(void); // returns message on error
typedef void (*Sorter)(int* arr, size_t n);

int do_unit_tests(Tester tester);

int in_order(int* arr, size_t n);

char* t_sort_empty(Sorter sort);
char* t_sort_single(Sorter sort);
char* t_sort_pair(Sorter sort);
char* t_sort_triple(Sorter sort);
char* t_sort_random(Sorter sort);
