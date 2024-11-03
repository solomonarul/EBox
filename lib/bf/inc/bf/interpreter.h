#pragma once

#include "util/dynarray.h"

#include <stdio.h>

typedef struct {
    FILE* input;
    FILE* output;
    dynarray_t program;
    uint16_t PC, I;
    uint8_t* memory;
} bf_interpreter_t;

void bf_interpreter_init(bf_interpreter_t* engine, FILE* input, FILE* output, dynarray_t program);
void bf_interpreter_step(bf_interpreter_t* engine);
void bf_interpreter_run(bf_interpreter_t* engine);
void bf_interpreter_free(bf_interpreter_t engine);