#include "interpreter.h"
#include "parser.h"
#include "util/dynarray.h"

#include <stdlib.h>

void bf_interpreter_init(bf_interpreter_t* engine, FILE* input, FILE* output, dynarray_t program)
{
    engine->input = input;
    engine->output = output;
    engine->program = program;
    engine->I = 0;
    engine->PC = 0;
    engine->memory = calloc(sizeof(uint8_t), 0x10000);
}

void bf_interpreter_step(bf_interpreter_t* engine)
{
    static bf_instruction_t* instruction;
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
            engine->PC += (engine->memory[engine->I] == 0) ? instruction->args : 1;
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