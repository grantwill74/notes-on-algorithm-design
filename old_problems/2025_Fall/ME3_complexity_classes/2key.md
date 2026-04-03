# Key

1. Yes. Those variables being true indicate that at time $t$, there are two adjacent cells on the tape with different values. There is nothing inconsistent about that.

Grading: All or nothing for the yes/no part. For the explanation part, some mention must be made of what $S$ means, and the interpretation should be consistent with what we know about time $t$ and index $i$.

2. There are three good answers here:
    1. A pair in which neither problem is in P.
       Explanation: showing P = NP requires reducing an NP-complete problem to a P problem.
    2. Alternatively, show any reduction that is slower than polynomial time. E.g., IsSorted $\le_\mathrm{exp}$ TSP.
       Explanation: the reduction must happen in polynomial time. An exponential time reduction would mean we can't practically solve the problem in P. 
    3. Or both.

Grading: any of these gains the full 10 points, otherwise 0. For 1, the explanation must indicate that you understand the specific criteria to prove P = NP, and it involves NP-complete. For 2, the explanation must include that the reduction is too slow (i.e., not polytime).

3. No. This algorithm runs in $O(2^n)$, which is not polynomial time on a deterministic machine.

Grading: all or nothing for the yes/no. For the explanation, should either show the time bound, or relate combinatorial problems to NP-complete or EXP-time or something.

4. If you draw the graph, you'll see that it looks like a triangle with an extra vertex connected to one of the points of the triangle. We can put the extra vertex in the indset, and then one of the vertices it's not connected to can also go in the set, so the size is 2.

Grading: 10 points for getting the size right, or 9 points if the diagram is wrong but the indset is correct for the diagram. Otherwise 0 points for that part.

A diagram should show a triangle with an extra node connected to one of the points of the triangle. Alternatively, a description to this effect.