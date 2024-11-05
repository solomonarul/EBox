#pragma once

#include "util/dynarray.h"

#include <stdio.h>
#include <stdbool.h>

typedef struct {
    uint16_t PC;
    uint32_t count;
} bf_interpreter_perfomance_info_t;

typedef struct {
    FILE* input;
    FILE* output;
    dynarray_t program;
    bool performance_info_enabled;
    dynarray_t performance_info;
    uint16_t PC, I;
    uint8_t* memory;
} bf_interpreter_t;

void bf_interpreter_init(bf_interpreter_t* engine, FILE* input, FILE* output, dynarray_t program, bool enable_performance_info);
void bf_interpreter_step(bf_interpreter_t* engine);
void bf_interpreter_run(bf_interpreter_t* engine);
void bf_interpreter_print_performance_info(bf_interpreter_t engine, FILE* output);
void bf_interpreter_free(bf_interpreter_t engine);