#pragma once

#include <stdint.h>
#include "defines.h"

// Freeing callback.
typedef void(*dynarray_free_callback)(void*);

// Base type for a simple dynamic array with a freeing function.
typedef struct {
    void* data;
    uint32_t size, capacity, element_size;
    dynarray_free_callback free_callback;
} dynarray_t;

/*
 * First argument is the size of the elements, second argument is the callback to call for freeing them, if necessary.
 * If the callback is null, only free() will be used.  
 */
void dynarray_init(dynarray_t* self, uint32_t element_size, uint32_t array_size, dynarray_free_callback free_callback);
void dynarray_push_back(dynarray_t* self, void* element);
void dynarray_pop_back(dynarray_t* self);
void* dynarray_get(dynarray_t self, uint32_t index);
void dynarray_remove(dynarray_t* self, uint32_t index);
void dynarray_free(dynarray_t self);
