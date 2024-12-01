#pragma once

#include <stdint.h>

typedef struct {
    uint16_t PC, I;
    uint8_t* memory;
} c8_interpreter_t;

void c8_interpreter_init(c8_interpreter_t* interpreter);
void c8_interpreter_free(c8_interpreter_t interpreter);
