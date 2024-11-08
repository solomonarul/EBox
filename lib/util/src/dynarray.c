#include "dynarray.h"

#include <stdlib.h>
#include <string.h>

void dynarray_init(dynarray_t* self, uint32_t element_size, uint32_t array_size)
{
    self->element_size = element_size;
    self->size = array_size;
    self->capacity = 1;
    while(self->capacity <= self->size)
        self->capacity *= 2;
    self->data = calloc(self->capacity, element_size);
}

void* dynarray_get(dynarray_t self, uint32_t index)
{
    if(index >= self.size)
        return NULL;
    
    // Pointer to data location.
    return (void*)((char*)self.data + index * self.element_size);
}

void dynarray_add(dynarray_t* self, const void* data)
{
    // We reached the resize point.
    if(self->capacity == self->size)
    {
        self->capacity *= 2;
        self->data = realloc(self->data, self->capacity * self->element_size);
    }
    memcpy((char*)self->data + self->size * self->element_size, data, self->element_size);
    self->size++;
}

void dynarray_pop_back(dynarray_t *self)
{
    if(self->size <= 0)
        return;

    self->size--;
    
    // We reached the resize point.
    if(self->size <= self->capacity / 2 && self->capacity > 1)
    {
        self->capacity /= 2;
        self->data = realloc(self->data, self->capacity * self->element_size);
    }
}

void dynarray_free(dynarray_t self)
{
    free(self.data);
}
