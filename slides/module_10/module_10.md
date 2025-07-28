---
marp: true
theme: slides
paginate: true
---

# Design and Analysis of Algorithms  

## Module 10: 

### Graph search odds and ends; Graph modelling

<br>
<br>

Slides © Grant Williams, [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/).  

<br>

This is an open educational resource.
Feel free to submit fixes, improvements, and new material [here](https://github.com/grantwill74/notes-on-algorithm-design).

---

# Last week

Last week we covered a lot of material in a short space:
- Graph representations in code
- Searching strategies
- The flood-fill algorithm
- The ever-important Dijkstra's algorithm

---

# This week

Ever-important? Yes! This week we're going to see how we can solve problems that don't even look like graph problems using Dijkstra's algorithm.

We're also going to see how to solve problems that *do* look like graph problems but that are much more difficult.

But first, there's an important odd or end I want to cover before we move on: A\*

---

# Motivating A\*

Recall that Dijkstra's algorithm worked roughly like this:
- Consider every path we know about, sorted by total length
- Take the shortest path we haven't explored
- If it's the destination, great.
- If not, record where we came from and add paths to all the neighbors.

The reason Dijkstra's algorithm works is that we explore every path strictly in
order of how long it is. Therefore, once we find a path that reaches the goal,
we know we're done (any other paths are longer).

But that doesn't seem very...*intelligent*.

---

# How do we solve the problem?

Watch this animation of Dijkstra's algorithm. What do you notice?

![bg right height:100% an animation showing the order the nodes are explored using Dijkstra's algorithm. The start node is on the lower left, and the destination node is on the upper right, but there is an obstacle with a concavity facing the source node. The algorithm basically searches around the source node in all directions, gradually widening the search radius. It does not seem to take advantage of open runs where it can move towards the destination.](Dijkstras_progress_animation.gif)

<div class="footnote">

[Animation](https://en.wikipedia.org/wiki/File:Dijkstras_progress_animation.gif) by [Subh83 (Subhrajit Bhattacharya)](https://en.wikipedia.org/wiki/User:Subh83?rdfrom=commons:User:Subh83). 
License: [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/deed.en).

</div>

---

# It's very similar to breadth first

Remember the problem we discussed last class: the two islands with the bridge?

We solved that problem with a breadth-first search. Because there was no notion of "cost" (every tile "cost" the same), this gave us what we wanted. We wanted to search in concentric "rings" around the first island.

When the weights are all the same, that's what Dijkstra's algorithm does. It's lowest-cost-first search, which is breadth-first search when the costs are all the same.

And why might that not be desireable?

---

# It's slow

Notice that we end up searching almost every node.

This is *not* how a human would approach the problem. Most of us would start moving in the direction of the goal. 

If we hit an obstacle, we'd try to go around it.

---

# Why not do that?

The problem with just going straight to the goal and trying to work our way around obstacles is that we can get caught in  *concavities*. 

When that happens, we need to remember where we came from and backtrack.

If we don't backtrack, we end up getting stuck, like these monsters in Gauntlet.

![bg right height:100% width:100% an animation of the game Gauntlet, ported to the Commodore C64. In the animation, enemies are trapped on the other side of a wall from the player, but they keep trying to move to the player even though they can't reach them. The player takes advantage of this by shooting through a crack in the wall as enemies fill in to the empty space.](gauntlet.gif)

<div class="footnote">

[Animation](https://www.c64-wiki.com/wiki/File:Gauntlet.gif) from [c64-wiki.com](https://www.c64-wiki.com/wiki/Gauntlet)

</div>

---

# Backtracking

The issue with backtracking is that is has the potential to go very wrong.

We could end up getting lost down many dark hallways over and over again, making progress very slowly. 

Also, suppose we find the goal: we might not have taken the shortest path! So we would still need to "relax" our solution by exploring more, which takes longer.

---

# Algorithm design goal

Ideally, we want some way to combine two behaviors:
- Considering all possible paths simultaneously, so that we get that nice feature of Dijkstra's of being able to stop when we reach the goal.
- "Bee-lining" toward the goal when available. Or at the very least, being biased towards searching paths that move us physically closer to the goal.

There is at least one algorithm that accomplishes both of these: A\*.

---

# A\*

[A\*](https://en.wikipedia.org/wiki/A*_search_algorithm), pronounced "EY-star", was the result of a [Stanford Research Institute](https://en.wikipedia.org/wiki/SRI_International) project to get [Shakey the Robot](https://en.wikipedia.org/wiki/Shakey_the_robot) to move around an interactive maze successfully.

You might be expecting a radically different algorithm than Dijkstra's, because of the need to balance efficient searching with guaranteeing optimal paths.

Actually, A\* is literally just Dijkstra's algorithm, but we compute the cost of a path slightly differently.

---

# Path costs

Instead of sorting paths by their total cost so far, we sort them by this:
Total cost so far + estimated cost to goal

In math, instead of total cost by itself, we compute: $\mathrm{cost} = g(x) + h(x)$,
where $g(x)$ is the cost so far, and $h(x)$ is an estimate of the cost to the goal. The $h$ stands for "heuristic", which basically means "guess".

That's it. We just add this additional estimate of "how far is the goal from here" to the cost of the path, and just run Dijkstra's like normal.

Why does that help? Because it means we're less likely to go backwards. The nodes in the opposite direction of the goal will have a very high $h(x)$, so we'll consider fewer of them.

---

# A\* animation

Notice how it "bee-lines" straight to the path before getting stuck.

It does this several times, because each "bee-line" potentially be the one that gets through.

This might seem inefficient, but notice that it searches fewer nodes total than Dijkstra's algo did.

![bg right height:100% width:100% An animation similar to the one showing off Dijkstra's algorithm's search strategy, but using A-star instead. There is still a concave obstacle in the same place, but now, instead of searching in "rings" around the source, it "bee-lines" straight to the destination, running into the concavity. It runs into the concavity several times, bee-lining it over and over with adjacent paths, before finally trying a couple that go around, eventually reaching the destination. Despite this "hitting the wall" behavior, it ends up searching fewer nodes than Dijkstra's algorithm did, indicating greater efficiency.](Astar_progress_animation.gif)

<div class="footnote">

[Animation](https://commons.wikimedia.org/wiki/File:Astar_progress_animation.gif) by [Subh83 (Subhrajit Bhattacharya)](https://en.wikipedia.org/wiki/User:Subh83?rdfrom=commons:User:Subh83). 
License: [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/deed.en).

</div>

---

# Another A\* animation

Notice how it doesn't search the little "caves" that move backwards from the goal.

Dijkstra's algorithm would have searched those.

The reason A\* doesn't is that they have a higher estimated cost than the nodes in a "forward" direction, even if their known cost is small.

![bg right:50% width:100% height:50% Another animation demonstrating A-star, this time searching through a grid-like environment with many obstacles. We see a series of paths snaking through the grid, gradually getting longer. The paths it considers start in the north-west and are generally moving in the direction of the south-east (the goal). Of note is the fact that it does not search in little "caves" that are moving "backwards" from the goal, wheras Dijkstra's algorithm would search those "caves".](Astarpathfinding.gif)

<div class="footnote">

[Animation](https://en.wikipedia.org/wiki/File:Astarpathfinding.gif) by "Wgullyn". License: [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/deed.en)

</div>

---

# How to do A\*

- Literally just do Dijkstra
- But when it comes time to add your path to the queue, don't do this:
  `path = {source: X, dest: Y, cost: cost_so_far + cost[X][Y]}`
- Instead, do this:
  `path = {source: X, dest: Y, cost: cost_so_far + cost[X][Y] + estimate[Y]}`
- That's it. You're using A\* now.

---

# Generating that estimate

Okay, so how do we generate an estimate?

First, we call it a *heuristic* from now on. Not an estimate. That's the technical term for the estimate that A\* uses. We see this term a lot in algorithms when you have a choice, and some choices are better than others, but the algorithm leaves it up to you.

The most common heuristic is the straight-line, as-the-crow-flies distance to the goal. 

That is, if we're on a grid, and each cell $i$ is at $x_i, y_i$, and the goal is $x_d, y_d$, then we compute the distance between $x_i, y_i$ and $x_d, y_d$ as the heuristic for cell $i$.

How do we do that?

---

# The Pythagorean theorem

We can use the [Pythagorean theorem](https://en.wikipedia.org/wiki/Pythagorean_theorem).

The value we want is the hypotenuse, and the side-lengths are the axis-distances:
$d = \sqrt{(x_d - x_i)^2 + (y_d - y_i)^2}$

This is the most common choice of heuristic. It also has some nice properties in Euclidian space, which we'll look at next:
- It is *admissible*
- It is *consistent*

---

# Admissibility

It turns out, we can't just use any heuristic as $h(i)$.

For classic A\*, one standard requirement for $h(i)$ is that it be "admissible", which means that it never overestimates the true cost.

Note: *under*estimates are fine and expected. We certainly don't want an heuristic that has to be exactly right, or else we might as well not have one at all.

But we're not supposed to use a heuristic that *over*estimates the distance.

Why do you think that is?

---

# Admissibility (2)

Because the heuristic $h(i)$ for a cell $i$ is used along with the total cost to sort the paths in the priority queue.

Recall that with Dijkstra's algorithm, we want to stop as soon as we process a path to the goal.

If we have a heuristic that overestimates the distance of some cells, we might choose not to explore them, even if they contained the actual shortest path.

This means we "missed" the shortest path, and the path we found is sub-optimal.

---

# Consistency

For A\* based on classic Dijkstra's algorithm, where we only permit one of each destination cell in the priority queue at a time, admissibility is all we need to guarantee the correct shortest path.

However, if we want to use the lazy version (and we probably do), then admissibility is not enough, we instead need *consistency*.

A heuristic is consistent if it not only does not overestimate the distance to the goal, but also doesn't overestimate the distance between any nodes in the graph.

---

# Why do we need consistency in lazy A\*?

For the lazy-A\*/Dijkstra, we assume that we have found the shortest path to a node whenever we take it out of the priority queue.

Once we add the heuristic function, we have the possibility of visiting nodes out of order of their total cost. 

Suppose the heuristic "wobbles", meaning that for some nodes it's relatively high, and for others it's relatively low. It never overstates the distance to the goal, but it might make it seems like some nodes are better for reaching the goal than others.

Now we search in a weird order. We might avoid a node thinking it's far away, only to realize later that we found a shorter path to it. We cannot assume that just because a node has come up, that we've found the shortest path to it. 

---

# Classic A\*/Dijkstra

We haven't really discussed why I prefer lazy Dijkstra.

If we use classical Dijsktra, we actually need a strange data-structure called a [Fibonacci Heap](https://en.wikipedia.org/wiki/Fibonacci_heap) for our priority queue. Otherwise we don't get $O(|E| + |V| \log |V|$) time anymore; instead it's $O(|V|^2)$.

Using the lazy algorithm allows us to achieve the fast time bound, while using an ordinary binary heap as our priority queue. In practice, I have never seen someone use a Fibonacci heap outside of an algorithms textbook.

But when doing A\*, we have the additional requirement that we use a *consistent* heuristic, and not merely an *admissible* one, in the lazy variant of Dijkstra

---

# Consistent heuristics

In practice, anytime our heuristic is based on something like straight-line distance, it is going to be consistent.

It only won't be consistent if we're using a strange heuristic that assumes that space is warped in some kind of non-uniform way.

Two things though:
1. *Uniform* space warping is often useful.
2. Sometimes we don't care if the path we find is perfect

Let's examine both situations. But first...

---

# Questions?

<!-- _class: invert questions -->

---

# Manhattan distance

Consider a grid-like space, where we cannot go diagonally.

That green path is impossible. The three other paths all have the same distance.

We don't use the pythagorean distance here, because it will vastly underestimate.

This is called Manhattan or Taxicab distance.

![bg right width:100% An image illustrating Manhattan distance. There is a grid, like the streets of Manhattan seen from above. The paths travelling 6 blocks north and 6 blocks east, one block north and one block east six times, and 5 blocks east, two blocks north, one block east, and 4 blocks north, all have the same distance of 12 blocks. There is a green line travelling as the crow flies, from the same source and to the same destination as the other lines. In Euclidean geometry, this would be the shortest path. However, this space is non-Euclidean, because diagonal motion is not available to us, so the green path doesn't represent a valid path.](Manhattan_distance.svg)

<div class="footnote">

[Image](https://en.wikipedia.org/wiki/File:Manhattan_distance.svg) by [Psychonaut](https://commons.wikimedia.org/wiki/User:Psychonaut). License: public domain.

</div>

---

# Manhattan distance (2)

But wait, I thought it was fine for the heuristic to underestimate?

It is fine, but ideally it would estimate accurately. If it underestimates too much, we end up searching a lot of nodes that weren't going to get us to the destination.

The Manhattan distance between cell $i$ and destination $g$ is computed like this:
$d = |x_g - x_i| + |y_g - y_i|$

So we don't square the differences, and we don't take the square root. We just add the x difference to the y difference. That's Manhattan distance vs Euclidean distance.

If we can't move diagonally (or if moving diagonally costs the same as moving up and then left, etc.), Manhattan distance is the better estimate, and it still won't be an over estimate.

---

# Minkowsky distance

But what if we *can* move diagonal, but it's more expensive? Or what if moving diagonally is *cheaper* than moving vertically or horizontally?

There is a generalized distance function that looks like this:
$d = (|x_g - x_i|^p + |y_g - y_i|^p)^{(1/p)}$

The value of $p$ determines how space is warped. $p=1$ means diagonals aren't helpful. $p=2$ means prefer straight line. $1 \lt p \lt 2$ means diagonals aren't always helpful, but sometimes they can be.  $p\gt 2$ means diagonals are *super* helpful, and we should try to find perfectly diagonal paths. $p \lt 1$ means diagonals are *anti* helpful.

This generalized distance function is called Minkowsky distance. Consider it when working on project 5!

---

# Metric space examples

Notice how, as the value of diagonals changes, the distance of the shortest path changes. 

In video and board games especially, the "true" distance is not always Euclidean.

It's okay to underestimate with A\*, as long as you are consistent, but it's better to be as accurate as possible.

![bg  right width:100% height:100%](Minkowski_distance_examples.svg)

<div class="footnote">

[Image](https://en.wikipedia.org/wiki/File:Minkowski_distance_examples.svg) by [Cmglee](https://commons.wikimedia.org/wiki/User:Cmglee). License: [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/deed.en).

</div>

---

# What if we don't really care?

Remember that screenshot of Gauntlet earlier in the presentation?

That is a fast-moving arcade game. And in that screenshot, it was running on a Commodore 64, which only had a 1 Mhz. processor.

Are we seriously going to run A\* for each enemy, each frame? 

No, that would be impractical. In the game, it just had enemies bee-line the player.

But that meant enemies got stuck in corners! What if we want bee-lining, but with the ability to recover if we get stuck?

---

# Using non-admissible heuristics

Use an overestimate. Sometimes we just need *a* path. Here's an example where the heuristic is 5-times the distance to the goal.

Notice that the behavior is very "realistic": it hits the wall and then goes around. Fast, too!

And it still finds its way to the goal, even if the path isn't optimal.

![bg right width:100% height:100% an illustration by the same author as the first two animations. The scenario is the same: there is a concave obstacle with concavity facing the source node, viewed from the top. Here, the search algorithm searches every node in a line to the destination until hitting the obstacle. Then, it ](Weighted_A_star_with_eps_5.gif)

<div class="footnote">

[Animation](
https://en.wikipedia.org/wiki/File:Weighted_A_star_with_eps_5.gif) by [Subh83 (Subhrajit Bhattacharya)](https://en.wikipedia.org/wiki/User:Subh83?rdfrom=commons:User:Subh83).
License: [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/deed.en).

</div>

---

# Trade-offs

Ultimately, A\* is a useful variant of Dijkstra's algorithm that lets us bias the search algorithm towards nodes that are likely to be closer to the goal.

If we set the heuristic to 0, we just get DIjkstra's algorithm, so we can think of Dijkstra as a special case of A\*.

When should we use A\*? When we have a heuristic that gives us useful information.

If we're in a labyrinth in which each turn is equally likely, the straight-line distance gives us no information and computing it just slows us down.

But in "real world" type spaces, like video game maps or spaces with limited obstacles, A\* can be a big improvement.

---

# Questions?

<!-- _class: invert questions -->

---

# Switching gears

I'm going to give you a kind of problem that can appear on "hard" coding interviews and which is common in competetive programming.

Solving it is actually quite easy if you conceptualize it correctly, but it's very hard to conceptualize it correctly if you haven't seen it before.

---

# A strange question

You are playing a turn-based role playing game.

You have 100 hit points (health) and 3 magic points.

You are up against $n$ enemies, whom you will fight in sequence.

Each enemy $i$ has $h_i$ health and does $d_i$ damage when they attack

You and the enemy take turns. You go first. You will take an action, and then the enemy will attack you for $d_i$ damage (subtracting that many hit points)

If your hit points is $\le 0$, you lose. Same for the enemy.

You have to fight a series of enemies, and your health and magic carry over between fights.

---

# A strange question (2)

You have several combat actions you are allowed to take:
1. Attack: you do 10 damage to your opponent
2. Heal: drink a healing potion. Restore 50 health, but only 2 times per rest.
3. Blast: you do 20 damage to your opponent, but lose a magic point.
4. Defend: regain a magic point, and take half damage from the next attack.
5. Flee: run away, rest, and regain all your health, heals, and magic, but your opponent is also likewise fully restored. Opponents you have defeated stay defeated, so this "resets" the current combat.

---

# A strange question (3)

The question is, given a list of opponents, to find out:
1. Whether it is possible to beat them at all. I.e., is there some order of actions that defeats all the enemies without running out of hitpoints
2. If so, to determine *the shortest number of actions* it takes to do so.

Let's spend some time thinking about how we would approach this problem.

[any ideas?]

---

# This is actually a graph problem

Believe it or not, this is a graph problem, and we can use Dijkstra's/A\*.

How? Fundamentally, our little scenario describes a finite state machine. There are only certain states that are possible to reach. That is, our health and magic can only have certain values, and so can the healths of our enemies.

State machines are graphs. Therefore, we can reframe our problem as trying to find the shortest path from the start state to an accepting state.

As an example, suppose we are up against a single Goblin who has 25 health and who does 20 damage...

---

# A fraction of the space

I'm only showing a couple of moves here. This flowchart shows what it looks like to model the problem as a graph.

I'm only showing attack and blast and one flee. In reality, every non-victory state can "flee" back to the start state. I also left out heal and defend for space.

![bg right height:80% width: 100% A flow chart showing only some of the possible actions and states that can be reached. For example, from the start state where we have 3 magic points, 100 hit points, and an enemy with 25 HP, we can attack, which will make the enemy's hp become 15, but the enemy will attack, making our health be 80. This is labelled as a separate state. Likewise, we could flee, which would return us to the start state. We could blast the enemy; they would still do 20 damage, but they would only have 5 hp. In that state, we could either attack or blast to defeat them, but if we blast, we'd have one less magic point. Lastly, if we attacked twice from the start state, we'd have only 60 hp and the enemy would have 5, but we'd have all our magic points.](graph_modelling_1.svg)

---

# How do we solve it?

Each edge has a length of 1: it's one action.

Each "node" is a state, which represents a distinct value assignment for every variable.

What are the neighbors? The actions. Every non-dead-end node has 5 neighbors, one for each action you are allowed to take.

To solve this problem, we use Dijkstra's algorithm. Our "path" variable is the current state of the game (the numbers), plus the action we're considering. 

Each action takes one turn, so technically, Dijkstra's is the same as breadth first search. However, what if an action took multiple turns? Like, if fleeing took 5? Then we would need Dijkstra's algorithm.

---

# Psuedocode

- Create a priority queue that stores instances of this structure:
  `{hp, mp, enemy_hp, actions_taken}`

---

# Questions?
<!-- _class: invert questions -->


---

# Simplifying it

The state space is huge

---

# Simplifying assumptions:

- It never makes sense to flee except as the first thing we do in a fight.
- If we've already visited a state with `hp: X, mp: Y, enemy_hp: Z`, we should never consider a state in which all the following are true: `hp <= X, mp <= Y, enemy_hp >= Z`.
- If the enemy has $\ge 20$ health, we might as well blast and then defend. It does the same damage but we take less damage. This will add 2 to the turns taken, but it lets us ignore 2 turns of attacking.


---

# Graph modelling flowchart 1 source

```
flowchart LR
    Start["hp: 100<br> magic: 3<br> enemy: 25"]
    A["hp: 80<br> magic: 3<br>enemy: 15"]
    Start -->|attack| A 
    B["hp: 80<br> magic: 2<br>enemy: 5"]
    Start -->|blast| B 
    C["hp: 80<br> magic: 1<br>enemy: defeated"]
    B -->|blast| C
    D["hp: 80<br> magic: 2<br>enemy: defeated"]
    B -->|attack| D
    A -->|blast| D
    E["hp: 60<br>magic: 3<br>enemy:5"]
    A -->|attack| E
    A -->|flee| Start
```