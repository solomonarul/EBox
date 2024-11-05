#include "parser.h"
#include "util/dynarray.h"

#include <stdio.h>

static bf_instruction_t instruction;
static bf_instruction_t* current_instruction;

void bf_parse_arg_instruction(dynarray_t* result, bf_instruction_type_t type, int32_t value)
{
    if(result->size != 0)
    {
        current_instruction = dynarray_get(*result, result->size - 1);
        if(current_instruction->type == type)
        {
            current_instruction->args += value;
            if(current_instruction->args == 0)
                dynarray_pop_back(result);
            return;
        }
    }
    instruction.type = type;
    instruction.args = value;
    dynarray_add(result, &instruction);
}

dynarray_t bf_parse_string(const char* input)
{
    dynarray_t result;
    dynarray_init(&result, sizeof(bf_instruction_t), 0);

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
            dynarray_add(&result, &instruction);
            break;

        case '[':
            instruction.type = JMP;
            instruction.args = 0;
            dynarray_add(&result, &instruction);
            break;

        case ']':
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
                    dynarray_add(&result, &instruction);
                    break;
                }
            }
            
            // TODO: unmatched ].
            
            break;
        case ',':
            instruction.type = IN;
            instruction.args = -1;
            dynarray_add(&result, &instruction);
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
    for(uint32_t index = 0; index < input.size; index++)
    {
        bf_instruction_t* current_element = (bf_instruction_t*)dynarray_get(input, index);
        switch(current_element->type)
        {
        case ADD:
            fprintf(output, "ADD %d\n", current_element->args);
            break;

        case MOV:
            fprintf(output, "MOV %d\n", current_element->args);
            break;

        case JMP:
            fprintf(output, "JMP %d\n", current_element->args);
            break;

        case IN:
            fprintf(output, "IN %d\n", current_element->args);
            break;

        case OUT:
            fprintf(output, "OUT %d\n", current_element->args);
            break;
        }
    }
}