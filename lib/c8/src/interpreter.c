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
    interpreter->SP = 0x00;
    memset(interpreter->V, 0x00, 0x10 * sizeof(uint8_t));
    interpreter->memory = calloc(0x10000, sizeof(uint8_t));
    memcpy(interpreter->memory + interpreter->PC, config.program, config.program_size);
    interpreter->draw_function = config.draw_function;
    interpreter->clear_function = config.clear_function;
    interpreter->log_function = config.log_function;
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
    #define ERROR_UNKNOWN_OPCODE() interpreter->log_function("[EROR]: Unknown opcode found at PC 0x%04X: %04X.\n", PC, opcode), interpreter->running = false;

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
                interpreter->clear_function();
                PC += 2;
                break;
            
            // RET.
            case 0xEE:
                PC = interpreter->stack[--interpreter->SP];
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
                interpreter->log_function("[WARN]: Infinite loop found at PC %04X. Shutting down...\n\n", ONNN);
                return;
            }
            PC = ONNN;
            break;

        // CALL NNN
        case 0x2:
            interpreter->stack[interpreter->SP++] = PC + 2;
            PC = ONNN;
            break;
        
        // SE Vx, NN
        case 0x3:
            if(VX == OONN) PC += 2;
            PC += 2;
            break;

        // SNE VX, NN
        case 0x4:
            if(VX != OONN) PC += 2;
            PC += 2;
            break;
        
        case 0x5:
            switch(OOON)
            {
            // SE, Vx, Vy
            case 0x0:
                if(VX == VY) PC += 2;
                PC += 2;
                break;
            default:
                ERROR_UNKNOWN_OPCODE();
                break;
            }        
            break;

        // LD Vx, NN
        case 0x6:
            VX = OONN;
            PC += 2;
            break;

        // ADD Vx, NN
        case 0x7:
            VX += OONN;
            PC += 2;
            break;

        case 0x8:
            switch(OOON)
            {
            // LD Vx, Vy
            case 0x0:
                VX = VY;
                PC += 2;
                break;

            // OR Vx, Vy
            case 0x1:
                VX |= VY;
                PC += 2;
                break;

            // AND Vx, Vy
            case 0x2:
                VX &= VY;
                PC += 2;
                break;
        
            // XOR Vx, Vy
            case 0x3:
                VX ^= VY;
                PC += 2;
                break;

            // ADD Vx, Vy
            case 0x4:
                VX += VY;
                PC += 2;
                break;
            
            // SUB Vx, Vy
            case 0x5:
                VX -= VY;
                PC += 2;
                break;

            // SHR Vx, Vy
            case 0x6:
                VF = VY & 1;
                VX = VY >> 1;
                PC += 2;
                break;

            // SUBN Vx, Vy
            case 0x7:
                VF = VY > VX;
                VX = VY - VX;
                PC += 2;
                break;

            // SHL Vx, Vy
            case 0xE:
                VF = VY & (1 << 7);
                VX = VY << 1;
                PC += 2;
                break;

            default:
                ERROR_UNKNOWN_OPCODE();
                break;
            }        
            break;

        case 0x9:
            switch(OOON)
            {
            // SNE, Vx, Vy
            case 0x0:
                if(VX != VY) PC += 2;
                PC += 2;
                break;

            default:
                ERROR_UNKNOWN_OPCODE();
                break;
            }        
            break;

        // LD I, NNN
        case 0xA:
            VI = ONNN;
            PC += 2;
            break;

        // DRW, X, Y, n
        case 0xD: 
            VF = interpreter->draw_function != NULL ? interpreter->draw_function(VX, VY, interpreter->memory + VI, OOON) : 0x00;
            PC += 2;
            break;

        case 0xF:
            switch(OONN)
            {
            // ADD I, Vx
            case 0x1E:
                interpreter->I += VX;
                PC += 2;
                break;

            // BCD Vx
            case 0x33:
                interpreter->memory[interpreter->I + 2] = VX % 10;
                interpreter->memory[interpreter->I + 1] = (VX / 10) % 10;
                interpreter->memory[interpreter->I] = VX / 100;
                PC += 2;
                break;

            // LDR Vx
            case 0x55:
                for(uint32_t index = 0; index <= ONOO; index++)
                    interpreter->memory[interpreter->I++] = interpreter->V[index];
                PC += 2;
                break;

            // STR Vx
            case 0x65:
                for(uint32_t index = 0; index <= ONOO; index++)
                    interpreter->V[index] = interpreter->memory[interpreter->I++];
                PC += 2;
                break;

            default:
                ERROR_UNKNOWN_OPCODE();
                break;
            }   
            break;

        default:
            ERROR_UNKNOWN_OPCODE();
            break;
        }
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

void c8_interpreter_free(c8_interpreter_t* interpreter)
{
    free(interpreter->memory);
}

