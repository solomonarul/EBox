# C8 - CHIP8 module

## Description:

This is the CHIP8 module of the emulator.

It parses the input into a simplified, optimized, internal form which a backend will execute.

The backends present are:
- interpreter - simply execute the instructions as is.
- JIT compiler - compiles the result into native code before execution.
    - (optional:) static executables?

### [Machine specification here.](./docs/SPECS.md)
### [How was this made](./docs/LORE.md)