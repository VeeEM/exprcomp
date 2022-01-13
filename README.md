exprcomp is a toy compiler that takes mathematical expressions as input
and produces x86-64 elf executables as output. Input expressions can 
contain (, ), *, +, and natural numbers. When invoked with an input 
file as argument, exprcomp produces an executable with the name "program", 
an object file "program.o", and an assembly file "program.s". exprcomp 
also prints the abstract syntax tree for the target expression, in a
lisp-like representation to stdout every time its run.

## Requirements
For building exprcomp:
* gcc
* Make

I use gcc and GNU Make, but other C compilers and Makes can probably be 
used.


For running exprcomp:
* ld
* as
* 64-bit Linux system

After generating the assembly file, exprcomp invokes the assembler 
and linker with the following command:
> as -o program.o program.s && ld -o program program.o

Programs created by exprcomp use the write system call to produce 
output.

## Demo
```
$ make
$ echo "(1 + 2) * 40 + 3" > example
$ ./exprcomp example

(+
  (*
    (+
      1
      2
    )
    40
  )
  3
)
$ ./program 
123
```
