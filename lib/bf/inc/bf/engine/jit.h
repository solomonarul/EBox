#pragma once

#include <stdint.h>
#include <lightning.h>
#include "util/dynarray.h"
#include "glue.h"

typedef struct {
    bf_input_function_t input_function;
    bf_output_function_t output_function;
    dynarray_t program;
} bf_jit_config_t;

typedef void (*bf_jit_function_t)(void);

typedef struct {
    uint8_t* memory;
    jit_state_t* jit_state;
    bf_jit_function_t code;
} bf_jit_t;

void bf_jit_init(bf_jit_t* engine, bf_jit_config_t config);
void bf_jit_step(bf_jit_t* engine);
void bf_jit_run(bf_jit_t* engine);
void bf_jit_free(bf_jit_t engine);
