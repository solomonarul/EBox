#include "bf/jit.h"
#include "parser.h"
#include "util/dynarray.h"
#include <stdint.h>
#include <sys/mman.h>

#include <stdlib.h>
#include <string.h>

void bf_jit_init(bf_jit_t* engine, bf_jit_config_t config)
{
    engine->input = config.input;
    engine->output = config.output;

    // Allocate memory for the code and memory.
    dynarray_init(&engine->code, 1, 0, NULL);
    engine->memory = calloc(0x10000, sizeof(uint8_t));

    // Start generating code.
    // Everything will be sent as a parameter for ease of use with https://godbolt.org/ to generate code.
    unsigned char start_code[] = {
        0x55,  // push rbp
        0x48, 0x89, 0xe5,  // mov rbp, rsp
        0x48, 0x83, 0xec, 0x30,  // sub rsp, 0x30
        0x48, 0x89, 0x7d, 0xe8,  // mov QWORD PTR [rbp-0x18], rdi
        0x48, 0x89, 0x75, 0xe0,  // mov QWORD PTR [rbp-0x20], rsi
        0x48, 0x89, 0x55, 0xd8,  // mov QWORD PTR [rbp-0x28], rdx
        0x66, 0XC7, 0x45, 0xFE, 0x00, 0x00 // mov WORD PTR [rbp-0x2], 0x0000  - set up index.
        // This is equivalent to setting up void func(uint8_t* memory, FILE* input, FILE* output).
        // And creating a variable on the stack with the value 0x0000.
    };

    for (size_t index = 0; index < sizeof(start_code); index++)
        dynarray_push_back(&engine->code, &start_code[index]);

    // Generate the code for the program.
    for (size_t index = 0; index < config.program.size; index++)
    {
        const bf_instruction_t instruction = *(bf_instruction_t*)dynarray_get(config.program, index);
        switch(instruction.type)
        {
        case ADD:
            {
                // memory[index] += args
                unsigned char code[] = {
                    0x0F, 0xB7, 0x55, 0xFE,              // movzx edx,WORD PTR [rbp-0x2]
                    0x48, 0x8B, 0x45, 0xE8,              // mov rax,QWORD PTR [rbp-0x18]
                    0x48, 0x01, 0xD0,                       // add rax,rdx
                    0x0F, 0xB6, 0x10,                     // movzx edx,BYTE PTR [rax]
                    0x83, 0xC2, instruction.args & 0xFF,  // add edx, args
                    0x88, 0x10                                 // mov BYTE PTR [rax],dl
                };
                
                for (size_t index = 0; index < sizeof(code); index++)
                    dynarray_push_back(&engine->code, &code[index]);
            }
            break;
        case MOV:
            {
                // index += args
                unsigned char code[] = {
                    0x66, 0x83, 0x45, 0xfe, instruction.args // add WORD PTR [rbp-0x2], args
                };

                for (size_t index = 0; index < sizeof(code); index++)
                    dynarray_push_back(&engine->code, &code[index]);
            }
            break;
        case IN:
            {
                unsigned char code[] = {
                    0x48, 0x8B, 0x45, 0xE0,            // mov rax, QWORD PTR [rbp-0x20] (Load FILE* pointer)
                    0x48, 0x89, 0xC7,                       // mov rdi, rax
                    0x48, 0xB9,                                // movabs rcx, fgetc (address to be filled dynamically)
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Placeholder for the address
                    0xFF, 0xD1,                                 // call rcx
                    0x48, 0x89, 0xC1,                         // mov rcx, rax
                    0x0F, 0xB7, 0x55, 0xFE,              // movzx edx,WORD PTR [rbp-0x2]
                    0x48, 0x8B, 0x45, 0xE8,              // mov rax,QWORD PTR [rbp-0x18]
                    0x48, 0x01, 0xD0,                       // add rax,rdx
                    0x88, 0x08                                 // mov BYTE PTR [rax],cl
                };

                // Resolve the address of fgetc
                uintptr_t fgetc_addr = (uintptr_t)&fgetc;

                // Insert the address of fgetc into the code
                memcpy(&code[9], &fgetc_addr, sizeof(uintptr_t));

                for (size_t index = 0; index < sizeof(code); index++)
                    dynarray_push_back(&engine->code, &code[index]);

            }
            break;

        case OUT:
            {
                unsigned char code[] = {
                    0x0F, 0xB7, 0x55, 0xFE,              // movzx edx,WORD PTR [rbp-0x2]
                    0x48, 0x8B, 0x45, 0xE8,              // mov rax,QWORD PTR [rbp-0x18]
                    0x48, 0x01, 0xD0,                       // add rax,rdx
                    0x0F, 0xB6, 0x38,                     // movzx edi,BYTE PTR [rax]
                    0x48, 0x8B, 0x55, 0xD8,          // mov rdx,QWORD PTR [rbp-0x28]
                    0x48, 0x89, 0xD6,                     // mov rsi,rdx
                    0x48, 0xB9,                                // movabs rcx, fputc (address to be filled dynamically)
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Placeholder for the address
                    0xFF, 0xD1                                 // call rcx
                };

                // Resolve the address of fputc
                uintptr_t fputc_addr = (uintptr_t)&fputc;

                // Insert the address of fputc into the code
                memcpy(&code[23], &fputc_addr, sizeof(uintptr_t));

                for (size_t index = 0; index < sizeof(code); index++)
                    dynarray_push_back(&engine->code, &code[index]);
            }
            break;
        default:
            break;
        }
    }

    unsigned char end_code[] = {
        0xc9,   // leave
        0xc3    // ret
    };

    for (size_t index = 0; index < sizeof(end_code); index++)
        dynarray_push_back(&engine->code, &end_code[index]);
}

typedef void(*bf_jit_code_t)(uint8_t*, FILE*, FILE*);

void bf_jit_run(bf_jit_t* engine)
{
    // Get the generated code
    void* code_ptr = dynarray_get(engine->code, 0);
    size_t code_size = engine->code.size;

    // Allocate executable memory using mmap
    void* exec_memory = mmap(NULL, code_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (exec_memory == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Copy the generated code to the executable memory
    memcpy(exec_memory, code_ptr, code_size);

    // Change memory protection to executable
    if (mprotect(exec_memory, code_size, PROT_EXEC) == -1) {
        perror("mprotect");
        munmap(exec_memory, code_size);
        exit(EXIT_FAILURE);
    }

    // Cast the executable memory to a function pointer and execute it
    bf_jit_code_t code;
    *((void**)&code) = exec_memory;
    code(engine->memory, engine->input, engine->output);

    // Free the executable memory
    munmap(exec_memory, code_size);
}


void bf_jit_free(bf_jit_t engine)
{
    dynarray_free(engine.code);
    free(engine.memory);
}

