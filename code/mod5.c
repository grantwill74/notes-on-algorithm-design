#include "pool.h"
#include "trees.h"
#include <assert.h>

void bst_init(Bst* bst, size_t node_size, size_t page_size, NodeComparer cmp) {
    pool_init(&bst->pool, node_size, page_size);
    bst->size = 0;
    bst->root = 0;
    bst->comparer = cmp;
}

void bst_destroy(Bst* bst) {
    bst->root = 0;
    bst->size = 0;
    pool_destroy(&bst->pool);
}

FindResult bst_find_node(NodeComparer cmp, BstNode** starting_from, void* data){
    BstNode** slot = starting_from;
    BstNode* parent = NULL;
    TreeDir dir = TREE_LEFT;

    while (*slot) {
        int cmp_res = cmp(data, (*slot)->data);
        if (cmp_res != 0) {
            dir = cmp_res > 0;
            parent = *slot;
            slot = &(*slot)->children[dir];
        }
        else break;
    }

    return (FindResult) {.loc = slot, .parent = parent, .which_child = dir};
}

void* bst_insert(Bst* bst, void* data) {
    FindResult f = bst_find_node(bst->comparer, &bst->root, data);
     
    if (*f.loc) {
        void* old_data = (*f.loc)->data;
        (*f.loc)->data = data;
        return old_data;
    }

    BstNode* n = pool_alloc(&bst->pool);
    n->children[0] = n->children[1] = NULL;
    n->data = data;
    n->parent = f.parent;

    *f.loc = n;

    return NULL;
}

void* bst_lookup(Bst* bst, void* data) {
    FindResult f = bst_find_node(bst->comparer, &bst->root, data);

    if(!*f.loc) return NULL;

    return (*f.loc)->data;
}

void avl_init(Bst* tree, size_t page_size, NodeComparer cmp) {
    bst_init(tree, sizeof (BstNode), page_size, cmp);
}

void avl_destroy(Bst *avl) {
    bst_destroy(avl);
}

void bst_rotate(BstNode** root, TreeDir dir) {
    assert ("root is not null and points to a valid node" && root && *root);

    BstNode* old_root = *root;
    BstNode* new_root = old_root->children[!dir];
    BstNode* new_inner_child = new_root->children[dir];

    *root = new_root;
    new_root->children[dir] = old_root;
    old_root->children[!dir] = new_inner_child;

    new_root->parent = old_root->parent;
    old_root->parent = new_root;
    if (new_inner_child)
        new_inner_child->parent = old_root;
}

void avl_insert(Bst* tree, void* data) {
    // we're not using this right now
    assert ("unfinished" && 0);
}



// unit tests //////////////////////////////////////////////////////////////////
#include "test.h"
#include "util.h"

static char * test_insert_empty() {
    Bst bst;
    bst_init(&bst, sizeof(BstNode), 1024, node_compare_int_default);

    int x = 42;
    void *ret = bst_insert(&bst, &x);
    BstNode* root = bst.root;
    mu_assert("Expected NULL on first insert", ret == NULL);
    mu_assert("Root should now point to x", root->data == &x);
    mu_assert("Leaf nodes should have no children",
              root->children[TREE_LEFT] == NULL && 
              root->children[TREE_RIGHT] == NULL);

    bst_destroy(&bst);

    return 0;
}

static char * test_insert_lr() {
    Bst bst;
    bst_init(&bst, sizeof(BstNode), 1024, node_compare_int_default);

    bst_insert(&bst, (void*)10);
    bst_insert(&bst, (void*)5);
    bst_insert(&bst, (void*)15);

    bst_insert(&bst, (void*)13);
    bst_insert(&bst, (void*)9);

    mu_assert("Root data should be 10", bst.root->data == (void*)10);
    mu_assert("Left child should be 5", 
        bst.root->children[TREE_LEFT]->data == (void*)5);
    mu_assert("Right child should be 15", 
        bst.root->children[TREE_RIGHT]->data == (void*) 15);

    mu_assert("Right left should be 13",
        bst.root->children[TREE_RIGHT]->children[TREE_LEFT]->data == 
        (void*) 13);
    mu_assert("left right should be 9",
        bst.root->children[TREE_LEFT]->children[TREE_RIGHT]->data == (void*) 9);

    bst_destroy(&bst);

    return 0;
}

static char * test_insert_duplicate() {
    Bst bst;
    bst_init(&bst, sizeof(BstNode), 1024, node_compare_int_default);

    bst_insert(&bst, (void*)10);
    
    mu_assert("First insert returns NULL", bst_insert(&bst, (void*)5) == NULL);
    void* old = bst_insert(&bst, (void*)5);

    mu_assert("re-insert same key", bst_insert(&bst, (void*)5) == old);

    bst_destroy(&bst);
    
    return 0;
}

static char* test_lookup() {
    Bst bst;
    bst_init(&bst, sizeof(BstNode), 1024, node_compare_int_default);

    bst_insert(&bst, (void*)10);
    mu_assert("lookup root", bst_lookup(&bst, (void*)10));

    bst_insert(&bst, (void*)5);
    mu_assert("lookup lc", bst_lookup(&bst, (void*)5));

    bst_insert(&bst, (void*)15);
    mu_assert("lookup rc", bst_lookup(&bst, (void*)15));

    bst_insert(&bst, (void*)13);
    mu_assert("lookup rlc", bst_lookup(&bst, (void*)13));

    mu_assert("lookup null", !bst_lookup(&bst, (void*)99));

    bst_destroy(&bst);

    return 0;
}


char* run_tests() {
    mu_run(test_insert_empty);
    mu_run(test_insert_lr);
    mu_run(test_insert_duplicate);
    mu_run(test_lookup);

    return 0;
}

int main() {
    return do_unit_tests(run_tests);
}


