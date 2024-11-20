#include "bf/jit.h"

#include <stdlib.h>

void bf_jit_init(bf_jit_t* engine, bf_jit_config_t config)
{
    engine->input = config.input;
    engine->output = config.output;

    // Allocate memory for the code and memory.
    engine->code = calloc(1024, 1);
    engine->code_size = 1024;
    engine->memory = calloc(0x10000, sizeof(uint16_t));
}

void bf_jit_step(bf_jit_t* engine)
{
    UNUSED(engine);
}

void bf_jit_run(bf_jit_t* engine)
{
    UNUSED(engine);
}

void bf_jit_free(bf_jit_t engine)
{
    free(engine.code);
    free(engine.memory);
}

