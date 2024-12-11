#include "runner.h"
#include "interpreter.h"
#include "util/file.h"
#include "util/ini.h"

#include <stdlib.h>
#include <string.h>

static uint8_t display_data[64 * 32] = {0};

void test_display_function(void)
{
    printf("\x1b[1J");
    printf("\x1b[H");
    for(uint8_t y = 0; y < 32; y++)
    {
        for(uint8_t x = 0; x < 64; x++)
        {
            if(display_data[y * 64 + x] == 1)
                printf("\x1b[7m \x1b[0m");
            else
                printf(" ");
        }
        printf("\n");
    }
}

void test_clear_function(void)
{
    memset(display_data, 0, 64 * 32);
    test_display_function();
}

uint8_t test_draw_function(uint8_t x, uint8_t y, uint8_t* sprite, uint8_t n)
{
    bool collision = false;
    for(uint8_t index = 0; index < n; index++)
    {
        for(uint8_t position = 0; position < 8; position++)
        {
            if(sprite[index] & (1 << position))
            {
                if(display_data[(y + index) * 64 + x + (7 - position)] == 1)
                    collision = true;
                display_data[(y + index) * 64 + x + (7 - position)] ^= 1;
            }
        }
    }

    test_display_function();
    return collision;
}

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
        .program = program,
        .draw_function = test_draw_function,
        .clear_function = test_clear_function
    };

    c8_interpreter_t engine;
    c8_interpreter_init(&engine, config);
    free(program);
    c8_interpreter_run(&engine);
    c8_interpreter_free(engine);

    return EXIT_STATUS_SUCCESS;
}

