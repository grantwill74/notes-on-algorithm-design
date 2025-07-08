#include "trees.h"
#include <assert.h>

void bst_init(Bst* bst, size_t page_size, NodeComparer comparer) {
    pool_init(&bst->pool, sizeof(BstNode), page_size);
    bst->size = 0;
    bst->root = 0;
    bst->comparer = comparer;
}

void bst_destroy(Bst* bst) {
    bst->root = 0;
    bst->size = 0;
    pool_destroy(&bst->pool);
}

void* bst_insert_generic(
    Bst* bst, BstNode** sub_root, void* data, void* context, 
    BstPreInsert pre, BstPostInsert post
) {
    if (!*sub_root) {
        BstNode* node = pool_alloc(&bst->pool);
        node->data = data;
        node->left = node->right = 0;
        bst->size++;

        if (pre) pre(node, context);
        *sub_root = post ? post(node, context) : node;
        return 0;
    }

    assert(*sub_root && "sub root must exist if root exists");

    int cmp = bst->comparer(data, (*sub_root)->data);

    if (pre) pre(*sub_root, context);
    void* ret = NULL;

    if (cmp == 0) {
        ret = (*sub_root)->data; 
        (*sub_root)->data = data;
    }
    else if (cmp < 0) {
        ret = bst_insert_generic(bst,
            &(*sub_root)->left, data, context, pre, post);
    }
    else {
        ret = bst_insert_generic(bst,
            &(*sub_root)->right, data, context, pre, post);
    }
    
    if (post) *sub_root = post(*sub_root, context);
    return ret;
}

void bst_insert(Bst* tree, void* data) {
    bst_insert_generic(tree, &(tree->root), data, NULL, NULL, NULL);
}


// unit tests //////////////////////////////////////////////////////////////////
#include "test.h"
#include "util.h"

static char * test_insert_empty() {
    Bst bst;
    bst_init(&bst, 1024, node_compare_int_default);
    BstNode *root = NULL;

    int x = 42;
    void *ret = bst_insert_generic(&bst, &root, &x, NULL, NULL, NULL);
    mu_assert("Expected NULL on first insert", ret == NULL);
    mu_assert("Root should now point to x", root->data == &x);
    mu_assert("Leaf nodes should have no children",
              root->left == NULL && root->right == NULL);

    bst_destroy(&bst);

    return 0;
}

static char * test_insert_lr() {
    Bst bst;
    bst_init(&bst, 1024, node_compare_int_default);

    bst_insert_generic(&bst, &(bst.root), (void*)10, NULL, NULL, NULL);
    bst_insert_generic(&bst, &(bst.root), (void*)5,  NULL, NULL, NULL);
    bst_insert_generic(&bst, &(bst.root), (void*)15, NULL, NULL, NULL);

    bst_insert_generic(&bst, &(bst.root), (void*)13, NULL, NULL, NULL);
    bst_insert_generic(&bst, &(bst.root), (void*)9, NULL, NULL, NULL);

    mu_assert("Root data should be 10", bst.root->data == (void*)10);
    mu_assert("Left child should be 5",    bst.root->left->data == (void*)5);
    mu_assert("Right child should be 15",  bst.root->right->data == (void*) 15);

    mu_assert("Right left should be 13",
        bst.root->right->left->data == (void*) 13);
    mu_assert("left right should be 9",
        bst.root->left->right->data == (void*) 9);

    bst_destroy(&bst);

    return 0;
}

static char * test_insert_duplicate() {
    Bst bst;
    bst_init(&bst, 1024, node_compare_int_default);

    bst_insert_generic(&bst, &(bst.root), (void*)10, NULL, NULL, NULL);
    
    mu_assert("First insert returns NULL",
              bst_insert_generic(&bst, 
                &(bst.root), (void*)5, NULL, NULL, NULL) == NULL);
    void* old = 
        bst_insert_generic(&bst, &(bst.root), (void*)5,  NULL, NULL, NULL);
    mu_assert("re-insert same key",
              bst_insert_generic(&bst, 
                &(bst.root), (void*)5, NULL, NULL, NULL) == old);

    bst_destroy(&bst);
    
    return 0;
}

void record_node(BstNode* node, void* chain_) {
    Slice* slice = chain_;
    BstNode** nodes = slice->arr;
    nodes[slice->n] = node;
    slice->n++; 
}

BstNode* count_node(BstNode* node, void* count_) {
    size_t* count = count_;
    (*count)++;
    return node;
}

static char * test_insert_pre() {
    Bst bst;
    bst_init(&bst, 1024, node_compare_int_default);

    BstNode* nodes[2] = {};
    Slice context = {nodes, 0};

    bst_insert_generic(&bst, &(bst.root), (void*)10, &context, NULL, 0);
    bst_insert_generic(&bst, &(bst.root), (void*)5, &context, record_node, 0);

    mu_assert("context root", nodes[0]->data == (void*)10);
    mu_assert("left child of root", nodes[1] == bst.root->left);
    mu_assert("first call lc", nodes[1]->data == (void*)5);
    mu_assert("slice size", context.n == 2);
    context.n = 0;

    bst_insert_generic(&bst, &(bst.root), (void*)15, &context, record_node, 0);
    
    mu_assert("context root 2nd call", nodes[0]->data == (void*)10);
    mu_assert("right child of root", nodes[1] == bst.root->right);
    mu_assert("rc fourth call", nodes[1]->data == (void*)15);
    mu_assert("slice size 2", context.n == 2);

    bst_destroy(&bst);

    return 0;
}

static char * test_insert_post() {
    Bst bst;
    bst_init(&bst, 1024, node_compare_int_default);

    size_t count = 0;

    bst_insert_generic(&bst, &(bst.root), (void*)10, &count, NULL, count_node);
    mu_assert("root count is 1", count == 1);
    count = 0;

    bst_insert_generic(&bst, &(bst.root), (void*)5, &count, NULL, count_node);
    mu_assert("lc count is 2", count == 2);
    count = 0;

    bst_insert_generic(&bst, &(bst.root), (void*)3, &count, NULL, count_node);
    mu_assert("llc count is 3", count == 3);
    count = 0;

    bst_insert_generic(&bst, &(bst.root), (void*)15, &count, NULL, count_node);
    mu_assert("rc count is 2", count == 2);

    bst_destroy(&bst);

    return 0;
}

char* run_tests() {
    mu_run(test_insert_empty);
    mu_run(test_insert_lr);
    mu_run(test_insert_duplicate);
    mu_run(test_insert_pre);
    mu_run(test_insert_post);

    return 0;
}

int main() {
    return do_unit_tests(run_tests);
}


