#pragma once

#include <stddef.h>

// zone-based memory management

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

void* pool_alloc(Pool* pool);
void* pool_calloc(Pool* pool);
void* pool_alloc_d(Pool* pool); // debug version: scribbles 0xBADF00D on page.
void  pool_free(Pool* pool, void* item);

void pool_init(Pool*, 
    size_t cell_size,
    size_t cells_per_page
);

void pool_destroy(Pool*);
