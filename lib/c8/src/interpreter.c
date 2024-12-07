#include "interpreter.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void c8_interpreter_init(c8_interpreter_t* interpreter, c8_interpreter_config_t config)
{
    interpreter->PC = 0x0200;
    interpreter->I = 0x0000;
    memset(interpreter->V, 0, 0x10 * sizeof(uint8_t));
    interpreter->memory = calloc(0x10000, sizeof(uint8_t));
    memcpy(interpreter->memory + interpreter->PC, config.program, config.program_size);
}

void c8_interpreter_run(c8_interpreter_t* interpreter)
{
    #define ONOO ((opcode & 0xF00) >> 8)
    #define OONN (opcode & 0xFF)
    #define ONNN (opcode & 0xFFF)
    #define VI   interpreter->I
    #define VX   interpreter->V[ONOO]
    #define ERROR_UNKNOWN_OPCODE() printf("\nError: Unknown opcode found at PC 0x%04X: %04X.\n\n", interpreter->PC, opcode), ended = true;

    bool ended = false;
    while(!ended)
    {
        const uint16_t opcode = interpreter->memory[interpreter->PC] << 8 | interpreter->memory[interpreter->PC + 1];
        switch((opcode & 0xF000) >> 12)
        {
        case 0x0:
            switch(opcode & 0xFF)
            {
            // CLS.
            case 0xE0:
                printf("WARNING: Unimplemented opcode 0x00E0: CLS.\n");
                break;
            default:
                ERROR_UNKNOWN_OPCODE();
                break;
            }
            break;

        // LD Vx, NN
        case 0x6:
            VX = OONN;
            break;

        // LD I, NNN
        case 0xA:
            VI = ONNN;
            break;

        default:
            ERROR_UNKNOWN_OPCODE();
            break;
        }
        interpreter->PC += 2;
    }

    #undef ERROR_UNKNOWN_OPCODE
    #undef ONNN
    #undef OONN
    #undef ONOO
    #undef VI
    #undef V
}

void c8_interpreter_free(c8_interpreter_t interpreter)
{
    free(interpreter.memory);
}

