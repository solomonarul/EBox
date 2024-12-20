#include "util/defines.h"
#include "util/ini.h"
#include "util/meta.h"
#include "bf/runner.h"
#include "c8/runner.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    // Usage check.
    if(argc < 2)
    {
        printf("\nError: No initialization file specified.\n\tUsage: %s <path_to_file>.\n\n", argv[0]);
        return EXIT_STATUS_FAILURE;
    }
    
    // Global inits.
    meta_init(argc, argv);

    // Check for initialization file's existence.
    FILE* input_config = fopen(argv[1], "r");
    ini_file_t config = ini_file_read(input_config);
    fclose(input_config);

    // Main settings here.
    ini_section_t* section = ini_file_get_section(config, "Emulator");
    if(section == NULL)
    {
        printf("\nError: No 'Emulator' section in the config file.\n\n");
        ini_file_free(config);
        return EXIT_STATUS_FAILURE;
    }

    ini_data_t* core = ini_section_get_data(*section, "core");
    if(core == NULL)
    {
        printf("\nError: No 'core' data found in the 'Emulator' section of the config file.\n\n");
        ini_file_free(config);
        return EXIT_STATUS_FAILURE;
    }

    // Engine check.
    // TODO: maybe generalize this.
    if(strcmp(core->value, "bf") == 0)
    {
        section = ini_file_get_section(config, "bf");
        if(section == NULL)
        {
            printf("\nError: No 'bf' section found in the initialization file.\n\n");
            ini_file_free(config);
            return EXIT_STATUS_FAILURE;
        }

        if(bf_run_from_ini_section(*section) != EXIT_STATUS_SUCCESS)
        {
            printf("\nError: Failed to run the Brainfuck emulator.\n\n");
            ini_file_free(config);
            return EXIT_STATUS_FAILURE;
        }
    }
    else if(strcmp(core->value, "c8") == 0)
    {
        section = ini_file_get_section(config, "c8");
        if(section == NULL)
        {
            printf("\nError: No 'c8' section found in the initialization file.\n\n");
            ini_file_free(config);
            return EXIT_STATUS_FAILURE;
        }

        if(c8_run_from_ini_section(*section) != EXIT_STATUS_SUCCESS)
        {
            printf("\n\nError: Failed to run the CHIP8 emualtor.\n");
            ini_file_free(config);
            return EXIT_STATUS_FAILURE;
        }
    }
    else 
    {
        printf("\nError: Unknown value has been specified for value 'core' in the initialization file.\n\n");
        ini_file_free(config);
        return EXIT_STATUS_FAILURE;
    }
    ini_file_free(config);
    return EXIT_STATUS_SUCCESS;
}
