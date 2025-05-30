// this is an example demonstrating practice problem 2.
// it will only compile in clang with -std=c89 because of the implied int return

#include <stdio.h>

a(){return 7;}

int main() {
    printf("lol %d\n", a());
    return 0;
}