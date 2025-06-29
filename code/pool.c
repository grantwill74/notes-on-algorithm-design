#include "pool.h"
#include "test.h"
#include "bench.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>

/*
Start over with modal approach:
    Forget avoiding allocation. There shouldn't be a "full" state.
    Instead, have two states: bump and gaps.

    We will be in gaps state if next pointer is not null
    Otherwise we will be in bump state.

    It will start in bump state.
    In bump state:
        alloc with room: bump
        alloc full: new page and set bump pointer
        dealloc: write 0 and set next pointer, now in gaps state
    
    In gaps state:
        alloc with gaps: move next pointer, return previous next pointer
        alloc with no gaps: bump
        dealloc: write next pointer, stay in gaps state
*/

typedef struct bump_state_t {
    void** next;
} BumpState;

typedef struct gaps_state_t {
    void** next;
} GapsState;

typedef struct page_t {
    struct page_t* next_page;
    void** page_data;
} Page;

typedef struct pool_t {
    size_t cell_size;
    size_t cells_per_page;
    size_t n_pages;
    size_t n_allocd;
    size_t n_cells;
    
    GapsState gaps;
    BumpState bump;

    Page *first_page, *last_page;
} Pool;


void pool_new_page(Pool* pool) {
    // only create a new page when absolutely full
    assert(pool->n_allocd == pool->n_cells);
    assert(!pool->gaps.next);

    Page* page = malloc(sizeof(Page));
    page->page_data = malloc(pool->cell_size * pool->cells_per_page);
    page->next_page = NULL;
    pool->n_pages++;
    pool->n_cells += pool->cells_per_page;

    if (pool->last_page)
        pool->last_page->next_page = page;

    pool->last_page = page;

    if(!pool->first_page)
        pool->first_page = pool->last_page;

    // reset bump allocator to use this page
    pool->bump.next = page->page_data;
}

// if n_pages_preallocated == 0, start full. otherwise avail.
// prealloc = 0 -> nopage, prealloc = 1 -> onepage, otherwise -> manypage
void pool_init(
    Pool* pool, 
    size_t cell_size,
    size_t cells_per_page
) {
    if(cells_per_page == 0) {
        fprintf(stderr, "Panic: created pool with cells_per_page = 0.\n");
        exit(EXIT_FAILURE);
    }

    // ensure that a cell can hold a pointer, so we have room for the next cell
    pool->cell_size = cell_size >= sizeof(void*) ? cell_size : sizeof(void*);
    pool->cells_per_page = cells_per_page;
    pool->first_page = pool->last_page = NULL;
    pool->n_pages = pool->n_allocd = pool->n_cells = 0;
    pool->gaps.next = NULL;
    pool->bump.next = NULL;

    pool_new_page(pool);
}

void* pool_alloc(Pool* pool) {
    pool->n_allocd++;
    if(pool->n_allocd == pool->n_cells) {
        pool_new_page(pool);
    }

    void* result = NULL;

    if (pool->gaps.next) {
        result = pool->gaps.next;
        pool->gaps.next = *pool->gaps.next;
    } else {
        result = pool->bump.next;
        char* next_bump = (char*)pool->bump.next + pool->cell_size;
        pool->bump.next = (void**)next_bump;
    }

    return result;
}

void* pool_alloc_d(Pool* pool) {
    uint64_t* cell = pool_alloc(pool);

    uint64_t* end = cell + pool->cell_size / sizeof(void*);

    for(uint64_t* p = cell; p < end; p++) {
        *p = 0xBADF00D;
    }

    return cell;
}

void pool_free(Pool *pool, void *item) {
    // write next into item's cell, and set next to item's cell
    *(void**)item = pool->gaps.next;
    pool->gaps.next = item;
    pool->n_allocd--;
}

void pool_destroy(Pool* pool) {
    while(pool->first_page) {
        Page* page = pool->first_page;
        free(page->page_data); page->page_data = NULL;
        
        pool->first_page = page->next_page;
        page->next_page = NULL;
        free(page);
        
        pool->n_pages--;
    }

    memset(pool, 0, sizeof(Pool));
}


// unit tests //////////////////////////////////////////////////////////////////
char* test_cellsize_roundup(void) {
    Pool pool;
    pool_init(&pool, 4, 4);
    mu_assert("pool does not round up cellsize to sizeof(void*)",
        pool.cell_size == sizeof(void*));
    pool_destroy(&pool);

    return 0;
}

char* test_pool_init(void) {
    Pool pool;
    pool_init(&pool, 8, 2);

    pool_new_page(&pool);
    mu_assert("new page", pool.first_page == pool.last_page);
    mu_assert("cell increment", pool.n_cells == 2);

    pool_destroy(&pool);

    return 0;
}

char* test_pool_alloc_d(void) {
    Pool pool;
    pool_init(&pool, sizeof (uint64_t) * 2, 1);

    uint64_t (*dat)[2] = pool_alloc_d(&pool);

    mu_assert("debug allocation scribbled", 
        (*dat)[0] == 0xBADF00D && (*dat)[1] == 0xBADF00D );

    pool_destroy(&pool);

    return 0;
}

char* test_pool_alloc_free_after_zeropage(void) {
    Pool pool;
    pool_init(&pool, sizeof (uint64_t), 1);

    uint64_t* v = pool_alloc_d(&pool);

    mu_assert("debug allocation successful", *v == 0xBADF00D);

    pool_free(&pool, v);

    mu_assert("freeing successful", pool.gaps.next == (void**)v);

    uint64_t* v2 = pool_alloc_d(&pool);

    mu_assert("re-alloc successful", v2 == v);

    pool_destroy(&pool);

    return 0;
}

char* test_pool_multi_realloc(void) {
    Pool pool;
    pool_init(&pool, sizeof (uint64_t), 2);

    uint64_t* dat[4];
    for (size_t n = 0; n < 10; n++) {
        for (size_t i = 0; i < 4; i++) {
            dat[i] = pool_alloc(&pool);
        }
        mu_assert("no gaps", !pool.gaps.next );
        for (size_t i = 0; i < 4; i++) {
            pool_free(&pool, dat[i]);
            mu_assert("pool has gap", pool.gaps.next);
        }
    }

    mu_assert("pool has zero allocated", pool.n_allocd == 0);
    mu_assert("pool has right number of pages", pool.n_pages == 3);
    mu_assert("pool is in right page state",
        pool.first_page && pool.last_page && pool.first_page != pool.last_page);
    
    pool_destroy(&pool);

    return 0;
}

char err_buf[1204];
char* test_pool_page_chaining(void) {
    Pool pool;
    pool_init(&pool, sizeof (uint64_t), 1);

    uint64_t* dat[4];
    Page* pages[4];
    
    for (int i = 0; i < 4; i++) {
        pages[i] = pool.last_page;
        dat[i] = pool_alloc(&pool);
    }

    Page* current_page = pool.first_page;
    for (int i = 0; i < 4; i++) {
        sprintf(err_buf, "page %d exists and unique after alloc", i);
        mu_assert_str(err_buf, pages[i] == current_page);
        current_page = current_page->next_page;
    }

    for (int i = 0; i < 4; i++) {
        pool_free(&pool, dat[i]);
    }

    current_page = pool.first_page;
    for (int i = 0; i < 4; i++) {
        sprintf(err_buf, "page %d unaffected after free", i);
        mu_assert_str(err_buf, pages[i] == current_page);
        current_page = current_page->next_page;
    }

    pool_destroy(&pool);

    return 0;
}

char* test_pool_drains_gaps_first(void) {
    Pool pool;
    pool_init(&pool, sizeof (uint64_t), 3);

    uint64_t* objs[10];

    for (int i = 0; i < 10; i++) {
        objs[i] = pool_alloc(&pool);
    }

    for (int i = 0; i < 3; i++) {
        pool_free(&pool, objs[i]);
        uint64_t* next = (uint64_t*)pool.gaps.next;
        mu_assert("gap has object addr", objs[i] == next);
    }

    for (int i = 0; i < 3; i++) {
        pool_alloc(&pool);
    }

    mu_assert("there is no gap", !pool.gaps.next);

    pool_destroy(&pool);

    return 0;
}


char* all_tests(void) {
    mu_run(test_cellsize_roundup);
    mu_run(test_pool_alloc_d);
    mu_run(test_pool_alloc_free_after_zeropage);
    mu_run(test_pool_multi_realloc);
    mu_run(test_pool_page_chaining);
    mu_run(test_pool_drains_gaps_first);

    return 0;
}

typedef struct test_node_t {
    struct test_node_t *left, *right;
    int data;
} TestNode;

void bench_pool_10000_allocs_and_free(volatile int* sink, void* data) {
    TestNode** nodes = data;
    Pool pool;
    pool_init(&pool, sizeof(TestNode), 10001);

    for (int i = 0; i < 10000; i++) {
        nodes[i] = pool_alloc(&pool);
        nodes[i]->data = rand() % INT_MAX;
    }

    *sink += nodes[rand() % 10000]->data;
    
    pool_destroy(&pool);
}

void bench_pool_10000_allocs_and_10000_frees(volatile int* sink, void* data) {
    TestNode** nodes = data;
    Pool pool;
    pool_init(&pool, sizeof(TestNode), 10001);

    for (int i = 0; i < 10000; i++) {
        nodes[i] = pool_alloc(&pool);
        nodes[i]->data = rand() % INT_MAX;
    }

    *sink += nodes[rand() % 10000]->data;

    for(int i = 0; i < 10000; i++) {
        pool_free(&pool, nodes[i]);
    }
    
    pool_destroy(&pool);
}

void bench_malloc_free_10000(volatile int* sink, void* data) {
    TestNode** nodes = data;

    for (int i = 0; i < 10000; i++) {
        nodes[i] = malloc(sizeof(TestNode));
        nodes[i]->data = rand() % INT_MAX;
    }

    *sink += nodes[rand() % 10000]->data;

    for (int i = 0; i < 10000; i++) {
        free(nodes[i]);
    }
}

void do_benches(void) {
    TestNode* node_storage[10000];
    BenchResult pool_result, malloc_result;
    int sink = 0;

    puts("Bench: pool allocations.............");
    pool_result = do_bench(&sink, 10000, &node_storage, NULL, 
        bench_pool_10000_allocs_and_free, NULL);
    printf("Avg time: %lf nanos, %lf stdev\n\n",
        pool_result.mean_nanos, pool_result.stdev);

    puts("Bench: pool allocation and individual frees.....");
    pool_result = do_bench(&sink, 10000, &node_storage, NULL, 
        bench_pool_10000_allocs_and_10000_frees, NULL);
    printf("Avg time: %lf nanos, %lf stdev\n\n",
        pool_result.mean_nanos, pool_result.stdev);

    puts("Bench: malloc allocations.............");
    malloc_result = do_bench(&sink, 10000, node_storage, NULL,
        bench_malloc_free_10000, NULL);
    printf("Avg time: %lf nanos, %lf stdev\n\n",
        malloc_result.mean_nanos, malloc_result.stdev);
}


#ifdef TESTING_POOL
int main() {
    if (do_unit_tests(all_tests))
        return 1;

    do_benches();

    return 0;
}
#endif
