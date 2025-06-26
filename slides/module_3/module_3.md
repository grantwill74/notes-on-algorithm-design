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
little-$o$, big-$\Omega$, little-$\omega$, and big-$\Theta$

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

When you want a server to respond to requests in order of priority, but almost all requests have the same priority except for a constant number.

<div class="footnote">

\* or any object on a system like the Playstation 1, which had no depth buffer.

</div>

---

# How good is that?

In all the previous examples, insertion sort runs in $O(n)$.

It turns out, this is the theoretical best running time possible for a sorting algorithm that doesn't know an unreasonable amount of information about the list.*

So this is *really* good. But the big-$O$ doesn't tell us that. The big-$O$ tells us that the algorithm is bad. We need more information!

<div class="footnote">

\* In the example of inserting a constant number of items into a list, if we also knew the locations at which they were inserted, and we knew the rest of the list were sorted, then we could sort in $O(1)$ time by just sorting those exact items. Since there are a constant number, it would be O(1).

</div>

---

# Big-$\Omega$

That's a Greek capital letter Omega, whose name confusingly means "Big 'O'" in Greek.

If I hadn't told you that, it wouldn't be confusing. I'm sorry.

Anyway, Big-$\Omega$ is the counterpart to Big-$O$.

In the same way that Big-$O$ gives us an *upper* bound\* of a function, Big-$\Omega$ gives us a *lower* bound\*\*.

<div class="footnote">

\* if multiplied by a constant
** again, if multiplied by a constant

</div>

---

# The definition of Big-$\Omega$

$$
f(n) = \Omega(g(n)) \iff
\exists C \gt 0, \exists n_0 \in \mathbb{N},\forall n \geq n_0, f(n) \geq C\cdot g(n)
$$


Look familiar?

<div class="footnote">

\* Note: Again, we're assuming that $f(x)$ and $g(x)$ have a positive co-domain. If either can be negative we need magnitude bars around them:
$|f(n)| \geq C \cdot |g(n)|$

</div>

---

# Comparison between Big-$\Omega$ and Big-O

$$f(n) = \Omega(g(n)) \iff
\exists C \gt 0, \exists n_0 \in \mathbb{N},\forall n \geq n_0, f(n) \geq C\cdot g(n)$$

$$f(n) = O(g(n)) \iff
\exists C \gt 0, \exists n_0 \in ℕ,\forall n \geq n_0, f(n) \leq C\cdot g(n)$$

What's the difference?

---

# The only difference

Big-$\Omega$ requires $f(n) \ge C \cdot g(n)$, while Big-$O$ requires $f(n) \le C \cdot g(n)$ 

This means that proofs of big-$\Omega$ use the same techniques.

But first, let's make sure we understand what it means for $f(n) = \Omega(g(n))$

---

# Knowledge Check

Find $f(n)$ such that the following statement is true: 
$$f(n) = \Omega(n)$$


---

# Knowledge check answers:

Some acceptable answers:
- $f(n)=n$
- $f(n)={n \over 2}$
- $f(n)=2n$
- $f(n)=n^2$
- $f(n)=n \lg n$
- $f(n)=n^{3/2}$
- $f(n)=2^n$
- $f(n)=n!$

---

# Knowledge check (2)

Find $g(n)$ such that $n \lg n = \Omega(g(n))$

---

# Knowledge check answers (2)

Some correct answers:
- $g(n) = n \lg n$
- $g(n) = {n \lg (n / 2) \over 2}$ 
- $g(n) = n$
- $g(n) = \lg n$
- $g(n) = (\lg n)^2$
- $g(n) = (\lg n)^k$ for any k
- $g(n) = 1

---

# Compare to our earlier knowledge check

Last module, I asked you to find $f(n)$ and $g(n)$ such that $f(n) = O(g(n))$

It turns out, you already understood big-$\Omega$, because $f(n)$ and $g(n)$ have this relationship: 
$$f = O(g(n)) \iff g = \Omega(f(n))$$

We're going to prove this, but how? Walk me through how we prove that proposition?

---

# Proving it:

Let's follow this strategy:
- Apply the definitions of $O(g(n))$ and $\Omega(f(n)))$
- Break the $\iff$ into two implications
- Break down the assumptions for their $C$ and $n_0$ in each implication.
- Show that the resulting implications about inequalities follow.

---

# Proving similarity between $O$ and $\Omega$

Subgoal: $f = O(g(n)) \implies g = \Omega(f(n))$
- Apply the definitions of $O(g(n))$ and $\Omega(f(n))$
  Now we must show:
  $\exists C \gt 0, \exists n_0 \in \mathbb{N}, \forall n \ge n_0, f(n) \leq C \cdot g(n) \implies$ $\exists C' \gt 0, \exists n'_0 \in \mathbb{N}, \forall n \ge n'_0, g(n) \geq C' \cdot f(n)$ 
- Suppose\* we have a $C > 0$, an $n_0 \geq 0$, and that $\forall n \geq n_0, f(n) \leq C \cdot g(n)$
  We must show: $\exists C' \gt 0, \exists n'_0 \in \mathbb{N}, \forall n \ge n'_0, g(n) \geq C' \cdot f(n)$ 
- Choose $C' = {1 \over C}$, $n'_0=n_0$, now we must show $\forall n \geq n_0,g(n)\geq{1 \over C}\cdot f(n)$ 
- This follows from our earlier assumption, after multiplying both sides by $C$.


<div class="footnote">

\* Remember that if our goal is $P \implies Q$, we "suppose" $P$. If we have an assumption of the type $\exists a, P(a)$, we also "suppose" we have some $a$ named whatever we want. So we're skipping right to that: we're supposing the whole hypothesis and then supposing we get the values of $C$ and $n_0$ out of it. This is the kind of shortcut you can take once you're comfortable writing proofs.

</div>

---

# Back to insertion sort

Now that we've learned about big-$\Omega$, let's show that insertion sort is $\Omega(n)$:

```c
void ins_sort(int* arr, size_t n) {
    for (int i = 1; i < n; i++) {               // Omega(n)
        int t = arr[i]; int j = i;
        for (; j > 0 && arr[j - 1] > t; j--)    // Omega(1)
            arr[j] = arr[j-1];
        arr[j] = t;
    }
}
```

We can justify these annotations as following:
- The outer loop always runs $n$ times, so let $C = 1$: it always runs *at least* $C \cdot n$ times
- The inner loop might terminate immediately, so it might never run. However, the little test that checks whether it should run still takes $\Omega(1)$ time.

---

# Practice

We've only proved one goal: $f = O(g(n)) \implies g = \Omega(f(n))$
If we want to show $f = O(g(n)) \iff g = \Omega(f(n))$, we must also prove $g = \Omega(f(n)) \implies f = O(g(n))$
**Do this as a practice exercise.**

Also, prove the following from the definition: $n^2 = \Omega(n \lg n)$ 

---


# Practice (2)
Practice, annotate this code for big-$O$ and big-$\Omega$:
```c
void arg_min(int* arr, size_t n);
void sel_sort(int* arr, size_t n) {
    if (n == 0) return;
    for (int i = 0; i < n; i++) {
        int min_i = i + arg_min(arr + i, n - i);
        swap(arr + min_i, arr + i);
    }
}
```

Write an `arg_min` and annotate it for big-$O$ and big-$\Omega$ also.

The answer should be $O(n^2)$ and $\Omega(n^2)$ in total for the whole sort.

---

<!-- _class: invert questions -->
# Questions?

---

# The classic joke

If I ask a question like "what is the big-$O$ of this algorithm", there is usually someone who enjoys saying "it's $O(n!!)$" (i.e., "factorial factorial") or something. 

This is technically true and completely useless, which is what makes it funny. 

You can do the same thing with $\Omega$: technically every algorithm we learn is $\Omega(0)$, so saying "it's $\Omega(0)$ is technically correct. It also gives no information at all.

---

# Bounds that can be tighter

Of course computer scientists are aware of this, which is why there are rigorous ways of saying "you could have a tighter bound".

Let's consider two statements of big-$O$:
1. $10n = O(n)$
2. $10n = O(n^2)$

First, let's give these a brief proof. What is a choice of $C$ and $n_0$ that will demonstrate membership in the order for both statements?

---

# Bounds that can be tighter (2)

Here are some answers:
1. For $10n = O(n)$, we can choose $n_0 = 0$, and then we have to be careful: we have to then choose some $C \geq 10$. Let's choose $10$, it follows: $\forall n \ge 0, 10n \leq 10n$.

2. For $10n = O(n^2)$, we can choose *any positive C we want*. It doesn't matter, it will just change our choice of $n_0$. $n^2$ grows so much faster than $n$, that we can choose $C = .001$, and: 
$\exists n_0 \in \mathbb{N}, \forall n \ge n_0, 10n = O(n^2) \impliedby$
$\exists n_0 \in \mathbb{N}, \forall n \ge n_0, 10n \le .001 \cdot n^2 \impliedby$ 
$\exists n_0 \in \mathbb{N}, \forall n \ge n_0,10 \le .001\cdot n \impliedby$
$\exists n_0 \in \mathbb{N}, \forall n \ge n_0, 10000 \le n$

So choose $n_0=10000$ and you're done. Clearly there's an $n_0$ even for $C=10^{-100}$

---

# Bounds that can be tighter (3)

Why does this happen? Why is the proof so much more flexible for $O(n)$ vs. $O(n^2)$?

Because the bound of $n = O(n)$ is tight. There are some choices of $C$ that will work and some that won't. So *some* linear functions bound $f(n)=n$, but some don't.

We can't chose a smaller power of $n$ or some other sub-linear function. This is as "good" a statement of $O$ as we can obtain for $f(n) = n$

But for $n = O(n^2)$, the bound has room for improvement. We could improve it to $n = O(n^{1.5})$ or $n = O(n \lg n)$, but the best choice is $O(n)$.

---

# Expressing that rigorously

We can understand intuitively what it means to say "that big-$O$ can be tighter".

But how can we express that mathematically?
- It's not good enough to say something like "it's not tight if $f(n)=O(n^k)$, but $\exists k' \le k,f(n)=O(n^{k'})$, because what if the function isn't a polynomial?
- It's also not good enough to say something like "it's not tight if $\exists g'(n), f(n)=O(g'n) \land \forall n, g'(n) < g(n)$, i.e., "if we can find a smaller function that is also in the order of $g(n)$. The reason this definition is not useful is that it would mean $n = O(n)$ is not tight, because $n=O(n / 2)$ (and so on).

So we want to express "that big-$O$ can be tighter" in a way that is rigorous, which could potentially apply to any kind of function, and which still has reflexivity (i.e., $f(n)=O(f(n))$ should be tight)

---

# little-$o$

For this, we introduce a new notation: $o(g(n))$. Its definition:
$$
f(n)=o(g(n)) \iff \forall c \gt 0, \exists n_0 \in \mathbb{N}, \forall n \ge n_0,f(n) \leq c \cdot g(n)
$$

Compare this to the definition of $O(g(n))$:

$$
f(n)=O(g(n)) \iff \exists C \gt 0, \exists n_0 \in \mathbb{N}, \forall n \ge n_0,f(n) \leq C \cdot g(n)
$$

What's the difference?

---

# little-$o$ (2)

$$
f(n)=o(g(n)) \iff \forall c \gt 0, \exists n_0 \in \mathbb{N}, \forall n \ge n_0,f(n) \leq c \cdot g(n)
$$

There are two differences:
1. We changed the $\exists$ to a $\forall$
2. We lowercased the $C$ into  a $c$ (to make it clear that it is quantified differently)

(The book's definition uses strict $\lt$ rather than $\le$. It doesn't really matter, and some of the proofs are slightly easier with $\le$, so that's the one I use.)

---

# Knowledge check 

Is $n = o(n^2)$? If so, prove it, if not, prove that it isn't.

[Take a second, how would we prove it?]

---

# Knowledge check (2)

It is. We can choose *any* $c \gt 0$, and eventually $n \le c n^2$.

Let's apply the definition to the goal. We must show:
$\forall c \gt 0, \exists n_0 \in \mathbb{N}, \forall n \ge n_0,n \leq c \cdot n^2$

There's a $\forall$ in front, so let's start with "suppose"

---

# Knowledge check (3)

- suppose we have a $c \gt 0$
  we must show $\exists n_0 \in \mathbb{N}, \forall n \ge n_0, n \leq c \cdot n^2$
- choose $n_0 = \lceil {1 \over c} \rceil$. New goal: $\forall n \ge \lceil {1 \over c} \rceil, n \leq c \cdot n^2$
- It is safe to divide both sides by $n$, because we know it is $\gt 0$,
  so our goal becomes $1 \le c \cdot n$
- Our assumption is $\lceil {1 \over c} \rceil \le n$. We can multiply both sides by $c$.
- $1 \le c\lceil {1 \over c} \rceil \le cn$, from which the goal follows. $\square$

---

# Important note about little-$o$ proofs

It's pretty important that, when choosing $n_0$, to incorporate $c$ in some way.

Remember, the goal needs to hold *for all possible choices of c*. Suppose we need to show $1 = o(n)$. If we just choose $n_0 = 0$ or $n_0 = 1$ like we did with the big-$O$ proofs, it won't work:

$\forall n \ge 1, \forall c > 0, 1 \le cn$ is not a true statement. What if $c=0.0001$? 

We need $n_0$ to be big enouch so that we can divide out the $c$, but also have it be a natural number. So we often choose some function of $c$ but rounded up.

---

# Little-$o$ practice

1. Show that $1 = o(n)$
2. Show that $1 = o(\lg n)$. Hint: consider if $n_0 = 2^{(1 / c)}$. Does a natural number work?
3. Use the previous result to show $n = o(n \lg n)$
4. Show that $\lnot (n = o(n))$
5. Is this true? Prove it one way or another:
   $f(n) = o(g(n)) \implies g(n) = o(h(n)) \implies f(n) = o(h(n))$

<div class="footnote">

Reminder: this is curried logic. $(P \implies Q \implies R) \iff (P \land Q \implies R)$
Note: $\implies$ is right associative, so this is also true: $(P \implies (Q \implies R)) \iff (P \land Q \implies R)$
However, this is **not** true: $((P \implies Q) \implies R) \iff (P \land Q \implies R)$

---

<!-- _class: invert questions -->
# Questions?

---

# What about loose $\Omega$ bounds?

Now we have a mechanism for showing that a big-$O$ bound could be tighter. What about big-$\Omega$ though?

Yes, and as you might expect, it's little-$\omega$.

We won't spend as much time on it, because if you understand little-$o$, you understand little-$\omega$, but for completeness sake, let's see the definition.

---

# Definition of little-$\omega$

$$
f(n)=\omega(g(n)) \iff \forall c \gt 0, \exists n_0 \in \mathbb{N}, \forall n \ge n_0,f(n) \geq c \cdot g(n)
$$

<br>

<div class="footnote">

Note: again, as usual, $f(n)$ and $g(n)$ represent times, so they are non-negative.

---

# little-$\omega$ practice

1. Prove that it is impossible that for some functions $f$ and $g$, that $f(n)=o(g(n)) \land f(n)=\omega(g(n))$?
  Hint: The easiest proof is probably to suppose that it's true and derive a contradiction. What does it mean for $\forall c, f(n) \le c\cdot g(n)$ and $\forall c, f(n) \ge c \cdot g(n)$? Remember that $f(n)\gt 0$ and $g(n) \gt 0$ was implied in our definition.
2. Prove that $f(n) = o(g(n)) \iff g(n) = \omega(f(n))$
   Hint: choose the same $n_0$, but apply the assumption to a different $c$
3. prove that $n = \omega(1)$. You can use your result from the little-$o$ practice and the previous answer here.


---

<!-- _class: invert questions -->
# Questions

---

# One more: big-$\Theta$

This one is super useful. Let's first look at the definition:

$f(n) = \Theta(g(n)) \iff$
$\exists C_1 \gt 0, \exists C_2 \gt 0, \exists n_0 \in \mathbb{N}, \forall n \ge n_0, C_1 \cdot g(n) \le f(n) \le C_2 \cdot g(n)$

<div class="footnote">

Note: as always, both functions are positive.

</div>

---

# Big-$\Theta$ (2)

Big-$\Theta$ is like a combination of big-$O$ and big-$\Omega$ at the same time. "$f(x) = \Theta(g(x))$ means that $f(x)$ and $g(x)$ differ by only a constant factor.

![bg right:55% height:100% a graph of Big-Theta. It shows that a weirdly oscillating sinusoid is contained between f(x)=(3/2)x and f(x)=(2/3)x, confirming that it is Big-Theta of x, despite its strange behavior.](big-theta.svg)

Notice how the wacky sinusoid is still contained between the two linear functions. Therefore, it's $\Theta(x)$. As x gets big, it's hard to distinguish it from a line.

---

# Why is it useful?

Because of this identity: $f(n)=\Theta(n) \iff f(n)=O(n) \land f(n)=\Omega(n)$
This follows immediately from using the same constants.

If I say $f(n)=\Theta(g(n))$, I have told you the big-O and big-$\Omega$ *at the same time*.

And there's one other reason why it's even more useful than just big-$O$ and big-$\Omega$ put together:

$f(n) = \Theta(g(n)) \implies \lnot(f(n) = o(g(n)) \lor f(n) = \omega(g(n))$

That is, $f(n) = \Theta(g(n))$ tells us not only that $f(n)$ is bounded by a constant factor of $g(n)$, but that this bound is *tight*. 

---

# Proof of big-$\Theta$ having a tight bound

$f(n) = \Theta(g(n)) \implies \lnot(f(n) = o(g(n)) \lor f(n) = \omega(g(n))$

suppose $f(n) = \Theta(g(n))$, by definition $\exists (C_1\gt 0, C_2\gt 0, n_0 \in \mathbb{N}), \forall n \ge n_0, C_1 \cdot g(n) \le f(n) \le C_2 \cdot g(n)$ \*

suppose we have some $C_1\gt 0, C_2\gt 0, n_0 \in \mathbb{N}$,
then we have the hypothesis $\forall n \ge n_0, C_1 g(n) \le f(n) \le C_2 g(n)$
we must show: $\lnot(f(n) = o(g(n)) \lor f(n) = \omega(g(n))$
Remember that $\lnot P$ is equivalent to proving $P \implies \mathrm{contradiction}$.

<div class="footnote">

\* We can combine "foralls" and "exists" together using tuple notation: $\exists(a, b, c, \ldots)$

</div>

---

# Proof of big-$\Theta$ having a tight bound (2)

So assume: $f(n) = o(g(n)) \lor f(n) = \omega(g(n)$
When you have an assumption of the form $P \lor Q$, you can break your proof into two subgoals, one where you assume $P$, and one where you assume $Q$.

So "suppose" $f(n) = o(g(n))$.
Let's show that assuming $f(n) = o(g(n))$ leads to a contradiction.

---

# Proof of big-$\Theta$ having a tight bound (3)
$f(n) = o(g(n))$ means $\forall c \gt 0, \exists n'_0 \in \mathbb{N},\forall n \ge n'_0, f(n) \le c \cdot g(n)$
and we have from our earlier assumption: $\exists (C_1\ge 0, C_2\ge 0, n_0 \in \mathbb{N}), \forall n \ge n_0, C_1 \cdot g(n) \le f(n) \le C_2 \cdot g(n)$

in the same way that we "suppose" a "$\forall$" in the goal and an "$\exists$" in an assumption, we "choose" a "$\exists$" in the goal and a "$\forall$" in an assumption.

Choose some $n' \ge n_0 + n'_0$
Choose $c={C_1 \over 2}$, from $c$ and $n'$ and the $o$ assumption, $f(n) \leq {C_1 \over 2} \cdot g(n)$
This implies that $\forall n \ge n'_0, C_1 \cdot g(n) \le f(n) \le {C_1 \over 2} \cdot g(n)$

---

# Proof of big-$\Theta$ having a tight bound (4)

$\forall n \ge n'_0, C_1 \cdot g(n) \le f(n) \le {C_1 \over 2} \cdot g(n)$
implies by transitivity, $\forall n \ge n'_0, C_1 \cdot g(n) \le {C_1 \over 2} \cdot g(n)$
and $g(n) \ge 0$, so this implies $C_1 \leq {C_1 \over 2}$
while implies $1 \le {1 \over 2}$, a contradiction. $\square?$

Not quite $\square$, we showed that $f(n) = \Theta(g(n)) \implies \lnot (f(n)=o(g(n)))$.

What about $\lnot(f(n) = \omega(f(n)))$?

---

# Big-$\Theta$ practice

1. We didn't finish the proof of big-$\Theta$ having a tight bound. We showed that $f(n) = \Theta(g(n)$ and $f(n) = o(g(n))$ were incompatible. Now prove $\lnot(f(n) = \omega(g(n))$

2. Prove this equivalence: $f(n)=\Theta(g(n)) \iff f(n)=O(g(n)) \land f(n)=\Omega(g(n))$

3. From 1 and 2, show:
$f(n)=\Theta(g(n)) \iff$
$f(n)=O(g(n)) \land f(n)=\Omega(g(n)) \land \lnot ( f(n) = o(g(n))) \land \lnot(f(n) = \omega(g(n)))$

---

<!-- _class: questions invert -->
# Questions?

---

# But we can't always use big-$\Theta$

Ideally, we'd always use big-$\Theta$ because of how much information it gives us.

But we *can't* always use it. Here are some facts about insertion sort:
- $T(n) = O(n^2)$
- $\lnot (T(n) = o(n^2))$
- $T(n) = \Omega(n)$
- $\lnot (T(n) = \omega(n))$

The $1^{st}$ two mean there are a $C_1$ and a $C_2$, $T(n) \le C_1 \cdot n^2$, but $T(n) \gt C_2 \cdot n^2$
The $2^{nd}$ two mean there are a $C_3$ and a $C_4$, $C_3 \cdot n \le T(n)$, but $C_4 \cdot n \gt T(n)$ 

From that, we get contradictions like $\forall n \ge n_0, C_4 \cdot n \gt T(n) \gt C_2 \cdot n^2$, which is clearly not true.

---

# Some algorithms don't have a general big-$\Theta$

And ain't that a shame; big-$\Theta$ is useful.

Is there some way we can redefine our problem so we can use it?

---

# Introducing cases

We can consider the *best*, *worst*, and *average* cases for an algorithm. 

This lets us acknowledge that there are certain kinds of inputs that will have very regular runtimes from the algorithm.

If we constrain our inputs like this, we can be much more precise with our bounds. So precise that we can usually use big-$\Theta$!

---

# Cases for insertion sort: best case

We know that if the list is sorted, the outer loop runs for every $n$ and the inner loop never runs. 

We already showed that this results in it being $\Omega(n)$.

However, if we say, ahead of time, "we're assuming the list is sorted", then it is also $O(n)$, because we know it will not go through the list more than once.

Therefore, if we say "best case" or "the list is sorted", we can say insertion sort is $\Theta(n)$.

---

# Cases for insertion sort: worst case

If the list is in exactly reverse order,
the inner loop runs $i$ times and the outer loop runs $n$ times.

We showed that this was $O(n^2)$, but now, because we specified explicitly that the list was out of order, we also know that it is $\Omega(n^2)$ as well.

Therefore it is $\Theta(n^2)$

---

# Cases for insertion sort: average case

This is the toughest one. What does average mean? There are different ways of defining it. The simplest is the expected time taken of a uniform-randomly chosen input.

This kind of analysis is tough and sometimes requires higher math. However in this case, we can observe that a randomly sorted list is likely to have some proportion of its pairs out of order.

This means that the inner loop will run a random, but proportional amount of iters. So $i/q$ iters for some $q$. This ends up being related by a constant to $n^2$.

So we can say, on average, insertion sort is $\Theta(n^2)$

---

# Practice

1. Is selection sort $\Theta(n^2)$ in all cases, or is there some case where it has a different bound? Support your answer.
2. What about linear search? 
2. Think of another algorithm that has no big-$\Theta$ in general, but does when you narrow the cases down.

---

<!-- _class: invert questions -->
# Questions?

---

# Switching gears

What is the worst-case big-$\Theta$ of this function?

```c
void ins_sort_rec(int* arr, size_t n) {
    if (n <= 1) return;

    ins_sort_rec(arr, n - 1);                   // Theta(???)
    int j = n - 1, t = arr[j];
    for (; j > 0 && arr[j - 1] > t; j--) {      // Theta(n)
        arr[j] = arr[j-1];
    }

    arr[j] = t;
}
```

---

# Recursive inseriton sort analysis

This is a fun one because in order to know the big-$\Theta$ of the recursive function, we have to know the big-$\Theta$ of the recursive function.

Let's start by showing the time function:
$T(0) = \Theta(1)$
$T(1) = \Theta(1)$
$T(n)= T(n - 1) + \Theta(n), \mathrm{if}\ n \ge 0$

Here, we're saying that running the function on the empty array or a singleton array just returns (which takes constant time). But running on $n$ means we first run on $n - 1$, which takes $T(n - 1)$, and then we have a loop that takes $\Theta(n)$.

---

# Don't bother with substitution

Obviously we could substitute if we wanted:
$T(n)= T(n - 1) + \Theta(n), \mathrm{if}\ n \ge 0$
$T(n)= T(n - 2) + \Theta(n) + \Theta(n), \mathrm{if}\ n \ge 0$
$T(n)= T(n - 3) + \Theta(n) + \Theta(n) + \Theta(n), \mathrm{if}\ n \ge 0$

But we're not getting anywhere. We *know* that there's an endpoint for any $n$ 

Whenever we see this kind of infinite substitution happen, where we know that if we had something to fill in for $P(n-1)$, we could show $P(n)$, that's a sign that we need induction.

---

# Deciding on the proposition

But induction is called "induction" because you have to determine the proposition before you do the proof. It's different from "deduction" where it just follows.

So how do we get an idea of what the worst case big-$\Theta$ should be?

It can be helpful to draw a diagram

---

![bg height:95% a diagram showing the time behavior of recursive insertion sort. There are 4 columns. One for i = 1 shows that one Theta of one comparison happens. Then, its neighbor, i = 2 shows that two Theta of one comparisons happen. Then, its neighbor, i = 3 has three Theta of one comparisons inside of it. Lastly, there is a column for i = 4+, showing more comparisons. The overall shape is triangular, justifying a quadratic number of comparisons.](ins_sort_rec_time.svg)

---

# The diagram

There is a column for each place in the array, and each column has that many comparisons in it.  

As a result, the shape is triangular.

And a triangle has half the area of a square, so we're justified in thinking that this will end up being $\Theta(n^2)$.

But how do we prove it?

---

# Induction for big-$\Theta$

Remember the time function:
$T(0) = \Theta(1)$
$T(n)= T(n - 1) + \Theta(n), \mathrm{if}\ n \ge 0$

First, induction requires a goal. That goal is that in the worst case: $T(n) = \Theta(n^2)$

Then, it requires two proofs. One for $P(0)$, and one for $\forall n, P(n) \implies P(n + 1)$

Let's start with $P(0)$

---

# Induction for big-$\Theta$ (2)

$P(0): T(0) = \Theta(0)$
But $T(0)$ actually is $Theta(1)$.

So...that's not true. We're not off to a great start.

We don't actually care about how long it takes at $T(0)$ though. We care about it as the time gets big. And big-$\Theta$ lets us pick whatever $n_0$ we want.

Let's modify our hypothesis: $n \ge 1 \implies T(n) = \Theta(n^2)$

---

# Induction for big-$\Theta$ (3)

Okay, one more shot:

Base case: $n=0$, show $0 \ge 1 \implies T(n) = \Theta(n^2)$
  Wait...what? Yes, this statement is true: vacuously true. Because $0$ is not $\ge 1$.
  If you recall to our principal of induction algorithm, it didn't care whether the proof of $P(n-1)$ was vacuous or not. True is true.

Remember that in classical logic, $P \implies Q$ is equivalent to saying $\lnot P \lor Q$. So $n \ge 2 \implies T(n) = \Theta(n^2)$ is the same as saying "either $n$ isn't big enough or $T(n) = \Theta(n^2)$. In this case, $n$ isn't big enough, which is fine.

---

# Induction for big-$\Theta$ (4)

Inductive case: $n \ge 1 \implies T(n) = \Theta(n^2) \implies T(n + 1) = \Theta((n + 1)^2)$  

Suppose $n \ge 1$ and $T(n) = \Theta(n^2)$
We must show $T(n + 1) = \Theta((n+1)^2) = \Theta(n^2 + 2n + 1) = \Theta(n^2)$

We were able to simplify all the way down to the expression on the right because big-$\Theta$ follows the same rules as big-$O$ and big-$\Omega$. Adding terms subsumes smaller terms.

$T(n+1) = T(n) + \Theta(n)$ by definition. By the inductive hypothesis, $T(n)=\Theta(n^2)$, so $T(n+1) = \Theta(n^2) + \Theta(n) = \Theta(n^2)$. $\square$

---



problems

1. 

---

# Example table of Big-$\Theta$'s


| Big-$\Theta$| Kind of problem 
|-----------|----------------------------------------------
| $1$       | simple machine operation (arithmetic on int, boolean expression eval., etc.)
| $\lg n$   | binary search, search tree traversal
| $n$       | linear search, many string operations, arithmetic on BigInts, counting sort
| $n \lg n$ | fast comparison sorts
| $n^2$     | slow comparison sorts, vector matrix multiplication, convolution
| $n^3$     | linear optimization, simple matrix multiplication
| $2^n$     | any operation on all combinations of something
| $n!$      | any operation on all permutations (orderings) of something

---

# Actual quiz next class

Next class we will have a quiz on this material.

This quiz counts! It's going to measure your understanding of this module.

**You must bring paper and a writing implement! This is your responsibility! Set six different reminders on your phone!**

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


# Source code for mermaid diagram

```mermaid
mermaid:
graph TB
  direction LR

  subgraph row4
    direction TB
    i4["i = 4+"] --> c41["Θ(1)"]
    c41 --> c42["..."]
    c42 --> c43["..."]
    c43 --> c44["..."]
  end

  subgraph row3
    direction TB
    i3["i = 3"] --> c31["Θ(1)"]
    c31 --> c32["Θ(1)"]
    c32 --> c33["Θ(1)"]
  end

  subgraph row2
    direction TB
    i2["i = 2"] --> c21["Θ(1)"]
    c21 --> c22["Θ(1)"]
  end

  subgraph row1
    direction TB
    i1["i = 1"] --> c11["Θ(1)"]
  end

  i4 --> i3 --> i2 --> i1 
  -->
```