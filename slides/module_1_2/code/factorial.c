#include <stdio.h>

unsigned factorial(unsigned n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int main() {

    for (unsigned i = 0; i < 14; i++) {
        printf("%d\n", factorial(i));
    }

    return 0;
}