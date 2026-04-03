# Key

# 1
`{ position, focuses, moves_remaining, _effort_expended_ }`

Grading: -5 for every one of those missing. -5 for every unecessary value. -5 for not marking effort expended as the one we sort by (not moves remaining).

# 2

If `moves_remaining = 0`, that is our destination state. We found the lowest effort expended way to reach there.

For every position `j` not equal to `i`, there are the following neighbors:
- `{ j, focuses, effort_expended + adj[i][j], moves_remaining - 1 }`
- `{ j, focuses - 1, effort_expended + adj[i][j] / 2, moves_remaining - 1 }`

The later is only valid if `focuses >= 1`

Grading: Needs to show both with focus and without (-15 if missing one).
-5 for smaller errors (forgetting to divide by 2, forgetting to subtract 1, etc.)

# 3

Yes, it is consistent.

Grading: all or nothing.

# 4

Let s be a state, and s' be a neighboring state.
Consistency requires: $h(s) \le c(s, s') + h(s')$

`m` is defined such that it is `<=` every cost.
Entering a neighboring state decrements moves remaining.
Therefore we have
- `moves_remaining * m / 2 <= cost + (moves_remaining - 1) * m / 2` ===
- `moves_remaining * m / 2 <= cost + moves_remaining * m / 2 - m / 2`

Because we chose `m` to be the minimum cost, and because `m/2` is the least cost we can pay (if we focus), `cost >= m / 2`, making the inequality true.

Grading:
-10 pts: stated the definition of consistency.
-20 pts: demonstrated how the inequality holds when substituting in the cost
-10 pts: no other errors

