#pragma once

#include "defines.h"

typedef struct {
    uint32_t element_size, size, capacity;
    void* data;
} dynarray_t;

void dynarray_init(dynarray_t* self, uint32_t element_size, uint32_t array_size);
void* dynarray_get(dynarray_t self, uint32_t index);
void dynarray_add(dynarray_t* self, const void* data);
void dynarray_pop_back(dynarray_t* self);
void dynarray_free(dynarray_t self);
