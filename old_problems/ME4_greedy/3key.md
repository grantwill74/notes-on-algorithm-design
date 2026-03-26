# Key

```c
int cmp(const void* a, const void* b) {
    Beat* ba = a; Beat* bb = b;
    return ba->at_time - bb->at_time;
}

int max_score(Beat* beats, size_t n_beats) {
    qsort(beats, n_beats, sizeof(Beat), cmp);
    int score = 0;
    for (int i = 0; i < n_beats; i++) {
        score++;
        while (i + 1 < n_beats && beats[i + 1].at_time == beats[i].at_time)
            i++;
    }
    return score;
}
```

Not $O(n \lg n)$: -50

Not every used function is defined (except qsort): -50

Didn't sort by at_time or equivalent: -50

Comparator is worth 25 points

-5 for every distinct error with qsort

-5 for minor errors skipping ranges or off-by-ones
