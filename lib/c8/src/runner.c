#include "runner.h"
#include "interpreter.h"
#include "util/file.h"
#include "util/ini.h"

#include <stdlib.h>

int c8_run_from_ini_section(ini_section_t section)
{
    ini_data_t* input_file = ini_section_get_data(section, "program");
    if(input_file == NULL)
    {
        printf("\nError: Couldn't find program to run in section 'c8' in the configuration file.\n\n");
        return EXIT_STATUS_FAILURE;
    }

    FILE* input = fopen(input_file->value, "rb");
    if(input == NULL)
    {
        printf("\n\nError: Couldn't find CHIP8 program at path: %s\n", input_file->value);
        return EXIT_STATUS_FAILURE;
    }
    uint32_t size = file_get_size(input);
    uint8_t* program = file_read_all(input);
    fclose(input);

    c8_interpreter_config_t config = {
        .program_size = size,
        .program = program
    };

    c8_interpreter_t engine;
    c8_interpreter_init(&engine, config);
    free(program);
    c8_interpreter_run(&engine);
    c8_interpreter_free(engine);

    return EXIT_STATUS_SUCCESS;
}

