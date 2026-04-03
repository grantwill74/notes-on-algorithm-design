# Key

## 1

A state looks like this:
```
{ row, col, health, moves }
```
We can look up whether we're adjacent to a monster, we shouldn't store that in the state. 

- -10 for missing health
- -10 for missing moves 
- -5 for other errors (unecessary variables, leaving off coordinates)
(combining row and col into "coords" or something is fine)

## 2

Let's derive what going north looks like:
- `map[row - 1][col]` must be in bounds and not in a wall or monster
- if any of `map[row - 2][col]`, `map[row - 1][col - 1]`, `map[row - 1][col + 1]` are in bounds and have a monster, `health' = health - 1`. Otherwise `health' = health`. (We do not need to consider `map[row][col]` for monsters because we are occupying that cell)
- If the state is in bounds, then the northern neighboring state is:
`{ row - 1, col, health', moves + 1}`
- Other directions encoded similarly

(it's fine if you define north as row + 1 instead)


## 3
Manhattan distance

Grading: all or nothing except
10 points awarded for another consistent choice (like euclidean or chebyshev)


## 4
Suppose we have two adjacent cells: c1 and c2 and we can go from c1 to c2. 

- If c2 is closer to the goal by 1, h(c1) <= 1 + (h(c1) - 1) 
- If they are equally close to the goal, h(c1) == h(c2), so h(c1) <= 1 + h(c2)
- If c2 is farther from the goal, h(c1) = 1 + h(c2) + 1, so h(c1) <= cost + h(c2)

regardless, $h(c_1) <= \mathrm{cost}(c_1, c_2) + h(c_2)$

Grading:
+10 pts if it's clear you knew the definition of consistency (either stated it or demonstrated it implicitly)
10 pts per case if you did this kind of case analysis.
You can have just stated that h(s) - h(s') <= 1 with manhattan distance, that's equivalent to the case breakdown above.
