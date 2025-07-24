---
marp: true
theme: slides
paginate: true
---

# Design and Analysis of Algorithms  

## Module 9: Graphs and Paths

<br>
<br>

Slides © Grant Williams, [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/).  

<br>

This is an open educational resource.
Feel free to submit fixes, improvements, and new material [here](https://github.com/grantwill74/notes-on-algorithm-design).

---

# Last week

Last week we learned about dynamic programming.

We saw some simple 1-dimensional DP problems.

We saw how DP is important when solving overlapping sub-problems.

For example, to avoid re-computing Fibonacci terms over and over again, we could save them in a table. To avoid counting-out the same amount of change, we could save it in a table. 

Generally, DP involves some amount of tables.

---

# This week

Now it's time to put that knowledge to the test. DP is a very common algorithm approach when dealing with graphs.

A lot of graph algorithms would otherwise be combinatorial, but they become fast, efficient P-time algorithms when we find ways to define sub-problems and tabulate or memoize them.

But first, let's recall how to work with graphs at all...

---

# How to store a graph

There are fundamentally four ways to store a graph:
- Local adjacency lists
- Global edge lists/maps/sets
- Adjacency matrices
- Implicitly

All are common, all are useful for different kinds of graphs.

There are also two kinds of graphs: directed and undirected.

We'll look at each one, but first, consider this graph...

---

![a diagram of a directed graph. The start node is A, which has only outbound arrows. A is connected to B with weight 5. A is connected to C with weight 4. A is connected to D with weight 1. D is connected to G with weight 1. B is connected in both directions with C with weight 1. C is connected to F with weight 10. G is connected to F with weight 2. F is connected to E with weight 1. B is connected bi-directionally with E with weight 3. E is connected to Z with weight 5. G is connected to Z with weight 10.](digraph1.svg)

This is a directed graph. Some of the connections are symmetric, but it is only considered undirected if every connection is symmetric.

We don't have any bi-directional connections with non-symmetric weights, but we could. It's permitted to do that (I just thought the diagram would look nicer without).

---

# Adjacency lists

Recall that one way to store the graph was with adjacency lists. This means that we associate a list of neighbors with each node. Typically, this list is also where we store the edge weights.

```c
struct node_t; // forward declaration so we can refer to node_t* in edge_t

typedef struct edge_t {
    struct node_t* neighbor;
    int weight;
} Edge;

typedef struct node_t {
    size_t n_neighbors;
    Edge*  neighbors;
    // We can have per-vertex data here too if we want: void* vert_data;
} Node;
```

---

# Creating the list from before

Let's use our structs to build the graph from before. It's easiest if we first create our nodes, and then define our edge lists, both on the stack.

First, the nodes, the number is the number of neighbors:
```c
Node a = {3, NULL};
Node b = {2, NULL};
Node c = {2, NULL};
Node d = {1, NULL};
Node e = {2, NULL};
Node f = {1, NULL};
Node g = {2, NULL};
Node z = {0, NULL};
```

Notice how the edges are all null. We'll overwrite the `NULL`s with valid edge lists soon.

---

# Creating the list from before (2)

Now we need to create the neighbors. I'll define some of them; I'll leave the rest to you.

```c
Edge a_neigh[] = {{&b, 5}, {&c, 4}, {&d, 1}};
Edge b_neigh[] = {{&c, 3}, {&e, 1}};
Edge c_neigh[] = {{&f, 10}};
// ... fill in the rest
```

Once we create these edge lists, we can hook them into the graph by setting the neighbors field in each node:

```c
a.neighbors = a_neigh;
b.neighbors = b_neigh;
c.neighbors = c_neigh;
// ...
```

At this point, our graph is ready to use

---

# Short note about memory management

However, having to define everything on the stack is rather limiting. What if we're loading graph data from a file, and therefore we don't know exactly which nodes we need ahead of time?

In that case, we can allocate a big array of nodes:
```c
Node* arena = malloc(number_we_need * sizeof(Node));
```

And then, because our nodes are laid out in an arena in a defined order, we can also iterate over that for loop and `malloc` the neighbors, too.

This also gives us the flexibility to refer to nodes by an ID instead by pointer. This is useful both for stability (e.g., when serializing/deserializing) and also to save space: if we have fewer than about 4.2 billion nodes, we can use an unsigned 32-bit integer.

---

# Memory management (2)

We can also use our pool-based memory managent here if we want, because each node is the same size in memory. 

We can also store the neighbors in an array list, linked list, hashtable, or tree. All of these data-structures can grow dynamically so we don't need to pre-allocate space for all our neighbors. Here it is as a linked list:

```c
typedef struct edge_t {
    struct node_t* neighbor;
    struct edge_t* next; // in linked list
    int weight;
} Edge;
typedef struct node_t {
    Edge* first_edge; // don't need n_neighbors since it's implicit now
} Node;
```

---

# Summary of adjacency lists

So the key to using adjacency-list graphs is that we store nodes.

Each node has a list of neighbors inside of it.

We often attach a weight to each neighbor, using an edge struct.

Memory management is complex: we need to decide where nodes will live and where their adjacency list will live.

If we need to store per-node information and the graph is sparse, this is a reasonable way to store graphs. Also if we need to answer questions about who a vertex is connected to, or to do basic vertex-based searching, such as DFS.

Otherwise, one of the other ways might be better...

---

# Questions?
<!-- _class: invert questions -->

---

# Edge lists

Some algorithms are very "edge focused", like [Kruskal's Algorithm](https://en.wikipedia.org/wiki/Kruskal%27s_algorithm), which computes a minimum spanning tree from a given graph.

A minimum spanning tree is a subset of a graph that connects to every node while having the smallest possible sum of edges. It has no cycles and is undirected, which is why we call it a tree. 

You could imagine this being useful for connecting houses into a telecom network.

---

# Edge lists (2)

Kruskal's algorithm operates over a sorted list of edges, so it's much more efficient if we just store our graph as a list of edges, rather than requiring the algorithm to search through all the adjacency lists inside all the nodes.

Another example is the [Bellman-Ford](https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm) shortest path algorithm, which finds the shortest path from one node to every other node by gradually finding shorter edges.

Finally, it's common for file formats that store graphs to store them in this style. For example, the mermaid diagrams that I use in these lectures are textual edge-lists.

---

# Simplest edge list

The simplest way to store an edge list is with an array:
```c
typedef struct edge_t {
    int source;
    int dest;
    int weight;
} Edge;

Edge edge_list[] = {{0, 1, 100}, {1, 2, 50}, {0, 2, 25}, ...};
```

Notice that I'm storing the source and destination nodes as integers.

There is no "node" structure here. There *could* be, but there doesn't have to be. In this representation, we usually just care *that* two nodes are connected, rather than putting data inside the nodes themselves.

---

# Node IDs

Here, the fact that two nodes have different integers makes them different.

However, there's no reason why we couldn't use string IDs: there might be some memory management, but we can still compare them efficiently with SIMD if we avoid making the names too long.

We could also have node structures as before if we need to associate data with nodes, and have pointers to them in our edge structure. It's permitted but not required.

---

# Using the list

One issue with edge-lists, though, is that we need to organize the list.

If the list is not ordered, then any kind of query will be $\Theta(|E|)$ time on average, which is unacceptable for most applications.

Kruskal's algorithm relies on the list being sorted by edge weight. So why not sort the edges using `qsort`? This will take $\Theta(|E| \lg |E|)$ on average, but we only need to do it once. We just need to iterate over this list in order.

Alternatively, what if we want to determine who the neighbors of a particular node are? Then we can sort by the source id instead. Now a quick binary-search through the list will let us find all the edges out of source.

---

# Drawbacks

This raises the biggest drawback of edge lists, though: the need for the edge lists to be in a particular order.

Sorting isn't *slow*, and if we know something about our distribution of source or dest IDs, there might be shortcuts (like counting sort). 

We could also store our edges in a tree instead, sorted by the index we choose (like weight or source). We could also use a hashmap, but then we wouldn't be able to get every neighbor of a particular node (if that's not required, this is a good solution)

If we want to have multiple kinds of queries, we would need multiple copies of edge lists, sorted differently. This can be problematic if we need to insert a new edge. Workflows with random insertion and deletion probably require using a tree.

---

# Practice

- Implement the two techniques for graphs that we've talked about so far.
- Define our graph from earlier as an edge list.
- Write a qsort routine that sorts an edge list by edge weight.
- How could we easily implement undirected graphs using the previous two techniques? Would it be better to store each edge twice, once for each direction, or should we find ways to only store it once?

---

# Questions?

<!-- _class: invert questions -->

---

# Adjacency matrices

So far, we've seen two graph data-structures that are based on lists, or some other data structure that supports iteration.

However, in most graph applications, two nodes are connected at most once. 

If this is true, there is a limit to how many connections there can be.

Suppose there are $|V|$ nodes. What is the limit to the number of edges ($|E|$)?

---

# Adjacency matrices (2)

It turns out, $|E| \le |V|^2$.

That is, every vertex can be connected to every other vertex, which means that there are $|V|$ vertices for every vertex, hence $|V|^2$.

If every vertex is connected to every distinct vertex in a graph, we call it *complete*.

If *most* vertices are connected, we call the graph *dense*. This is a subjective term, of course, but generally there's a point at which a graph is *complete enough* for the following representation to be effective...

---

# Adjacency matrices (3)

An *adjacency matrix* is basically a giant matrix of numbers, where each number tells us the weight of one edge.

The row is typcially the source and the column is typically the destination, simply because row-major is a common way of laying out 2D arrays in most languages.

To look up an edge value, we typically write `matrix[row][col]`.

---

# Another graph

![Another digraph. Here, A is connected to C with weight 15, to B with weight 10, and to D with weight 10. B is connected to C with weight 2. C is connected to B with weight 3 and to D with weight 2. D is connected to A with weight 1](digraph2.svg)

---

# Adjacency matrix

This matrix corresponds to the graph in the previous slide:

| - | A | B | C | D |
|---|---|---|---|---| 
| A | - | 10| 15| 10|
| B | - | - | 2 | - |
| C | - | 3 | - | 2 |
| D | -1| - | - | - |

The row is the source, and the column is the destination.

---

# Why?

Adjacency matrices are very fast at answering one, specific question:
What is the weight of the edge between A and B.

They can be wasteful: if we have 100 vertices, that's a 100 by 100 matrix. What if the graph is sparsely connected? Like, maybe there are 200 edges. Then we've wasted a ton of memory.

It might be worth it though: edge lookups in this format are constant time.

So what defines whether a graph is dense, or the opposite, sparse?

---

# Counting memory

The only real downside to an edge matrix is its memory consumption.

Suppose we use the edge data structure we looked at previously for the edge-list representation. How much space does an edge take to represent?

```c
typedef struct edge_t {
    int source;
    int dest;
    int weight;
} Edge;
```

What do we expect sizeof(Edge) to be on a typical 32 or 64-bit machine?

---

# Counting memory

In this case, 12 bytes. If we used pointers for source and dest, it would be 24 bytes (because 4 bytes of padding would be inserted after weight. [Read this](http://www.catb.org/esr/structure-packing/) to learn why.)

On the other hand how big do we expect the adjacency matrix to be?

[Class?]

---

# Size of adjacency matrix

In this case, there is space allocated for $4 \times 4 = 16$ edges.

Each edge is an int, and `sizeof(int)` is usally 4. Therefore we expect it to use 64 bytes.

Is that a lot? Well, there are 7 edges. So, amortizing it: roughly $9.14$ bytes per edge.

On the other hand, our edge struct required $12$ bytes per edge. So the matrix is more efficient for this particular graph.

We can think of one definition of *dense* as being a graph with enough edges so that the matrix is more efficient than the list.

---

# Seems too good to be true?

But our graph only has 7 out of 16 edges! And it's still more efficient? Why even bother with edge lists if the matrix is so good?

Because what happens if we add another node?

Now we have $5 \times 5 \times 4 = 100$ bytes to store instead of 64. And if we add another?
$6 \times 6 \times 4 = 144$ bytes. It's quadratic, so we keep adding more and more each time we add one node.

In applications where we might have a lot of sparsely connected nodes, this does not work at all. Imagine if there were 1 million nodes? How many ints would we need to allocate?

---

# Too much space

A million nodes is completely forseeable. A GPS or pathfinding database can easily have more. 3D scenes, connectivity graphs, many different realistic data structures can easily exceed this.

And yet, that's 1 trillion ints. How many ints can fit in your computer if you have 64 GB of ram? Exactly $68\;719\;476\;736$ ints.

That's around 69 billion. So it wouldn't even fit in RAM even with lots of it and with theoretically your entire memory dedicated to storing this one graph.

However, this is assuming the graph is sparse (meaning: the matrix wastes space). If the graph is dense, then the matrix *doesn't* waste space, and it might just be impossible to store the whole thing in RAM at once. It might need to be broken up.

---

# And yet...

And yet the adjacency matrix is super easy to use. Just allocate a big 2D array (or a 1D and do the math for row-column access). 

If you're doing a competitive programming or interview problem, and it's clearly a graph problem with $\le 1000$ nodes, that can be a hint that an adjacency matrix is appropriate.

It's easy to set up, easy to use, and maximally fast at determine whether two nodes are connected and what that connection's weight is.

If the graph is dense, iterating over all the neighbors of a node is going to be on average $\Theta(|V|)$, which is as good as an adjacency-list.

---

# Questions?

<!-- _class: invert questions -->

---

# Using adjacency matrices

There are two basic data layouts we typically use for an adjacency matrix:
1. A 2D array: works in C, but in some languages, either not available, or involves indirection, which is slow.
2. A 1D array: works in any language with arrays

In C, we can easily allocate a 2D array on the stack:
```c
int mat[100][200];
```

Unfortunately, we often *can't* do this. Stacks have limited sizes, and matrix representations of graphs can easily overflow it. Even for static memory, especially on Windows, there are limits. So how can we fix that?

---

# Dynamic allocation

If we know the size of the matrix, we can store it as a fixed-size array of rows:

```c
int (*mat)[200] = malloc(100 * sizeof *mat);
```

Pay close attention to the parentheses on the left. This is **different**:
```c
int *mat[200] = malloc( ... );
```

The first one defines `mat` to be a pointer to arrays of 200 ints. That means we can index `mat[row]` to get one of these arrays, and `mat[row][col]` to get an element. This is what we want, because it means the array is densely packed.

[What is the second one?]

---

# Dynamic allocation

The second one defines mat to be an array of 200 pointers. Then we would have to iterate over that array and malloc a row for each pointer. Accessing an edge weight would require extra indirection: we would need to follow a pointer first.

The first one is singly-indirect. It also only requires one `malloc` and one `free`.

[Read this](https://c-faq.com/decl/spiral.anderson.html) if the syntax is confusing. We generally start with the variable name, and go right to left, inside parentheses to outside. The author relates it to a spiral shape.

In C, higher-dimensional arrays are still densely packed. That is, we can treat 1000 adjacent ints as if they were a $10 \times 100$ array or a $200 \times 5$ array, etc. But if we use dynamic allocation, we typically treat the pointer as pointing to a single row with a width equal to the number of columns.

---

# Using it

To use it, just set the row and col weights like this:
`mat[row][col] = weight;`

When you're done, free the whole array (another reason why the singly)

---

# Using 1D addressing


---

# Questions?

<!-- _class: invert questions -->

---

Marp source 1 (digraph):

```
flowchart LR
    A -->|5| B 
    A -->|4| C 
    A -->|1| D
    B <-->|3| E
    F -->|1| E
    C -->|10| F
    G -->|2| F
    C <-->|1| B
    D -->|1| G
    E -->|5| Z
    G -->|10| Z
```

---

Marp source 2 (digraph 2):

```
flowchart LR
    A -->|10| B
    A -->|15| C
    C -->|2| D
    C -->|3| B
    D -->|1| A
    B -->|2| C
    A -->|10| D
```