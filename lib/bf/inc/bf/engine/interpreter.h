#pragma once

#include <stdbool.h>
#include "util/dynarray.h"

typedef uint8_t (*bf_input_function_t)(void);
typedef void (*bf_output_function_t)(uint8_t);

typedef struct {
    bf_input_function_t input_function;
    bf_output_function_t output_function;
    dynarray_t program;
} bf_interpreter_config_t;

typedef struct {
    bf_interpreter_config_t config;
    uint16_t PC, I;
    uint8_t* memory;
} bf_interpreter_t;

void bf_interpreter_init(bf_interpreter_t* engine, bf_interpreter_config_t config);
void bf_interpreter_step(bf_interpreter_t* engine);
void bf_interpreter_run(bf_interpreter_t* engine);
void bf_interpreter_free(bf_interpreter_t engine);
