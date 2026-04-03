# Key

1. No, because it would mean that the tape at location $i$ and time $t$ has two different values, and we would fail the single value constraint.

Grading: +10 for saying "no". +15 if the reasoning mentions the tape, allowing only one variable to be true, the regularity criteria, or something like that.

2. Sum is even: a trivial decision problem in which the goal is to say "yes" if the sum of a list of numbers is even. This problem is in P, which is why reducing clique (an NP-complete problem) to it would prove P = NP.

Grading: The given problem must be a decision problem in P and not one of the two examples given. No partial credit. The answer must state that it is in P. No partial credit given.

3. We cannot conclude that A is NP-complete, but we can conclude that A is NP. The reason for the second is that every problem that can be reduced to TSP is in NP. TSP is NP-complete and therefore is in NP. NP is closed under polynomial time many-to-one reductions. But for the first, demonstrating that does not make A NP-complete: we would need to show that $TSP \le_p A$ or equivalent.

Grading: The two 5 point answers are all or nothing. For the explanations, it should mention that we already knew that A is NP but not that an NP complete problem is reducable to A.

4. It tells us that the largest indset has 90 nodes ($100 - 10$)

Grading: -5 for off-by-one, otherwise no credit.
