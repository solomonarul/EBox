#pragma once

#include <stdio.h>
#include <stdint.h>
#include "util/dynarray.h"

typedef struct {
    FILE* input;
    FILE* output;
    dynarray_t program;
} bf_jit_config_t;

typedef struct {
    FILE* input;
    FILE* output;
    
    void* code;
    uint32_t code_size;
    uint8_t* memory;
} bf_jit_t;

void bf_jit_init(bf_jit_t* engine, bf_jit_config_t config);
void bf_jit_step(bf_jit_t* engine);
void bf_jit_run(bf_jit_t* engine);
void bf_jit_free(bf_jit_t engine);
