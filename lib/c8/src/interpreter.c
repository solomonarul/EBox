#include "interpreter.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void c8_interpreter_init(c8_interpreter_t* interpreter, c8_interpreter_config_t config)
{
    interpreter->PC = 0x0200;
    interpreter->I = 0x0000;
    interpreter->running = true;
    memset(interpreter->V, 0, 0x10 * sizeof(uint8_t));
    interpreter->memory = calloc(0x10000, sizeof(uint8_t));
    memcpy(interpreter->memory + interpreter->PC, config.program, config.program_size);
    interpreter->draw_function = config.draw_function;
}

void c8_interpreter_run(c8_interpreter_t* interpreter)
{
    #define NOOO ((opcode & 0xF000) >> 12)
    #define ONOO ((opcode & 0xF00) >> 8)
    #define OONO ((opcode & 0xF0) >> 4)
    #define OOON (opcode & 0xF)
    #define OONN (opcode & 0xFF)
    #define ONNN (opcode & 0xFFF)
    #define PC   interpreter->PC
    #define VI   interpreter->I
    #define VX   interpreter->V[ONOO]
    #define VY   interpreter->V[OONO]
    #define VF   interpreter->V[0xF]
    #define ERROR_UNKNOWN_OPCODE() printf("\nError: Unknown opcode found at PC 0x%04X: %04X.\n\n", PC, opcode), interpreter->running = true;

    while(interpreter->running)
    {
        const uint16_t opcode = interpreter->memory[PC] << 8 | interpreter->memory[PC + 1];
        switch(NOOO)
        {
        case 0x0:
            switch(OONN)
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

        // JMP NNN
        case 0x1:
            if(PC == ONNN)
            {
                interpreter->running = false;
                printf("WARNING: Infinite loop found at PC %04X. Shutting down...\n\n", ONNN);
                return;
            }
            PC = ONNN - 2;
            break;

        // LD Vx, NN
        case 0x6:
            VX = OONN;
            break;

        // LD I, NNN
        case 0xA:
            VI = ONNN;
            break;

        // DRW, X, Y, n
        case 0xD: 
            VF = interpreter->draw_function != NULL ? interpreter->draw_function(VX, VY, OOON) : 0x00;
            break;

        default:
            ERROR_UNKNOWN_OPCODE();
            break;
        }
        PC += 2;
    }

    #undef ERROR_UNKNOWN_OPCODE
    #undef NOOO
    #undef ONNN
    #undef OONN
    #undef OOON
    #undef ONOO
    #undef OONO
    #undef VI
    #undef VX
    #undef VY
    #undef VF
    #undef V
}

void c8_interpreter_free(c8_interpreter_t interpreter)
{
    free(interpreter.memory);
}

