#pragma once

#include "bf/jit.h"
#include "bf/parser.h"
#include "bf/interpreter.h"

#include "util/file.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void bf_bench(const char* path, const char* input_path, bool optimize)
{
    // Get the file name for printing later.
    uint32_t index = 0, size = 0;
    while(path[size++]);
    index = size;
    while(path[index] != '/') index--;
    index++;

    FILE* input = fopen(path, "rb");
    char* input_data = file_read_all(input);
    fclose(input);

    dynarray_t parsed_input = bf_parse_string(input_data, optimize, input_path != NULL, false);
    free(input_data);

    FILE* input_file = NULL;
    if(input_path != NULL)
        input_file = fopen(input_path, "rb");

    bf_interpreter_config_t config = {
        .input = input_file,
        .output = NULL,
        .performance_info = {
            .enabled = false,
            .log_file = NULL
        },
        .program = parsed_input
    };
    bf_interpreter_t engine;
    bf_interpreter_init(&engine, config);
    clock_t time = clock();
    bf_interpreter_run(&engine);
    time = clock() - time;
    double final_time = ((double)time) / CLOCKS_PER_SEC;
    if(final_time < 0.01)
        printf("\t%s - %.2fms - %d instr\n", &path[index], final_time * 1000, parsed_input.size);
    else
        printf("\t%s - %.2fs - %d instr\n", &path[index], final_time, parsed_input.size);
    
    bf_interpreter_free(engine);

    if(input_path != NULL)
        fseek(input_file, 0, SEEK_SET);

    bf_jit_config_t config_jit = {
        .input = input_file,
        .output = NULL,
        .program = parsed_input
    };
    bf_jit_t engine_jit;
    bf_jit_init(&engine_jit, config_jit);
    time = clock();
    bf_jit_run(&engine_jit);
    time = clock() - time;
    final_time = ((double)time) / CLOCKS_PER_SEC;
    if(final_time < 0.01)
        printf("\t%s (JIT) - %.2fms - %d instr\n", &path[index], final_time * 1000, parsed_input.size);
    else
        printf("\t%s (JIT) - %.2fs - %d instr\n", &path[index], final_time, parsed_input.size);
    bf_jit_free(engine_jit);

    if(input_path != NULL)
        fclose(input_file);
    dynarray_free(parsed_input);
}

void bf_run_bench(void)
{
    bf_bench("./samples/bf/mandlebrot.b", NULL, false);
    bf_bench("./samples/bf/hanoi.b", NULL, false);
    bf_bench("./samples/bf/factor.b", "./samples/bf/factor.in", false);
    bf_bench("./samples/bf/collatz.b", "./samples/bf/collatz.in", false);
}
