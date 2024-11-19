# How was this made?

A BF emulator is one of the easiest kind of emulators to implement out there.

In fact, the smallest implementation I could find is an impressive 172 bytes, right here:

```sh
tr -Cd '][.,<>+-'|sed 's/\./putchar(*p);/g;s/,/*p=getchar();/g;s/[+-]/&&*p;/g;s/[<>]/&&p;/g;s/\[/while(*p){/g;y/]<>/}-+/;s/^/main(){int a[30000];int *p=a;/;s/$/}/'|cc -xc -
```

(I got it from [here](https://dpk.land/io/bfc))

This apparent simplicity leaves room for a lot of complexity in how we approach this problem. This is what this module solves.

## Optimization #1: the obvious.

When looking at the original code of any BF program we find from a first glance that there are long sequences of repeating symbols, such as:

> +++++++++

> \>\>\>\>\>\>\>

and so on. This is where our first optimization can come in: reducing all of those into a single instruction.

Thus in the intermediary instruction set I made for this exact purpose I have replaced the original +-\<\> instructions with the following:
- ADD x - adds a signed value to the current cell.
- MOV x - adds to the pointer a signed value.

## Optimization #2: a bit less obvious.

Something that may not look that obvious in our initial instruction handling is how we are going to implement the \[\] instructions.

What comes to mind first when handling this would be going trough all of the previous / next instructions to find the next bracket if the condition required for a jump is found to be true.

But this is incredibly slow, especially if doing this for every single jump.

The solution is to just precalculate where the jumps will land at parsing time instead, thus replacing those instructions with:
- JMP x - jumps a signed value of x cells in the code.
    - if x is negative, the jump corresponds to a \] and will be executed only if the value at the pointer is not 0.
    - if x is positive, the jump corresponds to a \[ and will be executed only if the value at the pointer is 0.

## How do optimizations #1 and #2 fare?

After applying those optimizations to the code we can see the results here:

| Program Name  | Execution Time | Instructions | Initial instructions |
| :-----------: | :------------: | :----------: | :------------------: |
| hanoi.b       | ~11.75s        | 17799        | 53884                |
| mandlebrot.b  | ~8.50s         | 4115         | 11451                |
| factor.b      | ~3.50s         | 1268         | 3880                 |
| collatz.b     | ~0.15s         | 261          | 395                  |
| squares.b     | ~3.00ms        | 127          | 196                  |
| sierpinski.b  | ~0.50ms        | 85           | 124                  |

Keep in mind that benchmarks are done on an optimized build on an Intel i7-12700H.