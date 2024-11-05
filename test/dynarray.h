#pragma once

#include "util/dynarray.h"

#include <stdio.h>
#include <assert.h>

void dynarray_test(void)
{
    dynarray_t test_array;
    dynarray_init(&test_array, sizeof(uint8_t), 10);

    // 1. Sanity checks.
    assert(test_array.size == 10);
    assert(test_array.capacity == 16);
    for(uint32_t index = 0; index < test_array.size; index++)
        assert(*(uint8_t*)dynarray_get(test_array, index) == 0);
    
    // 2. Get test.
    for(uint32_t index = 0; index < 10; index++)
    {
        uint8_t* current_element = dynarray_get(test_array, index);
        *current_element = index;
    }
    uint8_t sum = 0; UNUSED(sum);
    for(uint32_t index = 0; index < test_array.size; index++)
        sum += *(uint8_t*)dynarray_get(test_array, index);
    assert(sum == 45);

    // 3. Add test.
    for(uint32_t index = 10; index < 21; index++)
        dynarray_add(&test_array, &index);
    
    sum = 0;
    for(uint32_t index = 0; index < test_array.size; index++)
        sum += *(uint8_t*)dynarray_get(test_array, index);
    assert(test_array.size == 21);
    assert(test_array.capacity == 32);
    assert(sum == 210);

    // 4. Pop back test.
    for(uint32_t index = 0; index < 10; index++)
        dynarray_pop_back(&test_array);
    sum = 0;
    for(uint32_t index = 0; index < test_array.size; index++)
        sum += *(uint8_t*)dynarray_get(test_array, index);
    assert(test_array.size == 11);
    assert(test_array.capacity == 16);
    assert(sum == 55);

    // 5. Remove test. TODO

    dynarray_free(test_array);

    printf("\tdynarray - PASSED\n");
}
