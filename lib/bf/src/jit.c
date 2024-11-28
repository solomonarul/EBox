#include "bf/jit.h"
#include "parser.h"
#include "util/meta.h"
#include <stdint.h>
#include <stdlib.h>

static bool jit_global_init = false;

void bf_jit_init(bf_jit_t* engine, bf_jit_config_t config)
{
    if(!jit_global_init)
    {
        init_jit(meta_get_exe_path());
        atexit(finish_jit);
        jit_global_init = true;
    }

    engine->memory = calloc(0x10000, sizeof(char));
    engine->jit_state = jit_new_state();

    #define _jit engine->jit_state

    jit_prolog();
    jit_movi(JIT_V0, (jit_word_t)engine->memory);   // V0 is the memory pointer
    // R0 is working register.

    jit_pointer_t loopStart[512];
    jit_pointer_t loopEnd[512];
    jit_pointer_t jump;
    uint32_t loop = 0;

    for(uint32_t index = 0; index < config.program.size; index++)
    {
        const bf_instruction_t* instruction = dynarray_get(config.program, index);
        switch(instruction->type)
        {
        case MOV:
            jit_addi(JIT_V0, JIT_V0, instruction->args);
            break;
        case ADD:
            jit_ldr_c(JIT_R0, JIT_V0);
            jit_addi(JIT_R0, JIT_R0, instruction->args);
            jit_str_c(JIT_V0, JIT_R0);
            break;
        case JMP:
            jit_ldr_c(JIT_R0, JIT_V0);

            if(instruction->args > 0)
            {
                // Branch if R1 is zero, this will be emitted in the later branch.
                loopEnd[loop] = _jit_new_node_pww(engine->jit_state, jit_code_beqi, jit_forward(), JIT_R0, 0); // jit_beqi(jit_forward(), JIT_R0, 0);
                loopStart[loop] = jit_label();
                loop++;
            }
            else
            {
                loop--;
                // Branch if R1 is not zero.
                jump = _jit_new_node_pww(engine->jit_state, jit_code_bnei, NULL, JIT_R0, 0);   // jit_bnei(loopStart[loop], JIT_R0, 0);
                jit_patch(loopEnd[loop]);
                jit_patch_at(jump, loopStart[loop]);
            }
            break;
        case IN:
            jit_prepare();
            jit_pushargi((jit_word_t)config.input);
            jit_pushargi((jit_word_t)(jit_pointer_t)"%c");
            jit_pushargr(JIT_V0);
            jit_finishi((jit_pointer_t)(uintptr_t)fscanf);
            break;
        case OUT:
            jit_ldr_c(JIT_R0, JIT_V0);
            jit_prepare();
            jit_pushargr_c(JIT_R0);
            jit_pushargi((jit_word_t)config.output);
            jit_finishi((jit_pointer_t)(uintptr_t)fputc);
            break;
        }
    }
    jit_epilog();
    engine->code = (bf_jit_function_t)((uintptr_t)jit_emit());
    jit_clear_state();

    #undef _jit
}

void bf_jit_run(bf_jit_t* engine)
{
    engine->code();
}

void bf_jit_free(bf_jit_t engine)
{
    free(engine.memory);
}
