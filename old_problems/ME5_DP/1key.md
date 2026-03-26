# Key
It's the fibonacci sequence without the 0 term.

The solution for n stairs is that we have all the ways of jumping up n - 2 stairs followed by a 2 jump, or all the ways of jumping up n - 1 stairs followed by a one jump.

```c
int n_ways(int n)  {
    int dp[1001]; // we could just use two accumulators 
    dp[1] = 1;
    dp[2] = 2;

    for (int i = 3; i <= n; i++) {
        dp[i] = dp[i - 2] + dp[i - 1];
    }

    return dp[n];
}
```

Grading:
- -100 pts: not $O(n)$
- -100 pts: used an outside function
- If you discovered the key insight, that is, you either did a fibonacci-style solution or a table for the i - 2 and i - 1 numbers, you get at least an 80.
- The remainder of the score is partial credit for minor errors (bounds checking, types, dp size, etc.). 5 points per error.
- If you did not discover the insight, unfortunatly no credit for this one.

