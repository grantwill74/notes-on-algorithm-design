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
    struct bst_node_t* children[2];
    struct bst_node_t *parent;
    void* data;
} BstNode;

typedef enum tree_dir_t {
    TREE_LEFT = 0,
    TREE_RIGHT,
} TreeDir;

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

void bst_init(Bst*, size_t node_size, size_t page_size, NodeComparer comparer);
void bst_destroy(Bst*);

typedef struct find_result_t {
    BstNode** loc;
    BstNode* parent;
    TreeDir which_child;
} FindResult;

// find a node, or if it isn't there, where it should be inserted.
FindResult bst_find_node(NodeComparer cmp, BstNode** starting_from, void* data);

// find the parent of a node, or null if the tree is empty
BstNode** bst_find_parent(NodeComparer cmp, BstNode* starting_from, void* data);

// on collision: returns data that was originally there (so it can be freed)
// otherwise returns null.
void* bst_insert(Bst* tree, void* data);
void* bst_lookup(Bst* tree, void* data);

void bst_rotate(BstNode** root, TreeDir dir);

typedef struct avl_node_t {
    BstNode node;
    size_t height;
} AvlNode;

void avl_init(Bst*, size_t page_size, NodeComparer cmp);
void avl_destroy(Bst* avl);
void avl_insert(Bst* tree, void* data);

