# How was this made?

A BF emulator is one of the easiest kind of emulators to implement out there.

In fact, the smallest implementation I could find is an impressive 172 bytes, right here:

```sh
tr -Cd '][.,<>+-'|sed 's/\./putchar(*p);/g;s/,/*p=getchar();/g;s/[+-]/&&*p;/g;s/[<>]/&&p;/g;s/\[/while(*p){/g;y/]<>/}-+/;s/^/main(){int a[30000];int *p=a;/;s/$/}/'|cc -xc -
```

(I got it from [here](https://dpk.land/io/bfc))

This apparent simplicity leaves room for a lot of complexity in how we approach this problem. This is what this module attempts to solve.

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

| Program Name  | Execution Time |
| :-----------: | :------------: |
| hanoi.b       | ~11.75s        |
| mandlebrot.b  | ~8.50s         |
| factor.b      | ~3.50s         |
| collatz.b     | ~0.15s         |
| squares.b     | ~3.00ms        |
| sierpinski.b  | ~0.50ms        |

| Program Name  | Instructions | Initial instructions |
| :-----------: | :----------: | :------------------: |
| hanoi.b       | 17799        | 53884                |
| mandlebrot.b  | 4115         | 11451                |
| factor.b      | 1268         | 3880                 |
| collatz.b     | 261          | 395                  |
| squares.b     | 127          | 196                  |
| sierpinski.b  | 85           | 124                  |

Keep in mind that benchmarks are done on an optimized build on an Intel i7-12700H using an executable equivalent to [this commit](https://github.com/solomonarul/EBox/commit/bfb5e36f64ed3122c3d8c540906b3d4ea7551859).

## Optimization #3: cleaning hot loops.

Consider this piece of BF code:

> [-]

What does it do? The effect of this sequence is equivalent to setting the current memory value to 0.

Now how long does it take to execute? The answer is the value at the current position, which can get up to 255 as each cell is 8 bit. This can be optimized right away in a single instruction instead.

But how do we know what loops to optimize? I added a performance counter which checks how often each top-level loop in the initial program, as those are most likely to be culprits, run.

This outputs a result following these rules:

> PC | number of executions | sequence

For mandlebrot.b this kind of result is shown:

> 1 | 1 | 12j -1a 1m 2a 3m 5a 1m 2a 1m 1a -6m -10j 
>
> 20 | 15 | 3j 9m -1j 
>
> 24 | 15 | 3j -9m -1j 
>
> 33 | 16 | 3j -1a -1j 
>
> 39 | 1 | 3j -9m -1j 
>
> 43 | 1 | 3j -1a -1j 
>
> 51 | 5 | 6j -1a 9m 1a -9m -4j 
>
> 64 | 1 | 3j -9m -1j 
>
> 68 | 1 | 3j -1a -1j 
>
> 76 | 768 | 3j -1a -1j 
>
> 82 | 48 | 3j -9m -1j 
>
> 86 | 48 | 3j -1a -1j 
>
> 94 | 192 | 6j -1a 9m 1a -9m -4j 
>
> 108 | 336 | 6j -1a 9m 1a -9m -4j 
>
> 119 | 48 | 3j -9m -1j 
>
> 124 | 6192 | 3j -1a -1j 
>
> 130 | 99072 | 6j -1a -6m 1a 6m -4j 
>
> 137 | 99072 | 10j -1a 6m 1a -2m 1a -3m 1a -1m -8j 
>
> 150 | 6192 | 3j -9m -1j
>
> and so on... 

This kind of result is not really that useful in practice so I wrote an AWK script that condenses this is a more useful format:

> number of executions | UNIQUE sequence

This groups different occurences of the same loop into a singular result and makes our output way less verbose and generates this result:

> 157090277  6j -1a 9m 1a -9m -4j
>
> 46993495  3j -1a -1j
>
> 25555337  6j -1a -2m 1a 2m -4j
>
> 15623995  3j 9m -1j
>
> 14504484  6j -1a 1m 1a -1m -4j
>
> 12637333  8j -1a 2m 1a 2m 1a -4m -6j
>
> 11813904  8j -1a 2m 1a 1m 1a -3m -6j 
>
> 9017333  9j -1m -1a 1m -1a -6m 1a 6m -7j 
>
> 8853089  3j -9m -1j 
>
> 8466691  6j -1a -5m 1a 5m -4j 
>
> 7740892  6j -1a -3m 1a 3m -4j 
>
> 7569507  6j -1a 3m 1a -3m -4j 
>
> 7493248  6j -1a -36m 1a 36m -4j 
>
> 7440200  6j -1a -4m 1a 4m -4j 
>
> 6497223  8j -1a 3m 1a 1m 1a -4m -6j
>
> and so on...

Much better. We can now see some repeating patterns, for example the pattern:

> 3j xa -1j

Which considering our unsigned cells is equivalent to setting them to 0. Note that these cover all of the following original sequences:

> [-] [+] [++++] [---]
> 
> and so on...

These sequences have the exact same result so we can replace them with a single, brand new, CLR instruction in the intermediate representation, thus saving us from executing \<cell size\> * 2 instructions.

### How to identify these sequences in code:

In the current way that code is parsed, we are generating code in a way very similar to a stack:
- get current instruction
- see what it is
- put it onto the stack if a different kind of instruction already exists.
- if it is the same kind, check to see if its argument can be modified.

This can be easily modified to support our new sequence parsing by checking after each addition for known patterns at the top of the stack. These can then be replaced with our custom instructions that do the exact equivalent result.

Replacing just the above mentioned loop gets us the following results in the benchmark:

| Program Name  | Execution Time after #3 | Execution Time before #3 |
| :-----------: | :---------------------: | :----------------------: |
| hanoi.b       | ~0.59s                  | ~11.75s                  |
| mandlebrot.b  | ~7.50s                  | ~8.50s                   |
| factor.b      | ~3.00s                  | ~3.50s                   |
| collatz.b     | ~0.10s                  | ~0.15s                   |
| squares.b     | ~0.00ms                 | ~3.00ms                  |
| sierpinski.b  | ~0.00ms                 | ~0.50ms                  |

| Program Name  | Optimization #3 | Optimizations #1 and #2 | Initial instructions |
| :-----------: | :-------------: | :---------------------: | :------------------: |
| hanoi.b       | 14853           | 17799                   | 53884                |
| mandlebrot.b  | 3867            | 4115                    | 11451                |
| factor.b      | 1208            | 1268                    | 3880                 |
| collatz.b     | 245             | 261                     | 395                  |
| squares.b     | 119             | 127                     | 196                  |
| sierpinski.b  | 81              | 85                      | 124                  |

Because of such low execution times, we will not consider squares.b and sierpinski.b going further.

## Optimization #4: even more hot loops.

Now we can add more loops to the mix. Let's take for example:

> 6j -1a 9m 1a -9m -4j

Which is executed a whole lot in mandlebrot.b. This sequence is equivalent to:

> memory[p + 9] += memory[p]
> memory[p] = 0

This can be made into a single instruction, let's call it ADDCLR. This instruction now makes use of our parameter based infrastructure, because it actually has one:

> ADDCLR 9

Now let's see the improvements:

| Program Name  | Execution Time after #4 | Execution Time after #3 | Execution Time after #1 and #2 |
| :-----------: | :---------------------: | :---------------------: | :----------------------------: |
| hanoi.b       | ~0.30s                  | ~0.59s                  | ~11.75s                        |
| mandlebrot.b  | ~6.50s                  | ~7.50s                  | ~8.50s                         |
| factor.b      | ~2.00s                  | ~3.00s                  | ~3.50s                         |
| collatz.b     | ~0.01s                  | ~0.10s                  | ~0.15s                         |

| Program Name  | Optimization #3 | Optimization #3 | Optimizations #1 and #2 | Initial instructions |
| :-----------: | :-------------: | :-------------: | :---------------------: | :------------------: |
| hanoi.b       | 9843            | 14853           | 17799                   | 53884                |
| mandlebrot.b  | 3162            | 3867            | 4115                    | 11451                |
| factor.b      | 1073            | 1208            | 1268                    | 3880                 |
| collatz.b     | 240             | 245             | 261                     | 395                  |

This can be continued ad infinitum until we get entire programs in a single instruction, which is not really that practical.

Let's introduce something even better instad:

## Final optimization: Just in Time compilation.