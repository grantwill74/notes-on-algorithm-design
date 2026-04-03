# Key

## Problem 1:

iterative:
```c
bool is_sorted(size_t arr[], size_t n) {
    for (size_t i = 0; i + 1 < n; i++)
        if (arr[i] > arr[i + 1])
            return false;
    return true;
}
```

recursive:
```c
bool is_sorted(size_t arr[], size_t n) {
    if (n <= 1) return true;
    return arr[0] <= arr[1] && is_sorted(arr + 1, n - 1);
}
```

### Grading

Give yourself 5 points for each test case your code passes:
```c
is_sorted((size_t[]){},          0) == true;
is_sorted((size_t[]){5},         1) == true;
is_sorted((size_t[]){1,2,2,3,5}, 5) == true;
is_sorted((size_t[]){1,3,2,4},   4) == false;
is_sorted((size_t[]){3,1,2,4},   4) == false;
```

If you accidentally wrote `i < n - 1` instead of `i + 1 < n` subtract 1 point only (don't deduct an entire 5 for missing the first test). I did the same thing the first time I worked this one =(

Be careful with the recursive solution, if you wrote:
`... && is_sorted(arr + 2, n - 2)`, you should fail the {1, 3, 2, 4} test case above.

## Problem 2:

iterative:
```c
// I: the array is sorted from [0..i) 
bool is_sorted(size_t arr[], size_t n) {
    for (size_t i = 0; i + 1 < n; i++)
        // if arr[i] > arr[i + 1], the invariant breaks, but
        // we return. Otherwise it is maintained.
        if (arr[i] > arr[i + 1])
            return false;
    return true;
}
```

recursive:
- If $n = 0$, we return true, which is correct.
- If $n = 1$, we return true, which is correct
- If it's correct for $n - 1$, we test the condition between `arr[0]` and the first element of the remaining array, `arr[1]`.
    - If `arr[0] <= arr[1]`, then by the inductive hypothesis, `arr[0..n)` is sorted.
    - Otherwise, we know the array isn't sorted, and we return false correctly.

We had to handle both $n = 0$ and $n = 1$ as base cases the way I wrote this proof, because the inductive case assumes there are two elements in the array.


### Grading

Iterative:
- (15 pts) loop invariant is correct: the array must be sorted up to but not including the value we're considering. -5 points for a fencepost error.
- (10 pts) you must justify that the invariant is maintained inside the array. -5 points for a fencepost error, or accidentally inverting the order of the array elements.


## Problem 3:

It should be $\Theta(n)$, unless you sorted the array with a comparison sort and compared it with the original, in which case $\Theta(n \lg n)$ or $\Theta(n^2)$ depending on your sort (or no $\Theta$ if quicksort specifically was used).

### Grading: 
All or nothing

## Problem 4:

Iterative:
- Our loop runs $n - 1$ times (or potentially $n$ or $n - 2$ times depending on how you handled the loop)
- Our loop body code runs in $\Theta(1)$
- Therefore, We have $(n - 1) \times \Theta(1) = \Theta(n)$

Recursive:
- Our recurrence relation is $T(n) = T(n - 1) + \Theta(1)$
- By the linear recurrence relation lemma, this is $\Theta(n)$.

### Grading:

Iterative:
- (15 points) you counted the outer loop correctly. Either $\Theta(n)$ or more specifically $n - 1$ or whatever it ended up being in your loop. Lose all points if you are off by an order of magnitude (e.g., $\Theta(n^2)$ instead of $\Theta(n)$), -1 points if you're off by a constant (e.g., $n - 2$ instead of $n - 1$).
- (10 points) you correctly identified the cost per iteration (should be $\Theta(1)$). Could also count the number of machine ops or something.

Recursive:
- (10 points) for the recurrence relation. Deduct all 10 for a major mistake (n / 2) instead of (n - 1) or something. Deduct 1 for a typo.
- (15 points) for invoking the correct lemma. If you proved it inductively, 5 points for a correct base case and 10 points for a correct inductive case.

