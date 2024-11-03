# BF - BrainFuck module

### Description:

This is the BrainFuck module of the emulator.

It parses the input into a simplified, optimized, internal form which an internal backend will execute.

The backends present are:
- (TODO): interpreter - simply execute the instructions as is.
- (TODO): AOT - compiles the result into native code.

### Machine specification:

The machine consists of:
- a fixed size memory of 65535 cells (TODO: make dynamic maybe?).
- an instruction pointer.
- a memory pointer.

### Language specification:

BrainFuck is a simple 'programming language', consisting of the following opcodes:

- '>' - move the pointer right
- '<' - move the pointer left
- '+' - increment the current cell
- '-' - decrement the current cell
- '.' - output the value of the current cell
- ',' - replace the value of the current cell with input
- '[' - jump to the matching ']' instruction if the current value is zero
- ']' - jump to the matching '[' instruction if the current value is not zero

Everything else is treated as a comment, so ignored.

In the internal optimized form, they are remodelled as follows:

- 'add' value - Adds a value (can be negative) to the current cell.
- 'mov' steps - Moves the pointer some steps in a direction, with negative going backwards.
- 'jmp' steps - Jumps the instruction pointer some steps in a direction.
    - if the value is positive, then the jump will be executed if the value is 0.
    - if the value is negative, then the jump will be executed if the value is not 0.
- 'out' - Prints the value at the current cell.
- 'in' - Reads value to place at the current cell.

#### [Link to a more complete specification of the language.](https://github.com/sunjay/brainfuck/blob/master/brainfuck.md)