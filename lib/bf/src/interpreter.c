#include "interpreter.h"
#include "parser.h"
#include "util/dynarray.h"

#include <stdlib.h>

void bf_interpreter_init(bf_interpreter_t* engine, FILE* input, FILE* output, dynarray_t program, bool enable_performance_info)
{
    engine->input = input;
    engine->output = output;
    engine->program = program;
    engine->I = 0;
    engine->PC = 0;
    engine->performance_info_enabled = enable_performance_info;
    if(engine->performance_info_enabled)
    {
        dynarray_init(&engine->performance_info, sizeof(bf_interpreter_perfomance_info_t), 0);

        // Find all the top level loop sequences.
        for(uint32_t index = 0; index < engine->program.size; index++)
        {
            bf_instruction_t* current_instruction;
            current_instruction = dynarray_get(engine->program, index);
            if(current_instruction->type == JMP && current_instruction->args > 0)
            {
                bf_instruction_t* checked_instruction;
                uint32_t nextIndex = index;
                do {
                    nextIndex++;
                    checked_instruction = dynarray_get(engine->program, nextIndex);
                } while(nextIndex < program.size && checked_instruction->type != JMP);

                // Save if the only jump ahead is backwards, then we are sure the sequence is of type [] and not [[].
                if(nextIndex == program.size || (checked_instruction->type == JMP && checked_instruction->args > 0))
                    continue;

                static bf_interpreter_perfomance_info_t element_to_add;
                element_to_add.count = 0;
                element_to_add.PC = index;
                dynarray_add(&engine->performance_info, &element_to_add);
            }
        }
    }
    engine->memory = calloc(0x10000, sizeof(uint8_t));
}

void bf_interpreter_step(bf_interpreter_t* engine)
{
    bf_instruction_t* instruction;
    instruction = dynarray_get(engine->program, engine->PC);

    switch(instruction->type)
    {
    case ADD:
        engine->memory[engine->I] += instruction->args;
        engine->PC++;
        break;
    case MOV:
        engine->I += instruction->args;
        engine->PC++;
        break;
    case JMP:
        // Jump ahead if value is 0.
        if(instruction->args > 0)
        {
            if(engine->performance_info_enabled)
            {
                // Search linearly trough the sequences and update the right one.
                bf_interpreter_perfomance_info_t* current_element;
                for(uint32_t index = 0; index < engine->performance_info.size; index++)
                {
                    current_element = dynarray_get(engine->performance_info, index);
                    if(current_element->PC == engine->PC)
                    {
                        current_element->count++;
                        break;
                    }
                }
            }
            engine->PC += (engine->memory[engine->I] == 0) ? instruction->args : 1;
        }
        // Jump behind if value is less than 0.
        else
            engine->PC += (engine->memory[engine->I] != 0) ? instruction->args : 1;
        break;
    case IN:
        if(engine->input != NULL)
            fscanf(engine->input, "%c", &engine->memory[engine->I]);
        else
            engine->memory[engine->I] = 0;
        engine->PC++;
        break;
    case OUT:
        if(engine->output != NULL)
            fprintf(engine->output, "%c", engine->memory[engine->I]);
        engine->PC++;
        break;
    }
}

void bf_interpreter_run(bf_interpreter_t* engine)
{
    while(engine->PC < engine->program.size)
        bf_interpreter_step(engine);
}

void bf_interpreter_print_performance_info(bf_interpreter_t engine, FILE* output)
{
    bf_instruction_t* current_instruction;
    bf_interpreter_perfomance_info_t* current_element;

    // Go trough all the sequences.
    for(uint32_t index = 0; index < engine.performance_info.size; index++)
    {
        current_element = dynarray_get(engine.performance_info, index);
        fprintf(output, "%d | %d | ", current_element->PC, current_element->count);
        
        // Prints the sequence.
        for(uint32_t program_index = current_element->PC; program_index < engine.program.size; program_index++)
        {
            current_instruction = dynarray_get(engine.program, program_index);
            fprintf(output, "%d", current_instruction->args);
            switch(current_instruction->type)
            {
            case ADD:
                fprintf(output, "a ");
                break;
            case MOV:
                fprintf(output, "m ");
                break;
            case JMP:
                fprintf(output, "j ");
                break;
            case OUT:
                fprintf(output, "o ");
                break;
            case IN:
                fprintf(output, "i ");
                break;
            }
            if(current_instruction->type == JMP && current_instruction->args < 0)
                break;
        }
        fprintf(output, "\n");
    }
}

void bf_interpreter_free(bf_interpreter_t engine)
{
    free(engine.memory);
    dynarray_free(engine.program);
}
