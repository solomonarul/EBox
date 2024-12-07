#pragma once

#include <stdint.h>

typedef struct {
    uint32_t program_size;
    uint8_t* program;
} c8_interpreter_config_t;

typedef struct {
    uint16_t PC, I;
    uint8_t V[0x10];
    uint8_t* memory;
} c8_interpreter_t;

void c8_interpreter_init(c8_interpreter_t* interpreter, c8_interpreter_config_t config);
void c8_interpreter_run(c8_interpreter_t* interpreter);
void c8_interpreter_free(c8_interpreter_t interpreter);

