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

# Exercise 1

Give an example of a function $f(n)$ such that $n \notin O(f(n))$

That is, find a function that is too small to have its big-O contain $n$.

---

# Some exercise 1 answers
- $f(n)=log (n)$
- $f(n)=\sqrt{n}$
- $f(n)=n^{2/3}$

No matter how much we scale these functions, eventually n will be larger than them.

Example: 
$n \gt 100\sqrt{n}$ when $n>10000$
$n \gt 10000\sqrt{n}$ when $n>1000000$
For a large enough n, for all $C$, $n \gt C\sqrt{n}$, so $n \notin O(\sqrt n)$

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