# bfd - Brainfuck interpreter and debugger in C
# by Terry Hearst

all: bfd.exe

bfd.exe: bfd.c
	gcc -Wall -Wextra -pedantic -o bfd bfd.c

clean:
	rm -f *.exe