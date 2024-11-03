#include "util/defines.h"
#include "util/file.h"
#include "bf/parser.h"
#include "bf/interpreter.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("\nError: No input file specified.\nUsage: %s <path_to_file>.\n\n", argv[0]);
        return EXIT_STATUS_FAILURE;
    }

    FILE* input = fopen(argv[1], "r");
    char* input_data = file_read_all(input);
    fclose(input);

    dynarray_t parsed_input = bf_parse_string(input_data);
    free(input_data);

    bf_interpreter_t engine;
    bf_interpreter_init(&engine, stdin, stdout, parsed_input);
    clock_t time; 
    time = clock(); 
    bf_interpreter_run(&engine);
    time = clock() - time;
    bf_interpreter_free(engine);
    printf("\nProgram took %fs to execute.\n\n", ((double)time)/CLOCKS_PER_SEC);
    return EXIT_STATUS_SUCCESS;
}