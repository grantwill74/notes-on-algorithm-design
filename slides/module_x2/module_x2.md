---
marp: true
theme: slides
paginate: true
---

# Design and Analysis of Algorithms  

## Module X2: Heaps, Heapsort, and Priority Queues

<br>
<br>

Slides © Grant Williams, [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/).  

<br>

This is an open educational resource.
Feel free to submit fixes, improvements, and new material [here](https://github.com/grantwill74/notes-on-algorithm-design).

---

# Heaps

We are going to need heaps.

Not now, but later.

And now's as good a time as any!

---

# The name is overloaded

Last small-module was about memory management.

There, we talked about *the* heap.

Don't get confused: those are completely different things.

*The* heap refers to a pile of bytes that is organized by whatever allocator you use. It's called the heap because it's like just a pile of data you can request when you need it.

*A* heap refers to a data structure with a specific property...

---

# The heap property

Heaps are binary trees.

Note: they are *not* binary search trees.

Binary search trees have the property that a node's left child has a smaller value than it, and its right child has a larger value than it (if either exist).

[What property do heaps have?]

---

# The heap property (2)

Min-heaps have the property that every element is smaller than its children.
Max-heaps have the property that every element is bigger than its children.

There is another property that practical heaps have that we will mention soon, but the heap property is the main one.

It is also called the "heap invariant", because it is expected to be preserved 