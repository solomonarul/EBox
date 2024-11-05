#pragma once

#include "bf/parser.h"

#include <stdio.h>
#include <assert.h>

void bf_parser_test(void)
{
    const char* test_input = "+++-[>+<].,";
    dynarray_t parsed_input = bf_parse_string(test_input);

    bf_instruction_t expected_result[8] = {
        {
            .type = ADD,
            .args = 2
        },
        {
            .type = JMP,
            .args = 5
        },
        {
            .type = MOV,
            .args = 1
        },
        {
            .type = ADD,
            .args = 1
        },
        {
            .type = MOV,
            .args = -1
        },
        {
            .type = JMP,
            .args = -3
        },
        {
            .type = OUT,
            .args = -1
        },
        {
            .type = IN,
            .args = -1
        }
    }; UNUSED(expected_result);

    for(uint32_t index = 0; index < 8; index++)
        assert(memcmp((bf_instruction_t*)dynarray_get(parsed_input, index), &expected_result[index], sizeof(bf_instruction_t)) == 0);
    
    dynarray_free(parsed_input);

    printf("\tparser - PASSED\n");
}
