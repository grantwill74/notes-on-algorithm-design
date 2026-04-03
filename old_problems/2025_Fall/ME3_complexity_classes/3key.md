# Key

1. Yes. These expressions indicate that the virtual machine has the same value at two points in time, which does not violate any constraint.

Grading: all or nothing for yes/no. For the explanation, it should be clear that you know that the $S$ refers to the state of the tape and what $t$ and $i$ mean.

2. P. Because we used Dijkstra's algorithm or A\* to solve it on a determinstic machine, and the algorithm runs in $O(|E| + |V|\log |V|)$, which is bounded by a polynomial.

Grading: Full credit for P, partial credit of +5 pts for NP (not specific but not otherwise incorrect), +2 pts for EXPTIME (not specific, not incorrect, but a weird answer). No credit for the others.
For the explanation: should indicate that you created a polynomial time algorithm to solve it. 

3. No. To conclude that a problem is in NP, a polynomial time reduction to a problem in NP is sufficient, but an exponential time reduction does not tell us anything.

Grading: all or nothing for yes or no. The explanation just has to mention that an exponential time reduction is not good enough (or suggest that it would need to be polytime)

4. It has size 100. The fact that the adjacency matrix is filled in means the graph is fully connected. Therefore, the whole graph forms a clique. 

Grading: all or nothing with the number. The explanation needs to indicate that we know the graph is connected or something to that effect.