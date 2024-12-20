#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

typedef void(*c8_log_function_t)(const char* format, ...);
typedef void(*c8_clear_function_t)(void);
typedef uint8_t(*c8_draw_function_t)(uint8_t x, uint8_t y, uint8_t* sprite, uint8_t n);

typedef struct {
    uint32_t program_size;
    uint8_t* program;
    c8_draw_function_t draw_function;
    c8_clear_function_t clear_function;
    c8_log_function_t log_function;
} c8_interpreter_config_t;

typedef struct {
    uint16_t PC, I;
    uint8_t SP;
    uint16_t stack[0x100];
    uint8_t V[0x10];
    uint8_t* memory;
    bool running;
    c8_draw_function_t draw_function;
    c8_clear_function_t clear_function;
    c8_log_function_t log_function;
} c8_interpreter_t;

void c8_interpreter_init(c8_interpreter_t* interpreter, c8_interpreter_config_t config);
void c8_interpreter_run(c8_interpreter_t* interpreter);
void c8_interpreter_free(c8_interpreter_t* interpreter);

