---
marp: true
theme: slides
paginate: true
---

# Design and Analysis of Algorithms  

## Module 3: 

<center>

### The Bachmann-Landau family
### Best/worst/average case
### recursive algorithms

</center>

<br>
<br>

Slides © Grant Williams, [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/).  

<br>

This is an open educational resource.
Feel free to submit fixes, improvements, and new material [here](https://github.com/grantwill74/notes-on-algorithm-design).


---

# Last Class

We talked about the definition of big-$O$:
$$f(n) \in O(g(n)) \iff
\exists C \gt 0, \exists n_0 \in ℕ,\forall n \geq n_0, f(n) \leq C\cdot g(n)$$

Then we used it to prove that certain functions had the big-$O$ we wanted.

We learned how to annotate big-$O$, too, for iterative procedures.

We learned how to prove correctness. We showed that an iterative version of insertion sort was correct.

---

# This class

We're going to learn about the rest of the Bachmann-Landau family:
small-$o$, big-$\Omega$, small-$\omega$, and big-$\Theta$

We're going to learn how to analyze recursive algorithms: both for correctness and for runtime complexity bounds.

We're going to get even sharper with induction.

---

# Back to insertion sort

Remember when I said insertion sort was very useful?

But we proved that it was $O(n^2)$

That is not great. We'll see mergesort next module, and it is $O(n \lg n)$.

So if insertion sort is slower than another simple, widespread algorithm, why would anyone use it?

---

# When is insertion sort useful?

Let's look at the code for insertion sort again. This is using the faster version that I suggested writing as a practice exercise:

```c
void ins_sort(int* arr, size_t n) {
    for (int i = 1; i < n; i++) {
        int t = arr[i]; int j = i;
        for (; j > 0 && arr[j - 1] > t; j--)
            arr[j] = arr[j-1];
        arr[j] = t;
    }
}
```
This version stores the unsorted value in $t$. Then finds where $t$ should go, and shifts everything above it to the right, making room.

Important question: [when is this function *not* quadratic?]

---

# When the inner loop doesn't run.

Look at the inner loop:
```c
for (; j > 0 && arr[j - 1] > t; j--)  // the inner loop
    arr[j] = arr[j-1];
```

This loop has two stopping conditions:
1. $j$ reaches $0$
2. $\mathrm{arr}[j - 1] \leq t$

If $j$ reaches $0$, the inner loop runs in $O(i)$, which is $O(n)$, making the sort $O(n^2)$
If $j$ reaches a *proportion* of $0$, *the same thing happens*.


--- 

# When the inner loop doesn't run (2)

Even if it only goes halfway before stopping, $n/2$ is still $O(n)$
Even if it only goes a *tenth* before stopping, $n/10$ is still $O(n)$

But if $j$ stops after a constant number (such as $0$), it becomes $O(1)$

The outer loop is still $O(n)$, making the sort $O(n)$ in total.

[And when does this happen? What situation is guaranteed to make the inner loop always get skipped?]

---

# When the inner loop doesn't run (3)

When $\mathrm{arr}[j - 1] \leq t$.

That is, when $\mathrm{arr}[i - 1] \leq \mathrm{arr}[i]$ for every $i \gt 0$

That is, when the array is already sorted.

---

# Why is that useful?

It's useful when we're measuring a natural process with some entropy, but not a lot. Like if a list is sorted but later you insert a constant number of things into it randomly.

When you need to sort transparent objects* in a 3D computer graphics scene, but the camera hasn't moved that much.

When you want to sort objects by their distance to a point in a physics simulation, but their relative orders rarely change from frame to frame. Example: a billiards game

When you want a server to respond to requests in order of priority, but most requests have the same priority.

<div class="footnote">

\* or any object on a system like the Playstation 1, which had no depth buffer.

</div>

---

# How good is that?

In all the previous examples, insertion sort runs in $O(n)$.

It turns out, this is the theoretical best running time possible for a sorting algorithm that doesn't know an unreasonable amount of information about the list.*

So this is *really* good. But the big-$O$ doesn't tell us that. The big-$O$ tells us that the algorithm is bad. We need more information!

<div class="footnote">

\* In the example of inserting a constant number of items into a list, if we also knew the locations at which they were inserted, and we knew the rest of the list were sorted, then we could sort in $O(1)$ time by just sorting those exact items.

</div>

---

# Big-$\Omega$

That's a Greek capital letter $\Omega$, whose name confusingly means "Big 'O'" in Greek.

If I hadn't told you that, it wouldn't be confusing. I'm sorry.

Anyway, Big-$\Omega$ is the counterpart to Big-$O$.

In the same way that Big-$O$ gives us an *upper* bound\* of a function, Big-$\Omega$ gives us a *lower* bound\*\*.

<div class="footnote">

\* if multiplied by a constant
** again, if multiplied by a constant

</div>

---

# The definition of Big-$\Omega$




---




---

# The classic joke

If I ask a question like "what is the big-$O$ 

small o

---

# Example table of Big-O's

Remember, we use $O$ (and the other notations we'll introduce soon)

Here are some common big-O's and the kinds of problems they tend to emerge from:

| Big-O     | Kind of problem 
|-----------|----------------------------------------------
| $1$       | simple machine operation (arithmetic on int, boolean expression eval., etc.)
| $\lg n$   | binary search
| $n$         | linear search, many string operations, arithmetic on BigInts, tons of things
| $

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

Then, rewrite them on a cheat sheet. It's an open materials quiz. Any printed material is acceptable. Composing your own cheat sheet will help comprehension a lot. I recommend 1 sheet per learning standard.

Set a practice timer for each problem. Turn off your electronic devices. You will have 10 minutes.

Remember: *if you aren't studying under time controls with pen and paper, **you aren't studying!***

And remember to bring pen and paper for the quiz next class!

---