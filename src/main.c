#include "util/file.h"
#include "util/meta.h"
#include "bf/parser.h"
#include "bf/jit.h"

#include <stdio.h>
#include <stdlib.h>

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
        .input = stdin,
        .output = stdout,
        .program = bf_parse_string(input_data, true, true, true)
    };
    free(input_data);

    bf_jit_t engine;
    bf_jit_init(&engine, config);
    dynarray_free(config.program);
    bf_jit_run(&engine);
    bf_jit_free(engine);
    return EXIT_STATUS_SUCCESS;
}
