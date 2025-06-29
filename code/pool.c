#include "pool.h"
#include "test.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    void** end;
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
    assert(pool->bump.next == pool->bump.end);

    Page* page = malloc(sizeof(Page));
    page->page_data = malloc(pool->cell_size * pool->cells_per_page);
    page->next_page = NULL;
    pool->n_pages++;
    pool->n_cells += pool->cells_per_page;

    pool->last_page = page;
    if(!pool->first_page)
        pool->first_page = pool->last_page;

    // reset bump allocator to use this page
    pool->bump.next = page->page_data;
    char* bump_end = (char*)page->page_data + 
        pool->cell_size * pool->cells_per_page;
    pool->bump.end = (void**)bump_end;
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
    pool->bump.next = pool->bump.end = NULL;

    pool_new_page(pool);
}

void* pool_alloc(Pool* pool) {
    if(pool->n_allocd == pool->n_cells) {
        pool_new_page(pool);
    }

    void* result = NULL;
    pool->n_allocd++;

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
    mu_assert("pool has right number of pages", pool.n_pages == 2);
    mu_assert("pool is in right page state",
        pool.first_page && pool.last_page && pool.first_page != pool.last_page);
    
    pool_destroy(&pool);

    return 0;
}

char* all_tests(void) {
    mu_run(test_cellsize_roundup);
    mu_run(test_pool_alloc_d);
    mu_run(test_pool_alloc_free_after_zeropage);
    mu_run(test_pool_multi_realloc);

    return 0;
}


#ifdef TESTING_POOL
int main() {
    return do_unit_tests(all_tests);
}
#endif
