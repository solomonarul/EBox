#include "util/defines.h"
#include "util/dynarray.h"
#include "util/file.h"
#include "bf/parser.h"
#include "bf/interpreter.h"

#include <stdlib.h>
#include <time.h>

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
    bf_interpreter_init(&engine, stdin, NULL, parsed_input, true);
    bf_interpreter_run(&engine);
    if(engine.performance_info_enabled)
        for(uint32_t index = 0; index < engine.performance_info.size; index++)
        {
            bf_interpreter_perfomance_info_t* current_element;
            current_element = dynarray_get(engine.performance_info, index);
            printf("%d | %d | ", current_element->PC, current_element->count);
            
            // Let's print the sequence.
            bf_instruction_t* current_instruction;
            for(uint32_t program_index = current_element->PC; program_index < engine.program.size; program_index++)
            {
                current_instruction = dynarray_get(engine.program, program_index);
                switch(current_instruction->type)
                {
                case ADD:
                    printf("(a ");
                    break;
                case MOV:
                    printf("(m ");
                    break;
                case JMP:
                    printf("(j ");
                    break;
                case OUT:
                    printf("(o ");
                    break;
                case IN:
                    printf("(i ");
                    break;
                }
                printf("%d) ", current_instruction->args);
                if(current_instruction->type == JMP && current_instruction->args < 0)
                    break;
            }
            printf("\n");
        }
    bf_interpreter_free(engine);
    return EXIT_STATUS_SUCCESS;
}