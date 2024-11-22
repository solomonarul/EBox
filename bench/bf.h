#pragma once

#include "bf/parser.h"
#include "bf/interpreter.h"

#include "util/file.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void bf_bench(const char* path, const char* input_path)
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

    dynarray_t parsed_input = bf_parse_string(input_data, false);
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
    if(input_path != NULL)
        fclose(input_file);
    double final_time = ((double)time) / CLOCKS_PER_SEC;
    if(final_time < 0.01)
        printf("\t%s - %.2fms - %d instr\n", &path[index], final_time * 1000, parsed_input.size);
    else
        printf("\t%s - %.2fs - %d instr\n", &path[index], final_time, parsed_input.size);
    
    bf_interpreter_free(engine);
}

void bf_run_bench(void)
{
    bf_bench("./samples/bf/collatz.b", "./samples/bf/collatz.in");
    bf_bench("./samples/bf/hanoi.b", NULL);
    bf_bench("./samples/bf/factor.b", "./samples/bf/factor.in");
    bf_bench("./samples/bf/mandlebrot.b", NULL);
}
