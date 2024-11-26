#include "interpreter.h"
#include "parser.h"

#include <stdlib.h>

void bf_interpreter_init(bf_interpreter_t* engine, bf_interpreter_config_t config)
{
    engine->config = config;
    engine->I = 0;
    engine->PC = 0;
    if(engine->config.performance_info.enabled)
    {
        dynarray_init(&engine->performance_info, sizeof(bf_interpreter_perfomance_info_t), 0, NULL);

        // Find all the top level loop sequences.
        for(uint32_t index = 0; index < engine->config.program.size; index++)
        {
            bf_instruction_t* current_instruction;
            current_instruction = dynarray_get(engine->config.program, index);
            if(current_instruction->type == JMP && current_instruction->args > 0)
            {
                bf_instruction_t* checked_instruction;
                uint32_t nextIndex = index;
                do {
                    nextIndex++;
                    checked_instruction = dynarray_get(engine->config.program, nextIndex);
                } while(nextIndex < config.program.size && checked_instruction->type != JMP);

                // Save if the only jump ahead is backwards, then we are sure the sequence is of type [] and not [[].
                if(nextIndex == config.program.size || (checked_instruction->type == JMP && checked_instruction->args > 0))
                    continue;

                static bf_interpreter_perfomance_info_t element_to_add;
                element_to_add.count = 0;
                element_to_add.PC = index;
                dynarray_push_back(&engine->performance_info, &element_to_add);
            }
        }
    }
    engine->memory = calloc(0x10000, sizeof(uint8_t));
}

void bf_interpreter_step(bf_interpreter_t* engine)
{
    bf_instruction_t* instruction;
    instruction = dynarray_get(engine->config.program, engine->PC);

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
            if(engine->config.performance_info.enabled)
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
        if(engine->config.input != NULL)
            fscanf(engine->config.input, "%c", &engine->memory[engine->I]);
        else
            engine->memory[engine->I] = 0;
        engine->PC++;
        break;
    case OUT:
        if(engine->config.output != NULL)
            fprintf(engine->config.output, "%c", engine->memory[engine->I]);
        engine->PC++;
        break;
    case CLR:
        engine->memory[engine->I] = 0;
        engine->PC++;
        break;
    case ADDCLR:
        engine->memory[engine->I + instruction->args] += engine->memory[engine->I];
        engine->memory[engine->I] = 0;
        engine->PC++;
        break;
    }
}

void bf_interpreter_run(bf_interpreter_t* engine)
{
    while(engine->PC < engine->config.program.size)
        bf_interpreter_step(engine);

    if(engine->config.performance_info.enabled)
        bf_interpreter_print_performance_info(*engine, engine->config.performance_info.log_file);
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
        for(uint32_t program_index = current_element->PC; program_index < engine.config.program.size; program_index++)
        {
            current_instruction = dynarray_get(engine.config.program, program_index);
            if(current_instruction->args)
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
            case CLR:
                fprintf(output, "clr ");
                break;       
            case ADDCLR:
                fprintf(output, "adcl ");
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
}
