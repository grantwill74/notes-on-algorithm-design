# Key

## 1

`{ hull_remaining, current_system, _fuel_spent_}`

Grading: -5 for every missing or other value or missing mark on metric.

## 2

Given the above state, from system `i` to every neighbor `j`
```
{ hull_remaining' = hull_remaining - systems[j].n_pirates, 
   current_system' = j, 
   fuel_spent' = fuel_spent + 
    sqrt( pow(systems[j].x - systems[9999].x, 2) + 
          pow(systems[j].y - systems[9999].y, 2)); }
```

A neighbor is only valid if `hull_remaining' >= 1`
The goal state is `euclidean_distance_to_destination == 0`

-10 if hull remaining calc is missing
-5 if forgetting to say that hull remaining must be `>=` 1
-10 if goal state missing

## 3

Euclidean distance

## 4

consistency means, for adjacent states $s$ and $s'$, $h(s) \le c(s, s') + h(s')$

The heuristic is euclidean distance, which is also the cost $c(s, s')$
Therefore we have $h(s) - h(s') = c(s, s')$, ensuring consistency.

+10 pts for stating the definition
+30 pts for showing consistency, either all at once like this, or with a case argument like we did together earlier.

