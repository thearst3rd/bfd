# bfd - Brainfuck Debugger

A gdb-inspired debugger for [Brainfuck](https://en.wikipedia.org/wiki/Brainfuck) programs, written in C.

### Why?

1. I really just want to test my C chops and see if I could do it.
2. I like Brainfuck and other esoteric languages.
3. Maybe somehow, somewhere, at some point it will be useful to someone. Maybe.

I'm mostly just making this program for myself. As such, I'm taking the lazy approach to some things. For example, in the makefile, the build targets are specified as .exe files, specific to Windows and MinGW. Since I'm using Windows, I've elected to simply use this.

# Using the Program

### Compiling

If you are running Windows with MinGW, you can run `make` in the command line and bfd will build itself with gcc. I'm using [MinGW-w64](https://sourceforge.net/projects/mingw-w64/).

Otherwise, simply compile bfd.c with your favorite C compiler. It should not have any Windows specific dependancies.

### Running

To run bfd, type `bfd FILE-NAME` into the command line. This will start the debugger running with the given brainfuck file.

For more information, type `bfd --help`.