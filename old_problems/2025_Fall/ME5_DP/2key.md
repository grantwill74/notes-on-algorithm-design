# Key

```c
int max_value(Task tasks[], int n_tasks) {
    int dp[10000];
    dp[0] = tasks[0].value;

    for (int i = 1; i < n_tasks; i++) {
        // find the last non-conflicting task, j, if it exists
        int j = i - 1;
        for (; j >= 0 && tasks[j].finish >= tasks[i].start; j--)
            ;
        int take_it = tasks[i].value + (j == -1 ? 0 : dp[j]);
        int leave_it = dp[i - 1];
        dp[i] = take_it > leave_it ? take_it : leave_it;
    }

    return dp[n_tasks - 1];
}

```

Grading: 
+ 10 pts: defines function correctly. types consistent.
+ 10 pts: dp array is configured correctly
+ 20 pts: finding the last non conflicting task
+ 20 pts * 2: for take it/leave it
+ 20 pts: dp update is correct 
- 100 pts penalty: solution does not run in O(n^2)
- 100 pts penalty: outside functions used

All positive grading items admit partial credit.