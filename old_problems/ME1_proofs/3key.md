# Key

## Problem 1:

Iterative:
```c
size_t ipow(size_t base, size_t exp) {
    size_t result = 1;

    while (exp) {
        if (exp & 1) {
            exp--;
            result *= base;
        } else {
            exp >>= 1;
            result *= result;
        }
    }

    return result;
}
```

Recursive:
```c
size_t ipow(size_t base, size_t exp) {
    if (exp == 0) return 1;
    if (exp & 1) return base * ipow(base, exp - 1);
    size_t root = ipow(base, exp >> 1);
    return root * root;
}
```

### Grading:

5 points for each test case you get right.

If you did a linear time solution, -15 points.

## Problem 2:

Iterative:
```c
size_t result = 1;

// I: let exp_0 be the original value of exp, pre-loop
//    then result = base ^ (exp_0 - exp)
while (exp) {
    // if the exponent is odd...
    if (exp & 1) {
        exp--;
        result *= base; 
        // now, result' = base * base ^ (exp_0 - (exp - 1)) 
        //              = base ^ (exp_0 - exp)
    } else {
        exp >>= 1;
        result *= result;
        // now, result = base ^ (exp_0 - exp / 2) ^ 2
        //             = base ^ (exp_0 - exp)
    }
}

return result;
```

Recursive:
By strong induction on `exp`

Base case: $n = 0 \implies ipow(base, 0) == 1$. True because of the guard.

Inductive case: suppose correct for $n$.
For `ipow(base, exp + 1)`, we have two sub-cases:
- `exp + 1` is even, in which case `ipow(base, (exp + 1) / 2) ^ 2` is the expected result and `ipow(base, (exp + 1) / 2)` is correct by IH.
- `exp + 1` is odd, and `ipow(base, exp)` is correct by IH, then we derive `base * ipow(base, exp) == ipow(base, exp + 1)`, which is correct.

### Grading

Iterative:
- (15 pts) loop invariant should be stated in terms of the result value. Either something like I did, or if you looped with an iterating variable like `i`, `base ^ (exp - i)`.
- (10 pts) -5 pts if only the even or the odd case was handled.

Recursive:
- (10 pts) correct for $n = 0$. (probably a freebie?)
- (15 pts) needed to handle both cases, and invoke the inductive hypothesis for subtracting 1 vs dividing by 2.

## Problem 3

This is $\Theta(\log \mathrm{exp})$

### Grading

all or nothing

## Problem 4

Iterative:
- For every iteration, it can be odd at most once every time it halves. Therefore, it will run $C \times \lg \mathrm{exp}$ times, for some $C \le 2$.
- Internally, we have an if statement which performs a bit test and some simple arithmetic. It's a constant number of  $\Theta(1)$ operations.

Therefore, we end up with $\Theta(C \times \lg \mathrm{exp}) = \Theta(\lg \mathrm{exp})$ which implies $O(\lg \mathrm{exp})$

Recursive:
Recurrence relation:

$T(0) = 1$ 

$T(n) = T(n - 1) + \Theta(1)$ if exp is odd

$T(n) = T(n / 2) + \Theta(1)$ if exp is even

Every time it's odd, we perform a $\Theta(1)$ operation, so this is equivalent:

$T(n) = T(n / 2) + 2\times \Theta(1) = T(n /2) + \Theta(1)$

$c_{crit} = 0 = c$ because $1 = \mathrm{exp}^c$ if $c = 0$.
This is case 2 of the master theorem.
There are no logarithms in the $\Theta(1)$ term, so the result is $\Theta(\log(n)) \implies O(\log \mathrm{exp})$

If you want to prove it without the master theorem,
We want to show: $T(n) \le C \cdot \lg \mathrm{exp}$.
Using the substitution method, this is equivalent to
$T(n / 2) + c \le C \cdot \lg (n / 2) \equiv$
$T(n / 2) + c \le C \cdot \lg \mathrm{exp} - \lg 2 \equiv$
$T(n / 2) + c \le C \cdot \lg \mathrm{exp} - 1$

which follows from the inductive hypothesis (if $T(n / 2) + c \le C \cdot \lg(n / 2)$, then 1 greater than that is bounded by the log of a twice-as-large exp)


### Grading

Iterative:
-1 pts for sub-order-of-magnitude mistakes in the count. 
otherwise lose all points

The loop body should be all or nothing, since it's constant time.

Recursive:

-5 points if the recurrence leaves something out (like the constant term)
lose all points if more than one mistake of that type is made.
manual grading for the 15 point proof.
