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

This uses the [mandlebrot.b](./samples/bf/mandlebrot.b) sample. Change it to anything you want in the [Makefile](Makefile).

#### Benchmark:

> make b

#### Test:

> make t

#### Anything else:

If you are willing, you can make your own starting commands using the [Makefile](Makefile).