#pragma once

#include <stddef.h>

// zone-based memory management

struct pool_t;
typedef struct pool_t Pool;

struct pool_cell_t;

void* pool_alloc(Pool* pool);
void* pool_calloc(Pool* pool);
void* pool_alloc_d(Pool* pool); // debug version: scribbles 0xBADF00D on page.
void  pool_free(Pool* pool, void* item);

void pool_init(Pool*, 
    size_t cell_size,
    size_t cells_per_page
);

void pool_destroy(Pool*);
