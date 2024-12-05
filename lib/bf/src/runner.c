#include "runner.h"

#include "parser.h"
#include "util/file.h"
#include "engine/jit.h"
#include "engine/interpreter.h"
#include <string.h>

uint8_t bf_cli_input(void)
{
    char result;
    fscanf(stdin, "%c", &result);
    return result;
}

void bf_cli_output(uint8_t character)
{
    fprintf(stdout, "%c", character);
}

int bf_run_from_ini_section(ini_section_t section)
{
    ini_data_t* program = ini_section_get_data(section, "program");

    if(program == NULL)
    {
        printf("\nError: No 'program' data found in the 'bf' section.\n\n");
        return EXIT_STATUS_FAILURE;
    }

    FILE* input = fopen(program->value, "r");
    if(input == NULL)
    {
        printf("\nError: Failed to open the program file: '%s'.\n\n", program->value);
        return EXIT_STATUS_FAILURE;
    }
    char* input_data = file_read_all(input);
    fclose(input);

    ini_data_t* engine_mode = ini_section_get_data(section, "engine");
    
    // TODO: parse the inputs and the outputs.
    
    if(strcmp(engine_mode->value, "jit") == 0)
    {
        bf_jit_config_t config = {
            .input_function = bf_cli_input,
            .output_function = bf_cli_output,
            .program = bf_parse_string(input_data, true, true, true)
        };
        free(input_data);

        if(config.program.size == 0)
        {
            printf("\nError: Failed to parse the Brainfuck program.\n\n");
            return EXIT_STATUS_FAILURE;
        }

        bf_jit_t engine;
        bf_jit_init(&engine, config);
        dynarray_free(config.program);
        bf_jit_run(&engine);
        bf_jit_free(engine);
    }
    else if(strcmp(engine_mode->value, "interpreter") == 0)
    {
        bf_interpreter_config_t config = {
            .input_function = bf_cli_input,
            .output_function = bf_cli_output,
            .program = bf_parse_string(input_data, true, true, true)
        };
        free(input_data);

        if(config.program.size == 0)
        {
            printf("\nError: Failed to parse the Brainfuck program.\n\n");
            return EXIT_STATUS_FAILURE;
        }

        bf_interpreter_t engine;
        bf_interpreter_init(&engine, config);
        bf_interpreter_run(&engine);
        dynarray_free(config.program);
        bf_interpreter_free(engine);
    }
    else
    {
        printf("\nError: Invalid engine mode: '%s'.\n\n", engine_mode->value);
        free(input_data);
        return EXIT_STATUS_FAILURE;
    }

    return EXIT_STATUS_SUCCESS;
}
