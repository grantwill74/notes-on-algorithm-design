
# Old fashioned sorts

In the 1890's, [Herman Hollerith](https://en.wikipedia.org/wiki/Herman_Hollerith) invented the card sorter.

You would record data records on a punch-card, containing, e.g., census information.

[The cards had a grid that could have holes punched](https://en.wikipedia.org/wiki/File:Hollerith_Punched_Card.jpg). For example, if you were tabulating peoples' ages, you could punch a "3" in one row and a "2" in another to record "32", along with other data.

These cards could then be sorted into physical buckets, where a card counter could tabulate them. So you could know the number of people who were 32.

---

# Multiple passes

The operator would place the cards in hoppers and run the machine.

The machine would sort one column into buckets. Say, the leading digit.

The operator could then take 1 bucket, e.g., the 3 bucket, set up 10 empty buckets, set the machine to sort on the next column, and dump the 3 bucket back into the hopper.

Now the new buckets would have all the 30s, 31s, 32s, etc.

These could each be tabulated by a card counter.

---

# What is the big-$\Theta$?

Assume that we can sort into $10$ buckets.

If we have $10$ values to sort, we sort them in one pass.

If we have $100$ values to sort, we sort them in $1 + 10 \times 1 = 11$ passes.
(i.e., $1$ pass to give $10$ buckets of $10$ each. For each one, $1$ pass)

If we have $1000$, we sort them in $1 + 10\times (1 + 10\times 1)=111$

If we have $10000$, we sort them in $1 + 10\times (1 + 10\times (1 + 10))=1111$

---

# A little surprising

Write the recurrence relation:

$T(<= 10) = 1$
$T(n) = 10\timesT(1/10) + 1$


---
