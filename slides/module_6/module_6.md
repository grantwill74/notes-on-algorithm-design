---
marp: true
theme: slides
paginate: true
---

# Design and Analysis of Algorithms  

## Module 6: Complexity Categories

<br>
<br>

Slides © Grant Williams, [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/).  

<br>

This is an open educational resource.
Feel free to submit fixes, improvements, and new material [here](https://github.com/grantwill74/notes-on-algorithm-design).

---

# This week

This week we are going to talk about a very abstract topic: **complexity categories**.

A complexity category is a set of **problems** (not algorithms) that all share certain time bounds and characteristics.

Up til now, we have considered the bounds of *algorithms*. We have not yet considered that *problems* can be bounded too.

But why would we do this?

---

# Why bound problems

Bounding problems is useful because it lets us describe the essential difficulty of solving that problem.

For example, general sorting is $\Omega(n)$. E.g., counting sort is $\Theta(n)$.
*You cannot go lower than this and still see every element in the array*.

Does that mean that it's worthless to try to find a $O(\lg n)$ sorting algorithm?

No, it means it's worthless to try to find a $O(\lg n)$ sorting algorithm that makes no assumptions at all (general sorting). There are situations where you might know that only a couple of indices of a skip list are out of place, and where they are.

Bounding problems helps us know when it's more productive to adjust our assumptions or desires than to try to find an impossible solution.

---

# Grouping problems

It turns out, when bounding problems, we often group them into sets.

A very important set is $P$, which is the set of *decision* problems for which a deterministic turing machine that solves them in polynomial time is known.

What is a decision problem? Think of it as a problem that asks for a boolean solution.

Example of decision problems:
- Is this list sorted? (This one is in $P$)
- Is this a prime number? (In $P$ [as of 2002](https://en.wikipedia.org/wiki/AKS_primality_test))
- Is this the shortest path through this graph? ($\in P$)
- Is this the *longest* simple path through this cyclic graph? ($\notin P$)
- Is this matrix the product of these two matrices? ($\in P$)

---

# Motivation

Determining whether a list is sorted is $\Theta(n)$.
Multiplying two matrices together is $\Theta(n^3)$ 
(technically faster solutions are known, but the fastest is $\omega(n^2)$). 

Why group these problems together? They seem like one is way faster than the other.

Let's revisit our basic machine model. We decided early on to use the RAM model of computation, where basically every memory or register access (and therefore, machine instruction in general) counts as 1 time unit.

Consider 3 different machines...

---

# 3 machines

1. A modern x64 desktop with terabytes of RAM, a bazillion core CPU, and a sick RTX video card that gets the high score on whatever 3D Max benchmark is current.
2. A Nintendo Entertainment System (NES)
3. A computer someone made in Minecraft.

I hope you will agree that these machines are different in their performance characteristics.

It's not just that one machine is faster than another. Basic operations have fundamentally different time bounds.

For example...

---

# 3 machines (2)

Consider multiplication. For any reasonable multiplication need (not considering BigInts) for most algorithms, the x64 desktop's `imul` or `mulsd` instruction suffices.

This instruction is $\Theta(1)$. It's contant time.

Again, technically we could say "but what if the number was really big", but say we only want to multiply numbers that fit in a machine register. It's $\Theta(1)$.

---

# 3 machines (3)

But the NES has a processor based on the classic 6502 CPU. This processor does not have a multiply instruction. If you want to multiply two integers, you have to code that with addition.

The naive approach to multiply $n \times m$ is:
```c
int product = 0;
while (n > 0) {
    product += m;
    n--;
}
```

This naive approach is $\Theta(n)$. 

---

# 3 machines (3)

A faster approach is to use an algorithm that applies the distributive property to binary numerals. It treats 7 * 7 as 7 * (1 + 2 + 4), adding a power of 2 for every $1$ in the numeral.

```c
int product = 0;
while (m > 0) {
    if (m & 1) product += n;
    n <<= 1;
    m >>= 1;
}
```

This algorithm is $\Theta(\lg m)$. Because it takes different amounts of time depending on how many bits are set in $m$. It's not constant. We can't do constant time multiplication on the NES.

---

# 3 machines (4)

Minecraft is a popular video game in which you can explore and build in a world made of blocks. 

Some of these blocks, called redstone blocks, have special properties that allow them to turn lights and switches on and off, in response to other lights and switches. 

It turns out, the logic behind how this works is turing complete. Meaning that turing machines, and therefore programs, can be simulated inside this game.

---

# 3 machines (5)

However, using these blocks is [tricky and slow](https://minecraft.wonderhowto.com/how-to/build-simple-redstone-adding-machine-minecraft-0131892/).

The game looks at adjacent blocks and to slowly propagate the state forward. 

As a result, adding even two bits is somewhat slow; adding an entire integer is orders of magnitude slower than doing it in an NES.

![bg right:60% a screenshot of the game Minecraft depicting a single bit full adder circuit. There are roughly 15 blocks visible, with torches sticking out at odd angles and lots of wiring on the floor, indicating the high complexity of doing anything with this model of computation](redstone_adder.webp)

---

# 3 machines (6)

The NES can add reasonable-sized integers in $O(1)$, but multiplication is $O(\lg n)$.

Minecraft redstone computers cannot even perform addition quickly. Even adding two numbers is $O(\lg n)$, where $n$ is the larger number. The time depends on the number of bits in that number.

If we apply the clever multiplication algorithm we saw before, it relies on bit shifts and additions, both of which are $O(\lg n)$ in minecraft. Since we are performing an addition of $n$ and some shifts for each 1 in the $m$ operand, multiplication is $O((\lg n)^2)$.

If we had used the naive multiplication algorithm, and naive addition by increment, multiplication would be $O(n^2)$

---

# Computation in general

Changing from one kind of machine to another radically altered how long it took to do even basic things, let alone more complex operations.

However, one thing remained the same: multiplication takes a polynomial amount of time *as a problem*.

---

# Abstract computation

And we want to consider how long multiplication should take, but as a computer scientist. And a computer scientist should be able to work with many different media of computation:
- There are the digital electronic computers we know and love.
- There are [analog electronic](https://en.wikipedia.org/wiki/Analog_computer) computers, which have been useful in the past and which people theorize may be useful for implementing neural networks.
- There are [optical computers](https://en.wikipedia.org/wiki/Optical_computing) that use light for calculations
- There are [chemical computers](https://en.wikipedia.org/wiki/Chemical_computer), where chemical reactions do computation.
- There are even [biological computers](https://en.wikipedia.org/wiki/Biological_computing) (your brain counts as one).
- Really, [there are a whole bunch of computing paradigms](https://en.wikipedia.org/wiki/Unconventional_computing).

---

# Turing computation

But remember, turing machines are extremely simple. They are just collections of states and transitions guarded by the current state and the state of memory.

And in fact, there are many versions of the Turing machine. Some versions have RAM. Some verisons have variables. Some version have 2 tapes. Some versions have the tape as a circle. These have all been proven equivalent to one another.

So when we're talking about the difficulty of a problem, we really need to be abstract, because we could be solving that problem in all kinds of wacky media or turing machines with different capabilities.

And yet, we can still say something about it...

---

# Class-P is robust across models of computation

A decision problem is in $P$ if there is a deterministic turing machine (TM) which decides it in $O(n^k)$ time, for some constant $k$.

If you have an $O(n^k)$ algorithm on one reasonable model of TM, there is an equivalent algorithm that will run it in $O(n^{k + c})$ on another reasonable TM, for some constant $c$.

A TM model is *reasonable* if its state-changes and basic operations are simulable on a classical TM in polynomial time, and vice versa (no magic instructions).

It does not matter if you are using a classical Turing machine, a DNA computer, a redstone contraption, or an iPhone. Your polynomial time problem will have a polynomial time algorithm if you have a poly-time algorithm for any (reasonable) turing-equivalent computer.

---

# Questions?

<!-- _class: invert questions -->

---

# Classes other than P

There are many useful problems in P.

But there are many important problems we want the answer to for which there is no known solution that is in P.

What does that mean? There are three possibilities ...

---

# Classes other than P (2)

1. There *would* be a P solution if we magically knew which action to take at every step. Equivalently, if someone gives us a solution, we can verify it polynomialy. If we had the ability to try every possibility non-deterministically, we could solve it in polynomial time. The set of these problems is called **NP**.
2. Some NP problems are harder than others. Some NP problems can be reformulated into others in polynomial time, but others, we don't know of a way to reformulate them into simpler problems. These tough NP problems are in the set **NP-Complete**.
3. Some problems are so hard, that they are at least as hard as **NP-Complete**, but can be even harder. *Sometimes even checking that a solution is correct is hard*, so even using a magic oracle we cannot obtain a solution in P time. We call the set of these kinds of problems **NP-Hard**

---

# Put more simply...

1. P is the set of problems with deterministic polynomial time solutions ($O(n^k)$)
2. NP is the set of problems with polynomial time *verification* of a solution, whether or not there is a known algorithm that solves it in $O(n^k)$. It turns out that every problem in NP is $O(2^{n^c})$ on a deterministic TM where c is a constant.
3. NP-complete is the set of the hardest problems in NP. We'll explain why it's important to define this subset in a bit.
4. NP-hard is the set of problems that are at least as hard as NP-complete

Later, I'll show you a diagram of these sets. However, the reason I haven't done that yet, is it actually has to be two diagrams, because...

---

# P = NP?

We don't actually know that P and NP are different sets. No one has proven it.

NP is *definitely* a superset of P. That is, if we have a polynomial time solution that runs on a TM, then we definitely have a polynomial time solution that runs on a non-deterministic TM (abbreviated NTM).

An NTM is just a TM that can magically predict what to do next. So if we're polynomial time *without* that magic power, giving it to us does not slow us down.

Therefore, all problems in P are also in NP.

---

# P = NP? (2)

However, we don't actually know whether all problems in NP are in P.

We can't really say that a problem in NP is *not* in P. Maybe we just haven't figured out how to solve it yet in polynomial time.

There might be a polynomial-time *reduction* from the problem to a problem in P.

*Reduction* means converting one problem into another, such that solving the other means solving the one.

---

# Reduction

For example, I can *reduce* the problem of determining whether a given number is the product of two other numbers to the problem of determining if a matrix is the result of multiplying two other matrices. I just encode the two integers as if they were both $1\times 1$ matrices, and apply my solution for matrices. Then I convert back.

I'm just re-encoding a simple data type twice, so this reduction takes $\Theta(1)$. 

Conversely, the problem of testing a product of square integer matrices *can also be reduced* to the problem of multiplying integers. For every $n\times n$ integer in the second matrix, multiply it by the $n$ integers in each column of of the first one, and do some additions.

This particular reduction (there are others) takes longer: $\Theta(n^3)$ multiplications need to happen. **But it is still polynomial time.**

---

# Alchemy of reduction

You notice that our reduction of matrix multiplication to scalar multiplication is pretty much how we usually mutliply (small) matrices. 

But sometimes we have a choice of different reductions. Example, my problem is that  I want to know if an element is contained in a list?
- One reduction involves sorting the list first, typically $\Theta(n \lg n)$ for comparison sorts, and then solving the problem "is this element in this *sorted* list", which can be solved in $\Theta(\lg n)$.
- Another reduction involves building a hash-set from the list $\Theta(n)$ and then asking the question, "is this value contained in this hashset", with expected time $\Theta(1)$.

---

# Alchemy of reduction (2)

As programmers, we have flexibility in which reductions we can perform.

There are a huge number of ways to re-imagine a problem. Doing so can give you more insight into both problems, but it can also offer a new solution.

So don't just think about what you're doing as solving a problem. Try to think about it as reducing one problem into another that you have a solution for. It's a kind of alchemy of problem solving.

---


# Questions?
<!-- _class: invert questions -->

---

# A little warning about problems

Before we continue, I feel like I should highlight something.

In this lecture we're specifically talking about P and NP, which contain *decision problems*.

Many of the problems we solve as programmers are not decision problems. For example, computing the product of two numbers is a function problem, not a decision problem. 

But determining whether a given product actually *is* the product is a decision problem. 

Practically computable function problems live in the sets FP and FNP. 

Many of the theorems that apply to P and NP *do not* apply to FP and FNP. It's important to keep them straight! Keep me honest too, if I give a problem in the wrong set!

---

# Reduction basics

To say that A reduces to problem B in polynomial time, on a deterministic TM, we write:
$$A \le_m^p B$$

This is confusing, because if we *reduce* A to B, it sounds like A must be *bigger* than B, otherwise how could we reduce it? And yet we use $\le$.

But really, this notation is about difficulty. If we can reduce A to B in polynomial time, then we know either that A is in the same class as B, or an easier class. For example, a polynomial is less than an exponential as $n$ goes to infinity.

The $p$ means "polynomial time". We don't care about reductions that are so slow we don't gain anything by doing them. The $m$ means many to one (as opposed to many-to-many). It means that you can't generate lots of reductions and ask an oracle to choose the best one. We will create one instance of B for each instance of A.

---

# NP completeness

For this class, assume that $A \le B$ is equivalent to writing $A \le_m^p B$ for problems A and B. We're going to assume that reductions are polynomial time and deterministic unless I say otherwise.

Suppose that the problem of checking scalar multiplication is called MUL.
And suppose that the problem of checking matrix multiplication is called MATMUL.

We saw that MUL $\le$ MATMUL, and MATMUL $\le$ MUL, implying that they are "equally" hard (and they are: they are both in P).

They aren't literally equally hard. Matrices take longer to multiply than scalars. But they are both polynomial time; they are in the same set.

---

# NP completeness (2)

NP is an important category because it contains all the problems that we could theoretically solve in polynomial time if we were smart enough or had enough threads.

There is an interesting problem in NP called **SAT**. 

This is the boolean satisfiability problem. It is the problem of, given some boolean formula like $a \lor b \land (\lnot b \land c)$, is there some set of booleans that satisfies it? I.e., are there some true/false values we can fill in for $a$, $b$, or $c$ that makes the expression true?

In this case, yes. $a$ = true, $b$ = false, $c$ = true.

This is the only solution, but sometimes there are more than one, and sometimes there are zero. The problem is to determine if at least one solution exists.

---

# NP completeness (3)

In 1971, computer scientist Stephen Cook had [an important realization](https://en.wikipedia.org/wiki/Cook%E2%80%93Levin_theorem). He realized that *every problem in NP could be reduced to SAT*.

First, how do we know that SAT is in NP? Let's imagine you have some complicated boolean formula with $n$ terms.

Determining if there is a solution is hard. We have to potentially consider every assignment of true or false to every variable, and there can be as many variables as terms.

In principle, this is $O(2^n)$, because there can be up to $n$ variables, and there are 2 possibilities per variable.

---

# NP completeness (4)

But, given a solution, determining if it is correct is very easy. We just fill in the trues and falses and evaluate all the operators. This takes linear time in the number of operands.

So on a non-deterministic turing machine, SAT is in NP. It has a polynomial time solution if we have the ability to try every combination at the same time.

Okay, SAT is in NP. But why would we care?

---

# Everything in NP can be reduced to SAT

The Cook-Levin theorem, named after Stephen Cook and Leonid Levin, a Soviet (at the time) computer scientist who independently discovered the same things as Stephen Cook, states:

*Any problem in NP can be many-to-one reduced to SAT in polyomial time*

In other words, $\forall X \in \mathrm{NP}, X \le \mathrm{SAT}$

Why? What's so special about boolean satisfiability?

---

# The SAT reduction

Imagine we have a SAT solver. SAT solvers are not things that only exist in theory: [people compete every year to make the fastest one](https://satcompetition.github.io/).

We can give a boolean expression to our SAT solver, and it will tell us whether it has a solution or not. In practice, it will also tell us the solution, but to be strictly a decider, it is only required to output "yes" or "no".

Cook and Levin's goal was to convert *any* decision problem in NP into a SAT problem. For a problem to be in NP, there must exist a Turing Machine (deterministic or not) that solves it in polynomial time.

So we must assume that machine exists, and convert it into a SAT problem somehow.

---




---

# P = NP? (2)

---

# Practice exercises

---

# Why don't we care as much about space?

---

# Cheat sheets

--------

# Some classic reductions (and "gadgets")

---

# SAT $\to$ 3SAT

---

# 3SAT $\to$ CLIQUE

---

# CLIQUE $\leftrightarrow$ VC $\leftrightarrow$ INDSET

---

# Subset-sum / partition

---

# Beyond NP: NP-Hard

---

# How I will generate quiz questions

---

# A bunch of quiz questions



# The gadget

When doing reductions, we often compose [*gadgets*](https://en.wikipedia.org/wiki/Gadget_(computer_science)). A gadget is a piece of a turing machine that maps one piece of a problem into another.

It turns out, there is a really complicated gadget 
