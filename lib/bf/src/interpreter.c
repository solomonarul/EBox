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

                if(nextIndex == program.size || (checked_instruction->type == JMP && checked_instruction->args > 0))
                    continue;

                static bf_interpreter_perfomance_info_t element_to_add;
                element_to_add.count = 0;
                element_to_add.PC = index;
                dynarray_add(&engine->performance_info, &element_to_add);
            }
        }
    }
    engine->memory = calloc(sizeof(uint8_t), 0x10000);
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

void bf_interpreter_free(bf_interpreter_t engine)
{
    free(engine.memory);
    dynarray_free(engine.program);
}