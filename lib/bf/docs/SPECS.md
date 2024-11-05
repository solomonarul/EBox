# Specification:

### Machine specification:

The machine consists of:
- a fixed size memory of 65535 cells.
- a 16-bit instruction pointer => max program size of 65535 characters.
- a 16-bit memory pointer.

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

In our the internal optimized form, they are remodelled as follows:

- 'add' value - Adds a value (can be negative) to the current cell.
- 'mov' steps - Moves the pointer some steps in a direction, with negative going backwards.
- 'jmp' steps - Jumps the instruction pointer some steps in a direction.
    - if the value is positive, then the jump will be executed if the value is 0.
    - if the value is negative, then the jump will be executed if the value is not 0.
- 'out' - Prints the value at the current cell.
- 'in' - Reads value to place at the current cell.

These form patterns that latter are optimized into single instructions for faster speeds.

### External:

- [Link to a more complete specification of the language.](https://github.com/sunjay/brainfuck/blob/master/brainfuck.md)