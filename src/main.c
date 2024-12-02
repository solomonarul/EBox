#include "util/file.h"
#include "util/meta.h"
#include "bf/parser.h"
#include "bf/engine/jit.h"

#include <stdio.h>
#include <stdlib.h>

uint8_t bf_input_function(void)
{
    uint8_t result;
    fscanf(stdin, "%hhu", &result);
    return result;
}

void bf_output_function(uint8_t character)
{
    fprintf(stdout, "%c", character);
}

int main(int argc, char* argv[])
{
    meta_init(argc, argv);

    if(argc < 2)
    {
        printf("\nError: No input file specified.\nUsage: %s <path_to_file>.\n\n", argv[0]);
        return EXIT_STATUS_FAILURE;
    }

    FILE* input = fopen(argv[1], "rb");
    char* input_data = file_read_all(input);
    fclose(input);

    bf_jit_config_t config = {
        .input_function = bf_input_function,
        .output_function = bf_output_function,
        .program = bf_parse_string(input_data, true, true, true)
    };
    free(input_data);

    bf_jit_t engine;
    bf_jit_init(&engine, config);
    bf_jit_run(&engine);
    dynarray_free(config.program);
    bf_jit_free(engine);

    /*bf_interpreter_config_t config = {
        .input_function = bf_input_function,
        .output_function = bf_output_function,
        .program = bf_parse_string(input_data, true, true, true)
    };
    free(input_data);

    bf_interpreter_t engine;
    bf_interpreter_init(&engine, config);
    bf_interpreter_run(&engine);
    dynarray_free(config.program);
    bf_interpreter_free(engine);*/
    return EXIT_STATUS_SUCCESS;
}
