#pragma once

#include <stdio.h>
#include "util/dynarray.h"

enum {
    // Basic instructions.
    ADD = 0u,
    MOV,
    JMP,
    OUT,
    IN,
    // Composite instructions from here.
    CLR,
    ADDCLR
};
typedef uint16_t bf_instruction_type_t;

typedef struct {
    bf_instruction_type_t type;
    int16_t args;
} bf_instruction_t;

dynarray_t bf_parse_string(const char* input);
void bf_print_internal_form(dynarray_t input, FILE* output);
