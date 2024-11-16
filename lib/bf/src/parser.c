#include "parser.h"
#include "util/dynarray.h"

#include <stdio.h>

void bf_parse_arg_instruction(dynarray_t* result, bf_instruction_type_t type, int32_t value)
{
    if(result->size != 0)
    {
        bf_instruction_t* current_instruction = dynarray_get(*result, result->size - 1);

        // Update if already existing.
        if(current_instruction->type == type)
        {
            current_instruction->args += value;
            if(current_instruction->args == 0)
                dynarray_pop_back(result);
            return;
        }
    }

    // Add if not existing.
    bf_instruction_t instruction;
    instruction.type = type;
    instruction.args = value;
    dynarray_push_back(result, &instruction);
}

dynarray_t bf_parse_string(const char* input)
{
    dynarray_t result;
    bf_instruction_t instruction;
    dynarray_init(&result, sizeof(bf_instruction_t), 0, NULL);

    for(uint32_t index = 0; input[index]; index++)
    {
        switch(input[index])
        {
        case '+':
            bf_parse_arg_instruction(&result, ADD, 1);
            break;
            
        case '-':
            bf_parse_arg_instruction(&result, ADD, -1);
            break;

        case '>':
            bf_parse_arg_instruction(&result, MOV, 1);
            break;

        case '<':
            bf_parse_arg_instruction(&result, MOV, -1);
            break;

        case '.':
            instruction.type = OUT;
            instruction.args = -1;
            dynarray_push_back(&result, &instruction);
            break;

        case '[':
            instruction.type = JMP;
            instruction.args = 0;
            dynarray_push_back(&result, &instruction);
            break;

        case ']':
        {
            bf_instruction_t* current_instruction;

            // Find previous open bracket.
            for(uint32_t index = result.size - 1; index < result.size; index--)
            {
                current_instruction = dynarray_get(result, index);
                
                // Found it.
                if(current_instruction->type == JMP && current_instruction->args == 0)
                {
                    current_instruction->args = result.size - index + 1;
                    instruction.type = JMP;
                    instruction.args = index - result.size + 1;
                    dynarray_push_back(&result, &instruction);
                    break;
                }
            }
            
            // TODO: unmatched [.
            
            break;
        }
        case ',':
            instruction.type = IN;
            instruction.args = -1;
            dynarray_push_back(&result, &instruction);
            break;

        default:
            // Ignore
            break;
        }
    }

    return result;
}

void bf_print_internal_form(dynarray_t input, FILE* output)
{
    bf_instruction_t* current_instruction;
    for(uint32_t index = 0; index < input.size; index++)
    {
        current_instruction = dynarray_get(input, index);
        switch(current_instruction->type)
        {
        case ADD:
            fprintf(output, "ADD %d\n", current_instruction->args);
            break;

        case MOV:
            fprintf(output, "MOV %d\n", current_instruction->args);
            break;

        case JMP:
            fprintf(output, "JMP %d\n", current_instruction->args);
            break;

        case IN:
            fprintf(output, "IN %d\n", current_instruction->args);
            break;

        case OUT:
            fprintf(output, "OUT %d\n", current_instruction->args);
            break;
        }
    }
}
