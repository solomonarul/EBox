# EBox - The Emulator Box

This is a tool that aims to be able to emulate multiple systems / languages.

## Current modules:

- [BrainFuck](./lib/bf/) - an esoteric programming language.

## How to run:

#### Build:

> make b\<initial of platform\>\<initial of debug or release modes\>

##### Example:

> make bur  # Build Unix Release

#### Run:

> make r

In the [Makefile](Makefile), this runs the emulator using a default startup.ini in the base folder.

Sample ini:

```ini
# This specifies which platform to emulate.
[Emulator]
core=bf

# These specify the data source and destination for the emulator.
[bf]
input=cli
output=cli
engine=jit
program=./samples/bf/mandlebrot.b

[c8]
program=./samples/c8/timendus/1-chip8-logo.ch8
```

#### Benchmark:

> make b

#### Test:

> make t

#### Anything else:

If you are willing, you can make your own starting commands in the [Makefile](Makefile).