#include "interpreter.h"

#include <stdlib.h>

void c8_interpreter_init(c8_interpreter_t* interpreter)
{
    interpreter->PC = 0x0200;
    interpreter->I = 0x0000;
    interpreter->memory = calloc(0x10000, sizeof(uint8_t));
}

void c8_interpreter_free(c8_interpreter_t interpreter)
{
    free(interpreter.memory);
}