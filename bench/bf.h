#pragma once

#include "bf/jit.h"
#include "bf/parser.h"
#include "bf/interpreter.h"

#include "util/file.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

double bf_interpreter_bench(const char* path, const char* input_path, bool optimize)
{
    FILE* input = fopen(path, "rb");
    char* input_data = file_read_all(input);
    fclose(input);

    dynarray_t parsed_input = bf_parse_string(input_data, optimize, input != NULL, false);

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
    bf_interpreter_free(engine);

    printf("\t%d instr - ", parsed_input.size);
    dynarray_free(parsed_input);

    if(input_path != NULL)
        fclose(input_file);

    return final_time;
}

double bf_jit_bench(const char* path, const char* input_path, bool optimize)
{
    FILE* input = fopen(path, "rb");
    char* input_data = file_read_all(input);
    fclose(input);

    dynarray_t parsed_input = bf_parse_string(input_data, optimize, input != NULL, false);

    FILE* input_file = NULL;
    if(input_path != NULL)
        input_file = fopen(input_path, "rb");

    bf_jit_config_t config_jit = {
        .input = input_file,
        .output = NULL,
        .program = parsed_input
    };
    bf_jit_t engine_jit;
    bf_jit_init(&engine_jit, config_jit);
    clock_t time = clock();
    printf("\tJIT - ");
    bf_jit_run(&engine_jit);
    time = clock() - time;
    double final_time = ((double)time) / CLOCKS_PER_SEC;
    bf_jit_free(engine_jit);

    printf("%d instr - ", parsed_input.size);
    dynarray_free(parsed_input);

    if(input_path != NULL)
        fclose(input_file);

    return final_time;
}

void bf_bench(const char* path, const char* input_path, bool optimize)
{
    // Get the file name for printing later.
    uint32_t index = 0, size = 0;
    while(path[size++]);
    index = size;
    while(path[index] != '/') index--;
    index++;

    double final_time = bf_jit_bench(path, input_path, optimize);

    if(final_time < 0.01)
        printf("%s - %.2fms\n", &path[index], final_time * 1000);
    else
        printf("%s - %.2fs\n", &path[index], final_time);

    final_time = bf_interpreter_bench(path, input_path, optimize);

    if(final_time < 0.01)
        printf("%s - %.2fms\n", &path[index], final_time * 1000);
    else
        printf("%s - %.2fs\n", &path[index], final_time);
}

void bf_run_bench(void)
{
    bf_bench("./samples/bf/mandlebrot.b", NULL, true);
    bf_bench("./samples/bf/hanoi.b", NULL, true);
    bf_bench("./samples/bf/factor.b", "./samples/bf/factor.in", true);
    bf_bench("./samples/bf/collatz.b", "./samples/bf/collatz.in", true);
}
