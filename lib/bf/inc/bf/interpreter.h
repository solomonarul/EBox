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
    struct {
        bool enabled;
        FILE* log_file;
    } performance_info;
} bf_interpreter_config_t;

typedef struct {
    bf_interpreter_config_t config;
    dynarray_t performance_info;
    uint16_t PC, I;
    uint8_t* memory;
} bf_interpreter_t;

void bf_interpreter_init(bf_interpreter_t* engine, bf_interpreter_config_t config);
void bf_interpreter_step(bf_interpreter_t* engine);
void bf_interpreter_run(bf_interpreter_t* engine);
void bf_interpreter_print_performance_info(bf_interpreter_t engine, FILE* output);
void bf_interpreter_free(bf_interpreter_t engine);
