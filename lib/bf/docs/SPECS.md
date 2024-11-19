# Specification:

### Machine specification:

The machine consists of:
- a fixed size memory of 65535 8-bit cells.
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

### External:

- [Link to a more complete specification of the language.](https://github.com/sunjay/brainfuck/blob/master/brainfuck.md)