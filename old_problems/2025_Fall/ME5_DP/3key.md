# Key

```c

int max2(int a, int b) {
    return a > b ? a : b;
}

int max3(int a, int b, int c) {
    return max2(a, max2(b, c));
}

int max_xp(Spawn spawns[], int n_spawns) {
    // don't need a giant array, only need 3 elements.
    // This way is a bit easier to understand imo.
    static int dp[1000000][3] = {{}}; 
    dp[0][spawns[0].zone] = spawns[0].xp;

    for (int i = 1; i < n_spawns; i++) {
        int z = spawns[i].zone;
        // if we stayed in the zone vs if we moved there
        int if_stayed = dp[i - 1][z] + spawns[i].xp;
        int if_moved = max2(dp[i - 1][(z + 1) % 3], dp[i - 1][(z + 2) % 3]);
        dp[i][z] = max2(if_stayed, if_moved);

        // otherwise, we ignored the monster
        dp[i][(z + 1) % 3] = dp[i - 1][(z + 1) % 3];
        dp[i][(z + 2) % 3] = dp[i - 1][(z + 2) % 3];
    }

    return max3(dp[n_spawns - 1][0], dp[n_spawns - 1][1], dp[n_spawns - 1][2]);
}

```

Grading: 
+ 10 pts: defines function correctly. returns a result of right type.
+ 10 pts: dp array is configured correctly, either dp[1000000][3] or just dp[3].
          (alternatively, the problem could be done in reverse; that's fine.)
+ 20 pts * 4: for each DP case (if stayed in each of 3 zones, if moved).
- 50 pts penalty: solution does not run in O(s)
- 50 pts penalty (each): outside functions used

ignore minor mistakes: semicolons, missing braces, operator precidence with "%"