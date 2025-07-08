---
marp: true
theme: slides
paginate: true
---

# Design and Analysis of Algorithms  

## Module 5: Trees

<br>
<br>

Slides © Grant Williams, [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/).  

<br>

This is an open educational resource.
Feel free to submit fixes, improvements, and new material [here](https://github.com/grantwill74/notes-on-algorithm-design).

---

# Introduction

In this module we will review trees.

We will learn about their benefits, their drawbacks, and how to mitigate them.

However, before we get too much into it, I have a quesiton:

What is the point? Why do we care about trees?

They feature prominently in CS education, but how often have you been like "I need a binary tree for this" when doing your software engineering or capstone projects?

---

# The point of trees?

Trees are *one* way of implementing dictionaries (aka *maps*).

Hashmaps are another.

These two completely different approaches both allow us to create the same abstract structure. And it turns out, there are reasons to use both.

[So what is a dictionary, and how does it work?]

---

# Dictionaries

A dictionary can be seen either as a generalization of an array, or as a special case of relation (partial map). 

Ordinarily, arrays are a kind of function. You give them an index, and they give you some kind of value.

We say that they are *partial* functions, because arrays are not required to have a value for every index. Some indices might not have a value. If an array has size 100, then any index $\ge$ 100 is not going to be there.

However, array indices are limited to natural numbers (usually, technically in C you can access arrays with negative indices for extremely arcane purposes). And the keys need to be adjacent for efficiency reasons.

---

# Dictionaries (2)

Dictionaries are data structures that break those two limitations:
1. You can associate (almost) any kind of key with any kind of value.
2. Integer keys don't need to be adjacent. You can have 1: "Hello" and 1000000000: "Goodbye" in the same data structure without allocating a one billion and one cell array.

[So, how do we implement them?]

---

# Default: hashmaps

By default, dictionaries are almost always just a thin wrapper over a hashmap.

That's how it works in Python, Lua, Ruby, and many other languages.

Hashmaps are fast, cache efficient, and typically waste no more than twice the amount of memory necessary when they are reasonably large.

However, Hashmaps have a major limitation [what is it?]

---

# Hashmaps are not sorted

The whole point of a hash function is that it be as close to uniform random as possible.

That is, the hashvalue of a particular input should be as unpredictable as possible without evaluating the function.

Why? Because we want to use the whole hashtable. If the function frequently returns some keys more than others, we end up with more collisions.

So it's not only unsorted: it's *super* unsorted. It's as unsorted as possible.

On the other hand...

---

# Trees are sorted

Binary search trees in particular are sorted.

Specifically, an in-order traversal will be in order.

[Why?]

---

# Trees are sorted (2)

By induction on a tree:
- If it's empty, it's sorted
- If in-order traversal of its child nodes is sorted, and everything in left < root < everything in right, than inorder(left) ++ root ++ inorder(right) is sorted.

So the normal property of BSTs, that the node's value is always between the values of its left and right children, naturally makes it so that in-order traversal is sorted.

---

# Questions?
<!-- _class: invert questions -->

---

# Who cares?

I have a question: why do we need dictionaries to be sorted?

It seems like every time we use a dictionary it's because we know the keys.

And even if we don't, like, if we want to iterate over all of them, we can just put the keys in an array. That still seems better than using a tree.

---

# Good examples

```sql
SELECT * FROM Transactions WHERE TxDate BETWEEN
 DATEADD(month, -1, CURRENT_DATE) AND CURRENT_DATE;
```

We're choosing all transactions in the last month.

How does the database actually do that?

One way to do it: put the dates in a tree.

The key is the date, the value (aka satellite data, we'll talk about it soon) is the row.

---

# Good examples (2)

The database can quickly scan over the date tree, in order, to find the start and end.

This ends up being $O(\lg n + k)$: 
- $\lg n$ for the initial search in the BST
- $k$ for the number of elements in the range

But it needs to stay sorted. We don't want to have inserts take too long.

---

# Tree dictionaries

So, sometimes we need a structure that stays sorted, but also can implement dictionaries.

How do we implement a dictionary with a tree?

Let's review the BST data structure to make sure we understand it, and then implement a simple dictionary.


---

# BstNode

```c
typedef struct bst_node_t {
    struct bst_node_t *left, *right;
    void* data;
} BstNode;
```

You've seen it before, but maybe not in C.

Remember: the typedef only works outside the struct, so we need to scope our `bst_node_t` pointer with "struct". 

---

# The data

What is `data`?

It's whetever you want it to be. That's why it's a `void*`.

It could be a string, a tuple, or even an integer casted to a pointer.

When you first learned about BSTs, likely the data was hardcoded to be an `int` or a string. But think back to the database example: a database row is a lot more complicated.

---

# The comparator

First, to make a BST, we need a comparator. A comparator is something that compares two data elements and tells you which one is smaller.

```c
static inline int node_compare_int_default(void* a, void* b) {
    return (intptr_t)a - (intptr_t)b;
}
```

This is a simple comparator that assumes the void* aren't actually pointing to anything useful, and instead they are just reinterpreted as integers (`intptr_t` is guaranteed to be as large as a pointer, but it's signed).

It's important to decide when you're going to do this, vs have the pointer actually point to something. For db-rows we would definitely do the latter:

`return ((SomeRow*)a)->date - ((SomeRow*)b)->date`;

---

# Function pointer reminder

In an OO language the comparator could be an object or a class.

In this case it is a function. It returns < 0 if a is less than b. 0 if equal. > 0 otherwise.

The type looks like this:
`int (*)(void* a, void* b)`

That's a function pointer. And we never, ever use them without a typedef.

---

# Function pointer typedef

`typedef int (*NodeComparer)(void* a, void* b);`

Now the pointer has a name: it's a `NodeComparer`.

How is it used? We create a field of this type in our tree structure, and call it every time we do an insertion or a lookup.

Speaking of the tree structure...

---

# The struct

```c
typedef struct bst_t {
    Pool pool;
    size_t size;
    BstNode* root;
    NodeComparer comparer;
} Bst;
```

The root and size are self-explanatory. The comparer is an instance of that typedef. What about the pool?

It's the pool allocator we saw in the first extra lecture.

[Why are we using the pool?]

---

# The pool

If we didn't use the pool, over time, through insertions and deletions, the tree would become scattered. This is worse for cache locality.

That said, our pool pages are still in a linked list, and trees are non-linear data structures, so it's not like they're guaranteed to be adjacent.

However, we can free the entire tree with a single `pool_destroy` call.

It's not absolutely required to use a pool. Feel free to use `malloc` and `free`. There are other benefits, too, 

---

# Questions?
<!-- _class: invert questions -->

---

# Inserting into the tree

You've already seen a simple insertion method for binary search trees. 

Briefly here is a review of the pseudo-code:
```c
insert(Node** node, value):
    if (!*node)
        *node = create a new node with value
    else if (value < (*node)->value)
        insert ((*node)->left, value);
    else if (value == (*node0)->value)
        *node = deallocate the old value and replace
    else 
        insert((*node)->right, value);
```

Here, we're using a double pointer. Why? [Ideas?]

---

We'll revisit this in a bit with a bit more complexity, but for now, does it make sense?