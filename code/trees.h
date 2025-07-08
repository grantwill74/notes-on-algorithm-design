#pragma once

#include "pool.h"
#include <stdint.h>

// return < 0 for less, 0 for equal, 1 for greater
typedef int (*NodeComparer)(void* a, void* b);

// assumes the pointer data is actually an int (not a pointer to an int)
static inline int node_compare_int_default(void* a, void* b) {
    return (intptr_t)a - (intptr_t)b;
}

typedef struct bst_node_t {
    struct bst_node_t *left, *right;
    void* data;
} BstNode;

typedef struct bst_t {
    Pool pool;
    size_t size;
    BstNode* root;
    NodeComparer comparer;
} Bst;

// callback for pre-insert function. passes context along from insert call.
typedef void (*BstPreInsert)(BstNode* about_to_insert, void* context);

// callback for post-insert function. passes context along from insert call.
// returns new subroot.
typedef BstNode* (*BstPostInsert)(BstNode* just_inserted, void* context);

void bst_init(Bst*, size_t page_size, NodeComparer comparer);
void bst_destroy(Bst*);
// on collision: returns data that was originally there (so it can be freed)
void* bst_insert_generic(Bst*, 
    BstNode** at_which,
    void* data, void* context, 
    BstPreInsert pre, BstPostInsert post);
void bst_insert(Bst* tree, void* data);