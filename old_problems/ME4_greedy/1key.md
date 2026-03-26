# Key

```c
int cmp(void* a, void* b) {
    Task* ta = a;
    Task* tb = b;
    return ta->finish - tb->finish;
}

int max_tasks(Task tasks[], int n_tasks) {
    qsort(tasks, n_tasks, sizeof(Task), cmp);
    int result = 1;
    int prev_finish = tasks[0].finish;
    for (int i = 1; i < n_tasks; i++)
        if (tasks[i].start > prev_finish) {
            prev_finish = tasks[i].finish;
            result++;
        }

    return result;
}
```

Grading: for every unique mistake with `qsort` or with a custom sort, -5 points.
Using an undefined sort instead of `qsort` or a well-defined custom sort, -25 points.

If the tasks are not sorted by finish time, -50 points

If solution is not $O(n \lg n)$, -50 points

Forgetting to check the previous finish time: -25 points

Forgetting to count the results: -10 points

