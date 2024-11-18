#include "util/defines.h"
#include "util/file.h"
#include "bf/parser.h"
#include "bf/interpreter.h"

#include <stdlib.h>

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("\nError: No input file specified.\nUsage: %s <path_to_file>.\n\n", argv[0]);
        return EXIT_STATUS_FAILURE;
    }

    FILE* input = fopen(argv[1], "rb");
    char* input_data = file_read_all(input);
    fclose(input);

    bf_interpreter_config_t config = {
        .input = stdin,
        .output = stdout,
        .performance_info = {
            .enabled = false,
            .log_file = fopen("./performance.log", "w")
        },
        .program = bf_parse_string(input_data)
    };
    free(input_data);

    bf_interpreter_t engine;
    bf_interpreter_init(&engine, config);
    bf_interpreter_run(&engine);
    fclose(config.performance_info.log_file);
    bf_interpreter_free(engine);
    return EXIT_STATUS_SUCCESS;
}
