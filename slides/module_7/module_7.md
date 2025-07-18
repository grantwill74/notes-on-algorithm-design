---
marp: true
theme: slides
paginate: true
---

# Design and Analysis of Algorithms  

## Module 7: Greedy Algorithms

<br>
<br>

Slides © Grant Williams, [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/).  

<br>

This is an open educational resource.
Feel free to submit fixes, improvements, and new material [here](https://github.com/grantwill74/notes-on-algorithm-design).

---

# Last week

Last time we learned about complexity categories.

One category was particularly important: NP-complete.

NP-complete is full of useful, interesting problems:
- SAT: useful for package managers, AI, among many other things
- TSP: useful for home delivery and machining
- VC: useful for security systems, city management, many other things

Solving these problems is massively useful for many important, real-world things.

The problem is that everything in NP-complete is exponential time!

---

# This week

But sometimes life throws us a bone: just because the general version of those problems is hard, does not mean that every single instance we encounter is hard.

For example, the TSP is hard, but what if our graph is a rectangular grid of tiles with uniform edge weights? Then it's easy: just follow a lawn-mower pattern.

What makes TSP hard is that normally, choosing to visit a city next means we can't visit it from a city later, which might have a shorter path. This means the decision of which city to visit first requires trying it and seeing how it works out, and then potentially revising our choice later.

---

# Greedy algorithms

Greedy algorithms are those in which we can get an optimal result with locally optimal decision making.

It would be like if we could solve TSP by always visiting the shortest distance (which doesn't normally work).

Greedy algorithms arise when there's some aspect of the problem that makes a simple strategy really effective. 

Let's consider video games...

---

# NP-complete game problems

The following problems are NP-hard (probably, I haven't proved it or anything):

Deciding the optimal place for districts in Civilization 6. Districts are little buildings you put down that have an effect on neighboring tiles. Going for the highest immediate bonus is not always optimal, because the district you put down blocks other districts.

Deciding the optimal build-order in a well-balanced RTS. Every building you build ends up changing the resources and strategies available, which may affect future decisions.

Determining the optimal chess move. Some moves are better than others, but determining the ideal move currently requires considering all the opponents moves they could make in response.

These are hard problems, but what if the game were not well balanced?

---

# Greedy algorithms for fake NP-complete games

Civ6: Imagine if there were a district that gave the bonuses of every other district. Now you just have to only build that one. There are no longer any tradeoffs.

RTS: Imagine if one building gave the benefits of all the other buildings and cost the same as the cheapest one. Now spamming that building is the optimal strategy.

What if we're playing a [fairy-chess](https://en.wikipedia.org/wiki/Fairy_chess) puzzle where there's a piece that can move anywhere and cannot be taken? Just only move that piece.

Notice how if one move becomes "overpowered", the search space for optimum strategies becomes very small and predictable.

That's the essence of a greedy algorithm. It's one that solves what seems like a hard problem, by solving a simple problem over and over again.

---

# Questions?
<!-- _class: invert questions -->

---

# An example problem

Suppose Alice and Bob are playing " Simple War" with $N \gt 1$ cards.

The cards each have a number on them from $1$ to $N$, with every number being used exactly once. 

The cards are randomly partitioned between Alice and Bob. 

Alice and Bob play cards at the same time. They can play any card in their hand. 

The highest card wins the play. The winning player takes that card and the card they played and add both to their hand. They continue until a player has won all the cards.

What is the optimum strategy? [Let's think about it first.]

If both players use it, can we determine who will win? And can we bound the time it takes?

---

# Simple war

At first, this seems like a combinatorial problem. Each player has a bunch of cards. Which one do they play? Maybe playing a good card now means we can't play it later...

Except if you win, you get your card back plus the opponent's card. So winning means you gave up nothing. And now you are in a stronger position.

And you maximize your chance of winning by playing your best card.

So the ideal strategy is to always play your maximum card.

And the winner will be whoever has card number $N$. Determining this takes $\Theta(N)$ if we are given a list of cards, or $\Theta(1)$ if we are told the number of cards at the beginning.

---

# Simple war induction

How would we prove this ideal strategy? You guessed it: induction!

But this time, let's do induction over a set of a particular type. Suppose a set of cards is constructed like this:
- $\emptyset$ is a set of cards.
- If $H$ is a set of cards and $c$ is a card, $H \cup { c }$ is a set of cards.

This type is inductive because it has a finite number of constructors. We can use induction on this type to prove statements that begin with $\forall H \in \mathrm{Hands}$

We could just do induction over the size of her hand, but it's a little cleaner sometimes to do induction directly on data structures.

---

# Simple war ideal strategy

We have two cases for induction: one for each constructor. Our goal is to prove that for any hand $H$, playing the largest card is optimal (i.e., that no other strategy is better).

- Suppose that Alice's hand is empty. Then she has lost; every strategy is identical.

- Suppose that for $H$, the optimum strategy is $\max H$.
  We must show that for some card $c$, $\max(H \cup \{ c \})$ is the optimum strategy
  There are 3 possibilities for $c$, 2 of which are valid:
  1. $c \gt \max H$. By the inductive hypothesis, playing $\max H$ was optimal, so playing an even higher card does not lose against any additional hands. 
  2. $c \lt \max H$. Then $\max(H \cup \{ c \}) = \max H$, and the goal follows from IH.
  3. $c = \max H$. This is impossible because the cards are distinct

---

# Who will win?

If our goal is to determine the winner, then it's whoever has the card numbered N.

How long does it take to do this? If we are given Alice's hand $H$, we must scan through it until either we find $N$ (meaning she wins), or until we reach the end (she loses).

Therefore, in the worst case, it is $\Theta(|H|)$

In this case, the fact that we defined our algorithm by linear search lets us re-use the big $\Theta$ for linear search. 

---

# Questions?
<!-- _class: invert questions -->

---

# A slight modification

Suppose the game has two rule changes:
- Instead of both players playing at the same time, Bob plays first.
- Now, N is the best card...except for 1, which beats N and absolutely nothing else.

So the questions are:
- Is there still a greedy algorithm for Alice that is optimum?
- Can we predict who will win?

We're still assuming both sides play optimally.

[What do you think?]

<div class="footnote">

inspired by [this CodeForces problem](https://codeforces.com/problemset/problem/2104/C).

</div>

---

# It's still greedy

The key here is that Bob goes first. So Alice can follow this plan:
- If Alice has N and 1, she plays N every hand regardless of what Bob plays.
- If Alice has N and some other card, but not 1, Alice still wins:
    - If Bob plays 1, Alice plays her other card.
    - If Bob plays another card, Alice plays N.
- If Alice has $N - 1$ and 1, she still wins:
    - If Bob plays N, Alice plays 1
    - Otherwise Alice plays $N - 1$
- Otherwise, Alice loses
    - If Alice only has N Bob will play 1.
    - If Alice does not have N or $N - 1$, Bob will play $N-1$ every hand.

---

# The simplest statement of the rule

Alice wins if she has either:
- N and something else: i.e., $N \in A, \land |A| \gt 1$
- $N - 1$ and 1: i.e., $\{N - 1, 1\} \subseteq A$

---

# Complicated rules

Notice that sometimes the greedy strategy can be a little complex.

In this case, the slight change in rules made the ideal strategy require some adjustment.

One way of thinking of greedy algorithms is that they  are an "exploit" for a game. 

Think back to games you may have played, when there was an overpowered strategy. Often, the developers "patch" the exploit, but then there is a slightly more complicated strategy that starts working. It's kind of like slowly moving the game from polynomial time to NP-complete by gradually removing greedy strategies.

Most good pure-strategy or puzzle games (pure meaning there's no reflex component) seem like they are NP-complete: hard enough to be deep and not have an obvious exploit, but not so hard that the game cannot be solved with good pattern recognition.

---

# Questions?

<!-- _class: invert questions -->

---

# Another variant

Alice and Bob get bored of playing games that seem like they always have easy solutions. So they try to make it more complicated again.

Bob proposes a rule change:
- Go back to playing simultaneously 
- Instead of playing one card at a time, players can play any number in a trick.
- The winner of the trick is the one with the largest sum.
- The winner keeps all the played cards from both players.

If you risk a lot of cards, you're in trouble if you lose. On the other hand if you don't risk enough, you lose the trick.

[What do we think? Any ideas if there even *is* an ideal strategy?]

---

# Yup, it's still greedy

Play every card you have every trick. If both players play optimally, the winner is whoever has the highest sum.

Let's prove it...

Base case: If you have zero cards, you've lost. So any strategy is the same.

If playing all the cards is optimum, consider if you had one more card. Should you play it or hold back?

The key insight: winning a trick makes you strictly better off. If you lose, you are in a strictly worse position. Therefore, if someone can guarantee they win a trick, they should, and they should do it over and over again.

---

# Yup, it's still greedy (2)

- If you hold back, you might still win, in which case it didn't matter, but playing it doesn't hurt either.
- If you hold back you might lose. You either would have lost anyway, or you lost a trick you could have won if you hadn't held back. So playing the extra card is either equal or better for that trick.
- If you don't hold back, you maximize your chances of winning. If you don't win with all your cards, you never could have won that trick, and you will be even weaker. If you do win, you lose nothing.

You might think "but wait, if I hold back, at least I don't lose that card!". However, if your opponent won, they are in a strictly stronger position. So if that card mattered, then you should have played it, and if it didn't, you are still guaranteed to lose now.

---

# Questions?

<!-- _class: invert questions -->

---

# One more try

Alice and Bob brainstorm to figure out what keeps going wrong in their game designs.

It feels like the fundamental issue is that winning a trick just makes you better. You don't have to give up anything.

Is there some way to make this no longer be true?

[Weigh in: any rule changes we want to consider?]

---

# One more try (2)

One option to improve the game would be to remove the cards played from further play. I.e., you don't get to pick up all the cards you just played and use them again.

The game proceeds in tricks as before. Both players play simultaneously, and play as many cards as they want.

Both players play with identical decks, so there isn't an advantage to having certain cards. Any card can still be played at any time.

The winner of a trick is whoever's sum is higher. If the sums are equal, no one wins.

---

# One more try (3)

You are required to play at least one card, and the game ends when one player finishes playing every card in their deck.

Your final score is the sum of the cards you won and the cards you didn't play. The sum does not include the cards you played.

So now you are not *strictly better* by playing high cards. If your opponent was going to play a 10, your best play would have been to play an 11. Playing all your cards will get the same result if you have > 10 total sum, but then you will not be able to win any further tricks. 

---

# Is there an exploit?

First, let's consider this ruleset. Each of the rules I added was because of an exploit:
- If you got to count cards you played for your score, it would go back to being smart to play all your cards. You either win the first trick or you draw. If you win, the game is over and you will have more points. If it's a draw, you would have lost if you hadn't played all the cards.
- If you didn't get to count the cards you won, but only the ones in your hand, your incentive would be to play one card at a time. You either draw or win with this strategy.

---

# Is there an exploit? (2)

- If you could play zero cards, that would then be ideal. The opponent would not get anything for winning tricks. Both players would cross their arms and refuse to play.
- If players did not play simultaneously, the best move would always be to add 1 to the opponents play if available, and your smallest card if you couldn't.


---

# A fragile game

However, I can't *prove* to you that there is no greedy algorithm that would still work here, without "solving" the game. 

And the fact that I had to "patch" the game so many times should make us wonder: is there really no way to greed? Why should we be sure this time?

There's no obvious answer. You might say "well, greedy algorithms are always polynomial time, so prove there can't be one for this algorithm". But for NP-complete games, proving that there's no polynomial-time algorithm would be akin to proving P $\ne$ NP. 

---

# Game theory

Ideally, we end up with a game in which there is no obvious one move...

Which means that we have to try to guess what our opponent will do...

Which means that any "bias" by our opponent becomes something to exploit. Imagine playing rock-paper-scissors against someone who had a higher than $1/3$ chance of picking rock. You would win in the long run by shifting your picks toward paper.

However, especially when a move that seems good now ends up hurting us later, we have to consider strings of moves. We start to feel the exponential nature of the move space. Think about how Chess is still unsolved despite enormous resources being poured into it.

But game theory is outside the scope of this class. Just remember that when you're looking for a greedy algorithm, you often don't have to use it!

---

# Questions?
<!-- _class: invert questions -->

---

# One lens to view these card games through

The card games with greedy algorithms actually have a fundamental property. A kind of "greedy essence" that tells you right off the bat that they are greedy if you notice it.

This essential property is the decision we have to make can be encoded as a [matroid](https://en.wikipedia.org/wiki/Matroid).


---

# What is a matroid?

Matroid is a scary sounding word, but "-oid" as a suffix just means "like"

So it's something that is like a Matrix.

In what way? In the way that it describes something that is linearly independent.

And really, it is this independence that is kind of a first step that makes greedy strategies a possibility for a problem.


---

# Linear independence

A linear system is one in which the variables don't depend on each other. If we have two variables, taking more of one does not require us to take more (or less) of another.

In the case of many greedy problems, this is the key. If there isn't some drawback to increasing one variable, we can just always do it. If your goal is to maximize $x$, and $x$ does not affect $y$ or $z$, then your decision is easy: ignore $y$ and $z$.

All matrices are matroids, but there are matroids that aren't matrices. There are lots of ways to define a matroid, but there is one that works well for many greedy problems and which doesn't require higher math: the set theoretic definition.

---


# The set definition of a matroid

A matroid is defined by two sets: $E$ and $I$.

$E$ is the set of **E**lements. These are usually the "things" that you can take or not take; or play or not play.

$I$ is the set of **I**ndependent subsets. Each element of $I$ is a set which consists of zero or more elements of $E$. These sets represent combinations of choices in the game. $I$ must have the following properties:
- $\emptyset \in I$
- "heredity": $A \in I \implies B \subset A \implies B \in I$
- "exchange": $A, B \in I$

---

# Matroids for Alice and Bob's simple card game

Recall that card game where both Alice and Bob could play as many cards as they wanted, and the winner got to keep all the cards they played as well as the cards they won.

Suppose there were 10 cards in Alice's hand. Then $E = \{1, 2, 3, 4, 5, 6, 7, 8, 9, 10\}$

Then $I$ is the set of all $2^{10}$ combinations of moves that Alice can make.

E.g., $\{\} \in I, \{1\} \in I, \{2\} \in I$, $\{1, 2\} \in I, \{1, 2, 5, 7, 10\} \in I$, etc.

Basically, Alice can play any combination of the values of $E$.

Technically she can't play the empty set according to our rules, but we can treat $\{\}$ as forfeiting the game or something. 

---

# Is this a matroid?

We've given $E$ and $I$. Now we must show that $I$ obeys the heredity and exchange properties:
- Heredity: If $A \subset B$, and $B \subseteq I$ is a valid move, then $A$ is a valid move. Is that true? This is a fancy way of saying, if playing a set of cards is a move, then playing fewer cards is also a move. This is true: the game does not require that we play a certain minimum number of cards.
- Exchange: If $A, B \in I$ and $|A| \gt |B|$, then $\exists a \in A, \{a\} \cup B \in I$
  Meaning: suppose there are two hands that Alice can play: $A$ and $B$. If $A$ has more cards, then there's a least one card that we could copy from $A$ and add to $B$ to make it even bigger, and the result from doing that would 

So the moves available to Alice form a matroid. Now: what's the best move?

---

# Rado-edmonds

[There is a theorem](https://link.springer.com/article/10.1007/BF01584082), attributed to Jack Edmonds and Richard Rado, which tells us that optimization problems that can be encoded as matroids have greedy solutions.

In this case, the problem is: "what is the largest value play". 

As you would expect, the answer is "the one with the most cards". There's nothing groundbreaking here, but stating it in the language of matroids gives us a way to quickly validate that a greedy strategy is even available.

In this case, we recognize that we can assign a weight to each set of cards Alice can play, and that adding more cards always increases that weight.

---

# Still need induction

Encoding the decision as a matroid helps us know what the "greedy" solution of the decision is.

But it doesn't prove that a greedy algorithm is optimum. We still need 

---

# Questions?
<!-- _class: invert questions -->

---

# Can we make a matroid?

Alice is learning a new skill. It could be anything: writing, math, Street Fighter 6.

Alice's skill level at whatever this is is $s$, which is a natural number.

She knows that to get better at a skill, you need to challenge yourself with little tests. Therefore, she is on the lookout for skill challenges.

Each challenge has two skill ratings: $s_{lo}$ and $s_{hi}$. Alice can benefit from the challenge if her skill is between $s_{lo} \le s \le s_{hi}$. Afterwards, she will gain one point of skill.

If $s \lt s_{lo}$, the challenge is too hard, and Alice cannot complete it. 
If $s_{hi} \lt s$, the challenge is too easy, and Alice does not gain benefit from it

<div class="footnote">

This was inspired by a PACNW regional problem that I can't find. It involved a student either choosing to study or not based on the skill range of a problem. Please submit a PR if you find it!

</div>


---

# Another problem (2)

The input to the program is a list of pairs: $s_{i, {lo}}$ and $s_{i,{hi}},$ for each challenge $i$. 

Alice considers the list in order, and for each challenge, she can choose to skip it, or perform it. If she skips it, it doesn't come up again. It's perform or skip for every problem, once.

If she is eligable to perform it, she gains one point of skill, but now may be ineligible to perform some challenges later.

The goal is to compute the maximum skill rating $s$ that Alice can achieve. You can look into the future at all the challenges that will appear, and advice Alice such that she maximizes the number of skill points.

---

# Another problem (3)

Here's an example, suppose this is an input:
```
0 10; 1 10; 2 10
```

In this case, there are three challenges with ranges 0 to 10, 1 to 10, and 2 to 10.
Alice should take all three. She ends up with 3 points of skill. She does not benefit from skipping.

Here's another example input:
```
0 10; 0 0; 1 1
```

Here, if she takes the first challenge, the second is off limits. But if she skips the first challenge, she can take the second. Either way, she should take the 3rd. 2 is the answer.

---

# Another problem (4)

So the question is: how do we do this? What is an algorithm that is both optimal (we get the maximum number of skill points) while also being as fast as possible.

Is this NP-complete? Harder? NP-inter? P?

[what do you think?]

---

# This is greedy and P time

Alice should always take a challenge if her skill is within the range.

Always. She should *never* skip a challenge.

First, if you agree, try to formulate why.

If you disagree, try to find a counter example.

---

# Proving the optimality of the greedy strategy

The reason the greedy strategy is correct, is that even if taking a skill challenge blocks off later challenges, the benefit is limited to the number of challenges we skipped.

For example, consider this input: `0 0; 0 0; 0 0`

Let's consider all possibilities:
- Skip all of them. Obviously the worst choice.
- Take one, skip others.
- Skip one, take two, skip three.
- Skip one and two, take three.

All the possibilities give the same skill.

---

# Inductive proof

Let's do a proof over a list. A list of some type T has two constructors:
- `[]` is a list.
- If `t` is a list, `h : t` is a list, where `h` is an element of type T, and `:` means "cons" (i.e., create a new list with the given element as its head).

Suppose we have a proposition that starts with "for all lists of T". We can prove it by showing that the proposition is true of the empty list, and that if the proposition is true of some list, it's still true if we add a random element to the front of the list.

In this case, our list is a list of pairs: $S_{i, lo}$ and $S_{i, hi}$

---

# Inductive proof (2)

Our goal is to show that always taking a challenge is optimal. That means that another strategy isn't strictly better (although it could be equivalent).

For the base case: if the list is empty, any strategy is optimal.

For the inductive case: suppose that always taking a challenge is optimal. Now we have a new challenge, $s_{lo}$ and $s_{hi}$. How can we show that we should take it?

---

# Inductive proof (3)

- If we take it, we block off any challenge with $s_{hi} \lt s$
- If we don't take it, we gain access to at most one challenge that we otherwise wouldn't have had, because it would have been too easy. 
  Why at most one challenge? Because if there are two challenges with $s_{hi} = s$, then after taking one, we will be ineligible for the other.

Therefore, skipping a challenge gives us, at most, access to one more challenge. Therefore, at best, it is an equal strategy because we skip one and gain one.

At worst, we lose access to challenges that had a minimum rating that we no longer meet.

Therefore, always taking the challenge is optimal.

---

# Questions?
<!-- _class: invert questions -->

---
