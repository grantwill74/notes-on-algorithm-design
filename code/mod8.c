#include <limits.h>
#include <stdio.h>
#include <stdint.h>

uint64_t fibo(uint64_t n) {
    if (n <= 1) return n;

    uint64_t a = 1, b = 1;

    for (uint64_t i = 2; i < n; i++) {
        uint64_t c = a + b;
        a = b; b = c;
    }

    return b;
}

typedef struct mat2_t {
    uint64_t m[4];
} Mat2;

Mat2 mat_mul(Mat2 a, Mat2 b) {
    return (Mat2){
        a.m[0] * b.m[0] + a.m[1] * b.m[2],
        a.m[0] * b.m[1] + a.m[1] * b.m[3],
        a.m[2] * b.m[0] + a.m[3] * b.m[2],
        a.m[2] * b.m[1] + a.m[3] * b.m[3],
    };
}

Mat2 mat_ident() {
    return (Mat2){
        1, 0,
        0, 1,
    };
}

// without big-ints, we can't get super big with e.
// *with* big-ints, we can, but then log factors appear in big-O due to 
// big-int addition
Mat2 mat_exp(Mat2 a, uint8_t e) {
    if (e == 0) return mat_ident();
    if (e == 1) return a;

    if ((e & 1) == 0) {
        Mat2 b = mat_exp(a, e >> 1);
        return mat_mul(b, b);
    } else {
        return mat_mul(a, mat_exp(a, e - 1));
    }
}

uint64_t mat_fibo(uint8_t n) {
    if (n <= 1) return n;
    Mat2 tx = mat_exp((Mat2){{1, 1, 1, 0}}, n - 1);

    return tx.m[0];
}


const int N_DENOMS = 4;
int denoms[] = {25, 10, 5, 1};

// takes a list of denominations, assums it is sorted biggest first.
int greedy_change(int cents) {
    int n_coins = 0;
    
    for (int i = 0; i < N_DENOMS; i++) {
        n_coins += cents / denoms[i];
        cents %= denoms[i];
    }

    return n_coins;
}

// hardcoded for 4 denominations: 25, 10, 5, 1
int greedy_fixed(int cents) {
    int n_coins = 0;

    n_coins += cents / 25;
    cents %= 25;
    n_coins += cents / 10;
    cents %= 10;
    n_coins += cents / 5;
    cents %= 5;
    n_coins += cents;

    return n_coins;
}

int dp[0x10000] = {};

int dyn_change(int cents) {
    if (cents <= 0) return 0;
    if (dp[cents]) return dp[cents];

    int sol = INT_MAX;
    for (int i = 0; i < N_DENOMS; i++) {
        if (denoms[i] > cents) continue;
        int sub = dyn_change(cents - denoms[i]);
        if (sub == -1) continue;
        if (1 + sub < sol) sol = 1 + sub;
    }

    dp[cents] = sol == INT_MAX ? -1 : sol;
    return dp[cents];
}

const int N_LENGTHS = 10;
int lengths[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int rod_prices[] = {1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
int rod_dp[0x10000] = {};

int rod_price(int l) {
    if (l <= 0) return 0;
    if (rod_dp[l] != 0) return rod_dp[l];

    int best_price = 0;
    for (int i = 0; i < N_LENGTHS; i++) {
        int length = lengths[i];
        int other = l - length;
        if (other < 0) continue;
        
        int potential = rod_prices[i] + rod_price(other);
        if (potential > best_price) best_price = potential;
    }

    rod_dp[l] = best_price;
    return rod_dp[l];
}

int main() {
    for (int i = 0; i <= 50; i++) {
        printf("rod_price(%d) == %d\n", i, rod_price(i));
    }

    return 0;
}

/*
int main() {
    for (int i = 0; i <= 50; i++) {
        printf("%d: greedy: %d; fixed: %d; dyn: %d\n", 
            i, greedy_change(i), greedy_fixed(i), dyn_change(i));
    }

    return 0;
}
*/

/*
int main() {
    for (int i = 0; i < 50; i++) {
        printf("%d: %llu\t%llu\n", i, fibo(i), mat_fibo(i));
    }

    return 0;
}
*/