#include "util/defines.h"
#include "util/dynarray.h"
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

    dynarray_t parsed_input = bf_parse_string(input_data);
    free(input_data);

    bf_interpreter_t engine;
    bf_interpreter_init(&engine, stdin, stdout, parsed_input, false);
    bf_interpreter_run(&engine);
    if(engine.performance_info_enabled)
    {
        FILE* performance_output = fopen("./performance.log", "w");
        bf_interpreter_print_performance_info(engine, performance_output);
        fclose(performance_output);
    }
    bf_interpreter_free(engine);
    return EXIT_STATUS_SUCCESS;
}
