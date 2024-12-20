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

void bf_check_hotloops(dynarray_t* result)
{
    typedef struct {
        const bf_instruction_t* instruction_list;
        uint8_t instruction_list_length;
        bf_instruction_t replacement;
    } bf_known_loop_t;

    // 3j xa -1j    
    static const bf_instruction_t CLR_loop[] = {
        {JMP, 3}, {ADD, 0}, {JMP, -1}
    };

    // 6j -1a xm 1a xm -4j
    static const bf_instruction_t ADDCLR_loop[] = {
        {JMP, 6}, {ADD, -1}, {MOV, 0}, {ADD, 1}, {MOV, 0}, {JMP, -4}
    };

    // 3j xm -1j
    static const bf_instruction_t MOVNZ_loop[] = {
        {JMP, 3}, {MOV, 0}, {JMP, -1}
    };

    static uint8_t known_loop_count = 3;

    static const bf_known_loop_t known_loops[] = {
        { CLR_loop, 3, {CLR, 0} },
        { ADDCLR_loop, 6, {ADDCLR, -1} },
        { MOVNZ_loop, 3, {MOVNZ, -1} }
    };

    for(uint32_t index = 0; index < known_loop_count; index++)
    {
        const bf_known_loop_t current_loop = known_loops[index];
        if(result->size < current_loop.instruction_list_length)
            continue;
        uint8_t ok = 1;
        for(uint32_t inner_index = 0; inner_index < current_loop.instruction_list_length && ok; inner_index++)
        {
            const bf_instruction_t* current_instruction = dynarray_get(*result, result->size - current_loop.instruction_list_length + inner_index);
            if(!(current_loop.instruction_list[inner_index].type == current_instruction->type &&
                (current_loop.instruction_list[inner_index].args == 0 || current_loop.instruction_list[inner_index].args == current_instruction->args)))
                    ok = 0;
        }
        if(ok == 1)
        {
            if(current_loop.replacement.args >= 0)
            {
                for(uint32_t inner_index = 0; inner_index < current_loop.instruction_list_length; inner_index++)
                    dynarray_pop_back(result);
                dynarray_push_back(result, (void*)&current_loop.replacement);
                return;
            }

            bf_instruction_t replacement_value = current_loop.replacement;
            for(uint32_t inner_index = 0; inner_index < current_loop.instruction_list_length && ok; inner_index++)
                if(current_loop.instruction_list[inner_index].args == 0)
                {
                    replacement_value.args++;
                    if(replacement_value.args == 0)
                    {
                        const bf_instruction_t* current_instruction = dynarray_get(*result, result->size - current_loop.instruction_list_length + inner_index);
                        replacement_value.args = current_instruction->args;
                        for(uint32_t inner_index = 0; inner_index < current_loop.instruction_list_length; inner_index++)
                            dynarray_pop_back(result);
                        dynarray_push_back(result, (void*)&replacement_value);
                        return;
                    }
                }
        }
    }
}

dynarray_t bf_parse_string(const char* input, bool optimize, bool has_input, bool has_output)
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
            if(!has_output)
                break;
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
                    if(index == result.size - 1)
                    {
                        dynarray_pop_back(&result);
                        break;
                    }

                    current_instruction->args = result.size - index + 1;
                    instruction.type = JMP;
                    instruction.args = index - result.size + 1;
                    dynarray_push_back(&result, &instruction);
                    break;
                }
            }
            
            // TODO: unmatched [.
            if(optimize)
                bf_check_hotloops(&result);
            break;
        }

        case ',':
            if(!has_input)
                break;

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
