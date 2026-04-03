
# Key

## Problem 1
One iterative version: 
```c
size_t nodd(size_t[] arr, size_t n) {
    size_t count = 0;
    for (size_t i = 0; i < n; i++) {
        // it's perfectly fine to just check arr[i] % 2 == 1
        count += arr[i] & 1; // but you can be fancy, too
    }
    return count;
}
```

One recursive version:
```c
size_t nodd(size_t[] arr, size_t n) {
    if (n == 0) return 0;
    return (arr[0] & 1) + nodd(arr + 1, n - 1);
}
```

### Test cases:
- +10 points if your version handles $n = 0$ correctly (result should be zero). No partial credit here.
- +10 points if your version has the correct inductive logic (checking that the value is odd and adding 1, and doing nothing if it is even). Give +5 points partial credit if you got it exactly backwards (adding 1 for even instead of odd).
- +5 points if your function typechecks. You should have either a size_t as your accumulator/return, or you should cast an int to a size_t. No floats or strings should appear in the code (don't award these points if they do).

## Problem 2
For the iterative version
```c
size_t nodd(size_t[] arr, size_t n) {
    size_t count = 0;
    // I: count == length(filter(odd, arr[0..i)))
    for (size_t i = 0; i < n; i++) {
        // if arr[i] is odd, count' = count + 1
        // but if arr[i] is even, count' = count
        count += arr[i] & 1;
    }
    return count;
}
```

For the recursive version
- For $n = 0$, `nodd` returns 0, which is correct.
- Suppose the algorithm works for $n$, show it works for $n + 1$
    - If the new value is odd, we return 1 + the value for n - 1.
    - If the new value is even, we leave the count unchanged
    - Either way, by the inductive hypothesis, the count is correct.


### Grading:
- For the iterative version: 
    - +15 points for a loop invariant which is correct. -5 for a fencepost error (accidentally including `i` in the array. We can assume that [0..i] is a half-open range if you wrote that instead of [0..i))
    - +10 points for the inductive step inside the loop. -5 for minor mistakes in this inductive step (e.g., mixing up even and odd)

- For the recursive version
    + 10 for the correct base case
    + 15 for the correct inductive step


## Problem 3

Both versions run in $\Theta(n)$

### Grading:

This one is all or nothing. If by some chance your version is *not* linear, then it's up to you to grade this, but please solve problems like this in linear time in the future.

## Problem 4

For the iterative version: there is one for-loop, which iterates from 0 .. n. The action inside consists of an addition, a bit mask, and an array offset, all of which are $\Theta(1)$ by the ram model. $\Theta(1) + \Theta(1) + \Theta(1) = \Theta(1)$. We do a $\Theta(1)$ action $\Theta(n)$ times, so the result is $\Theta(n)$

For the recursive version: 
The recurrence relation is $T(n) = T(n - 1) + \Theta(1)$, which is $\Theta(n)$ by the linear recurrence relation lemma/theorem.

### Grading

- For the iterative version:
    - +15 if you recognized the outer $\Theta(n)$
    - +10 if you explicitely mentioned the inner $\Theta(1)$, or at least that you didn't contradict it (I'm willing to accept answers like "The outer loop iterates $n$ times" with the assumption that the inner loop is constant)
- For the recursive version:
    - +10 for a recurrence relation
    - +15 for either invoking the lemma or inductively proving it from scratch.


