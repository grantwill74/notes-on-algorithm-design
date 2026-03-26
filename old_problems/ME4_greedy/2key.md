# Key

```c
int cmp(void* a, void* b) {
    Item* ia = a;
    Item* ib = b;
    double vw_a = ia->value / ia->weight;
    double vw_b = ib->value / ib-> weight;
    if (vw_a < vw_b) return 1; // descending order!
    if (vw_a == vw_b) return 0;
    return -1;
}

double max_val(Item* items, size_t n_items, double c) {
    qsort(items, n_items, sizeof(Item), cmp);

    double total_val = 0;
    for (int i = 0; i < n_items; i++) {
        double used_cap = items[i].weight * items[i].quantity;
        used_cap = used_cap > c ? c : used_cap;
        double used_quantity = used_cap / items[i].weight;
        total_val += used_quantity * items[i].value;
        c -= used_cap;

        if (c == 0) break;
    }

    return total_val;
}
```


Grading:
- If solution is not n lg n, -50
- We must be sorting by value / weight, otherwise -50
- We must be sorting biggest first, or iterating in reverse. If neither apply, -50
- If we use a function besides qsort that is not also defined in our answer, - 50
- If we don't check that we're taking too much, -10
- If we don't compute the value correctly, -10
- If we take too much, -10
- For each error with qsort, -5
- If you forget to decrease total capacity, -5

For other similar small errors, -10.

For minor syntax errors like missing semicolons and braces where indentation is clear, no points lost.

