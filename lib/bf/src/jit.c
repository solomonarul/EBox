#include "bf/jit.h"
#include "parser.h"
#include "util/meta.h"
#include <stdint.h>
#include <stdlib.h>
#include <lightning.h>

static jit_state_t* bf_jit;
#define _jit bf_jit

void bf_jit_init(bf_jit_t* engine, bf_jit_config_t config)
{
    engine->memory = calloc(0x10000, sizeof(char));

    init_jit(meta_get_exe_path());
    bf_jit = jit_new_state();

    jit_prolog();
    jit_movi(JIT_V0, 0);         // V0 is the index.
    // R0 is the memory pointer.
    // R1 is working register.

    for(uint32_t index = 0; index < config.program.size; index++)
    {
        const bf_instruction_t* instruction = dynarray_get(config.program, index);
        switch(instruction->type)
        {
        case MOV:
            if(instruction->args > 0)
                jit_addi(JIT_V0, JIT_V0, instruction->args);
            else
                jit_subi(JIT_V0, JIT_V0, -instruction->args);
            break;
        case ADD:
            jit_movi(JIT_R0, (jit_word_t)engine->memory);
            jit_ldxr_c(JIT_R1, JIT_V0, JIT_R0);
            if(instruction->args > 0)
                jit_addi(JIT_R1, JIT_R1, instruction->args);
            else
                jit_subi(JIT_R1, JIT_R1, -instruction->args);
            jit_stxr_c(JIT_V0, JIT_R0, JIT_R1);
            break;
        case JMP:
            if (instruction->args > 0) {
                jit_movi(JIT_R0, (jit_word_t)engine->memory);
                jit_ldxr_c(JIT_R1, JIT_V0, JIT_R0);

                // Branch if R1 is zero, this will be emitted in the later branch.
            } else {
                jit_movi(JIT_R0, (jit_word_t)engine->memory);
                jit_ldxr_c(JIT_R1, JIT_V0, JIT_R0);

                // Branch if R1 is not zero.
            }
            break;
        case IN:
            jit_prepare();
            jit_pushargi((jit_word_t)config.input);
            jit_finishi((jit_pointer_t)(uintptr_t)fgetc);
            jit_retval(JIT_R1);
            jit_movi(JIT_R0, (jit_word_t)engine->memory);
            jit_stxr_c(JIT_V0, JIT_R0, JIT_R1);
            break;
        case OUT:
            jit_movi(JIT_R0, (jit_word_t)engine->memory);
            jit_ldxr_c(JIT_R1, JIT_V0, JIT_R0);
            jit_prepare();
            jit_pushargr_c(JIT_R1);
            jit_pushargi((jit_word_t)config.output);
            jit_finishi((jit_pointer_t)(uintptr_t)fputc);
            break;
        }
    }
    jit_epilog();
    engine->code = (bf_jit_function_t)((uintptr_t)jit_emit());
}

void bf_jit_run(bf_jit_t* engine)
{
    engine->code();
}

void bf_jit_free(bf_jit_t engine)
{
    free(engine.memory);
    jit_destroy_state();
    finish_jit();
}

#undef _jit
