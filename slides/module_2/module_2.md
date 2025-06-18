---
marp: true
theme: slides
paginate: true
---

# Design and Analysis of Algorithms  

## Module 2: Bachmann-Landau Notation

<br>
<br>

Slides © Grant Williams, [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/).  

<br>

This is an open educational resource.
Submit fixes, improvements, and new material [here](https://github.com/grantwill74/notes-on-algorithm-design).


---

# Last Class
We talked about an example of a simple algorithm that was slow.

Specifically: concatenating multiple strings.

[Does anyone remember why it was slow?]

It was a little awkward to describe *how* slow it was. We had to calculate time units and we ended up with an expression like:
$T = {130*10000*(10000 + 1) \over 2}$

---

# This class

We're going to learn Big-O notation! 

This will let us describe the time an algorithm takes much faster.

You've probably heard of Big-O before, but there are other big and small letters, too: Big-O, small-o, Big-Ω*, small-ω*, and Big-θ*. We will learn the whole family!

It's called *Bachmann-Landau* notation after its inventors.
<Br>

<div class="footnote">
* Greek letter Omega, which literally means "big O" in Greek, confusingly. It's different from Big-O, mathematically though.

** This is a lower-case omega.
*** This is an upper-case theta.
**** There is no small-θ
</div>

---

# Empirical measurements are useful

There's nothing wrong with measuring how long an algorithm takes.

In fact: you should do that. It's a good idea. Sometimes algorithms can be theoretically fast an practically slow.

But there's a simple problem with that...

---

# It depends on your computer.

Other people don't have your computer.*

Mathematicians don't want to be like: "proof of running time: go get Alice's computer, then run the algorithm ten times. It will take an average of 2 minutes when n = 10000, with a standard deviation of 1.1."

Also, it's more work to deterine how an algorithm *scales* when doing purely empirical measurement. You have to take multiple measurements to determine exponents, plus additional measurements to get a feel for how much they vary. It's a decent amount of work doing *proper* empirical measurement.

<div class="footnote">
* unless you're hosting an unsecure SSH or something.
</div>
---

# Scaling

What we really care about is modelling how long an algorithm takes. 
Specifically, how it *scales*.

We would like a nice time function that lets us plug any input value we want and get a rough time estimate.

We also don't care about constant multiplication. Your computer may be 10 times faster than your phone; that doesn't change the underlying speed of the algorithm

---

# Scaling (2)

But we *do* care about *variable* multiplication.

For example, consider this time function:
$T(n)={{130\over 2}n(n+1)}=65n(n + 1)$

What matters more? The 65 at the beginning (the constant factor), or the factor of n (the variable factor)?

Well, let's say n is 5:
$T(n)=65\times 5(5 + 1)=65\times 5(6)=65\times 30$

Well 65 is bigger than 30, so it seems more important.

---

# Scaling (3)

But what about when n gets big? Say, 10,000 like before.*

$T(n)=65\times 10{,}000(10{,}000 + 1)=65\times 10^4(10^4 + 1)=65\times (10^8 + 10^4)$

$10^8 + 10^4$ is a *lot* bigger than $65$. It's roughly $384{,}654$ times larger in fact.

And that ratio only gets more wild as n gets bigger.

<div class="footnote">
*I moved the denominator of 2 under the 130 to combine the constant factors.
</div>

--- 

# Scaling (4)

It doesn't matter what constant we pick. If n gets big enough, it will matter more than the constant.

Even if it's something like $T(n) = 1000 n$ 

1000 is a large constant, but if n is, say, a million, it's no longer the dominant one.

So we like to ignore constants, and only focus on the variables. Let's do that now...

---

# Removing constants
So instead of writing
$T(n)=65n(n + 1)$

We could delete the constant factor $T(n)=n(n+1)$

And let's expand that multiplication so we can see the exponents:
$T(n)=n^2+n$

So now, we can clearly see that this is a quadratic function

---

# Can we really do this?

It might make you uncomfortable to just remove constant factors like that.

But consider: every machine takes a different amount of time to do something.

If Alice's computer takes 10 time units to copy a byte, then strcpy would have this time:
$T_{strcpy}(n)=10n$

But let's say Bob's computer is faster: it only takes 2 time units:
$T_{strcpy}(n)=5n$

This information is important, but it has nothing to do with the algorithm. 

We want to remove machine-specific information from the time function.

---

# Taking it a step further

If it bothered you that we removed the constant factor, this is *really* going to bother you: we're going to remove a whole variable.

Consider again $T(n)=n^2 + n$

What matters more, $n^2$ or $n$?

Obviously $n^2$, but let's really see how that plays out.

---

# Comparing $n^2 + n$ to just $n$

| n    | $n^2 + n$        | $n^2$     | Ratio   |
|------|------------------|-----------|---------|
| 1    | 2                | 1         | 2       |
| 10   | 110              | 100       | 1.1     |
| 100  | 10,100           | 10,000    | 1.01    |
|$10^3$| 1,001,000        | 1,000,000 | 1.001   |
|$10^6$| $10^{12} + 10^6$ | $10^{12}$ | 1.000001|  

---

# That $n$ term just does not matter

It barely changes the outcome.

Again, if one algorithm took $n^2 + n$ and another took $n^2$, by the time you get to a large input, you can't tell the difference.

But we don't want to abuse mathematical notation and just randomly delete variables...

---

<!-- _class: invert questions -->
# Questions?

---

# Introducing Big-O

The Big-O of $f(n)$, written $O(f(n))$ is the set of functions that are eventually bounded by $f(n)$ if n is large enough and if we multiply by a large enough positive constant $C$.

For example:
- $n \in O(n)$, because $n \leq C \cdot n$ if we choose $C=1$ (or 2, or 3, or anything $\geq$ 1)
- $n \in O(2n)$, because $n \leq C \cdot 2n$ if we choose $C=1$ (or as small as $0.5$)
- $n \in O(.25n)$, because $n \leq C\cdot 0.25n$ if we choose $C \geq 4$
- $n \in O(n \log n)$, because $n \leq C\cdot n \log n$ if we choose any $C \gt 0$, as n gets large
- $n \in O(n^2)$, because $n \leq C \cdot n^2$ if we choose any $C\gt 0$, as n gets large

<div class="footnote">
It actually doesn't matter what base the log is. The log functions with different bases all have constant ratios to each other.
</div>

---

# A graph

![bg right:60% height:100% A graph showing how plots of 1.25n, n log n, and n-squared all get bigger than the plot of n, either immediately or eventually as n gets large enough.](big_O_graph.svg)

Notice how, eventually all those functions get bigger than $f(n)=n$.

This would be true even of a smaller linear function, because we can multiply by any constant.

---

# Knowledge check

Give an example of a function $f(n)$, such that $n^3 \in O(f(n))$

That is, find a function $f(n)$ where $n^3 \leq C\cdot f(n)$ as n gets arbitrarily large.

---

# Some answers 

- $f(n)=n^3$
- $f(n)={n^3 \over 2}$
- $f(n)=n^4$
- $f(n)=n^{\pi}$
- $f(n)=n^3 \log n$
- $f(n)=n!$

All of these functions can be multiplied by a constant to make them $\geq$ than $n^3$

Yes, even ${n^3 \over 2}$. We can multiply it by 2!

---

# Knowledge check (2)

Give an example of a function $f(n)$, such that $n \notin O(f(n))$

That is, find a function that is too small to have its big-O contain $n$.

---

# Some answers (2)

- $f(n)=log (n)$
- $f(n)=\sqrt{n}$
- $f(n)=n^{2/3}$

No matter how much we scale these functions, eventually n will be larger than them.

Example: 
$n \gt 100\sqrt{n}$ when $n>10000$
$n \gt 10000\sqrt{n}$ when $n>1000000$
For a large enough n, for all $C$, $n \gt C\sqrt{n}$, so $n \notin O(\sqrt n)$

---

# Knowledge check (3)

Give an example of a function $f(n)$, such that $f(n) \in O(n^3)$

---

# Some answers (3)

- $f(n)=n^3$
- $f(n)=n^2$
- $f(n)=n$
- $f(n)=1$
- $f(n)=n \log n$

Yes, the functions can just be constants like $1$.

The constant function $f(n)=1 \in O(g(n))$ for any function $g(n) \gt 0$ 

---

# Knowledge check (4)

Is $n^2 \in O(n^2 - n)$?

---

# Some answers (4)

Actually yes. Let $C = 2$. What happens to this when n gets big?

$n^2 \leq 2(n^2 - n)$, distribute the 2
$n^2 \leq 2n^2 - 2n$, subtract $n^2$ from both sides
$0 \leq n^2 - 2n$
can this be made true for every n past a certain point? Add $2n$ to both sides:
$2n \leq n^2$, we only care what happens when n is big, so we can divide (assume $n>0$)
$2 \leq n$

So $n^2 - n$ can be made to eventually bound $n^2$ if we multiply it by a large enough constant. Once $n \geq 2$, $2(n^2 - n)$ will always be bigger than $n^2$.

---

<!-- _class: invert questions -->
# Questions?

---

# The formal definition

Okay, let's get more rigorous. Our definition was good before, but not quite good enough for proofs.

## Rigorous definition:
$$f(n) \in O(g(n)) \iff
\exists C \gt 0, \exists n_0 \in ℕ,\forall n \geq n_0, f(n) \leq C\cdot g(n)$$


In English: "f is in the order of g", is equivalent to saying that there are a pair of numbers: $C$ and $n_0$ which must be large enough so that $C\times g(n)$ is bigger than $f(n)$ for every choice of $n$ that is larger than $n_0$.

<div class="footnote">
Note: We're assuming that f and g are functions that return positive values (because they represent times). If you truly wish to allow f to return even negative numbers, you must put absolute value bars around it. We don't need to worry about this in the study of algorithms, but other fields use this notation too.
</div>

---

# How to think about that

If we want to prove that $f(n) \in O(g(n))$, we have to first choose a $C$ and an $n_0$.

C is our scaling factor. It just needs to be big enough to make $C\cdot g(n)$ large enough.

$n_0$ is our starting point. We don't care what happens for small values of n, we care about how the functions scale at large values. $n_0$ let's us define what "big enough" means for $n$.

Once we've chosen our $C$ and $n_0$, the last part is to prove the rest of the statement:

---

# Let's have an example

Insertion sort is a simple, but surprisingly valuable sorting algorithm.

First, let's see its code:

```c
void swap(int* a, int* b); // defined elsewhere: you know how to do it!

void ins_sort(int* arr, size_t n) { 
    for (size_t i = 1; i < n; i++) 
        for (size_t j = i; 0 < j && arr[j] < arr[j - 1]; j--)
            swap(arr + j - 1, arr + j);
}
```

<div class="footnote">
This can be slightly sped up by avoiding unecessary copies. Think about what swap does and expand it. Could you optimize the function?

Answer is in the appendix.
** "arr + i" is equivalent to "&arr[i]". We're doing pointer math to get the ith and jth elements of arr.
</div>

---


---

# Actual quiz next class

Next class we will have a quiz on this material.

This quiz counts! It's going to measure your understanding of this module.

**You must bring paper and a writing implement! This is your responsibility! Set six different reminders on your phone!***

---

# Actual quiz next class (2)

Start studying now, and try to resolve any feelings of meta-cognitive unease. If you feel like "I don't quite get this", listen to the feeling!

Test yourself. The quiz will be proctored, pen-and-paper, and timed (10 minutes). If you aren't studying at least a little bit under these time and resource controls, you aren't studying for the quiz!

The quiz will test the first learning mastery standard: that you can understand and prove things using Bachmann-landau notation.

---

# How should I study?

Do the following practice exercises! Some are worked, some are not.

Then, rewrite them on a cheat sheet. It's an open materials quiz. Any printed material is acceptable. Composing your own cheat sheet will help comprehension a lot.

Set a practice timer for each problem. Turn off your electronic devices. You will have 10 minutes.

Remember: *if you aren't studying under time controls with pen and paper, **you aren't studying!***

And remember to bring pen and paper for the quiz next class!