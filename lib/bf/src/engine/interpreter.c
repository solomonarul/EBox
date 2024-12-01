#include "engine/interpreter.h"
#include "parser.h"

#include <stdlib.h>

void bf_interpreter_init(bf_interpreter_t* engine, bf_interpreter_config_t config)
{
    engine->config = config;
    engine->I = 0;
    engine->PC = 0;
    engine->memory = calloc(0x10000, sizeof(uint8_t));
}

void bf_interpreter_step(bf_interpreter_t* engine)
{
    const bf_instruction_t* instruction = dynarray_get(engine->config.program, engine->PC);

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
        if(instruction->args > 0)
            engine->PC += (engine->memory[engine->I] == 0) ? instruction->args : 1; // Jump ahead if value is 0.
        else
            engine->PC += (engine->memory[engine->I] != 0) ? instruction->args : 1; // Jump behind if value is less than 0.
        break;

    case IN:
        if(engine->config.input_function != NULL)
            engine->memory[engine->I] = engine->config.input_function();
        else
            engine->memory[engine->I] = 0;
        engine->PC++;
        break;

    case OUT:
        if(engine->config.output_function != NULL)
            engine->config.output_function(engine->memory[engine->I]);
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

    case MOVNZ:
        while(engine->memory[engine->I] != 0)
            engine->I += instruction->args;
        engine->PC++;
        break;
    }
}

void bf_interpreter_run(bf_interpreter_t* engine)
{
    while(engine->PC < engine->config.program.size)
        bf_interpreter_step(engine);
}

void bf_interpreter_free(bf_interpreter_t engine)
{
    free(engine.memory);
}
