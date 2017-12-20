// bfd - Brainfuck Debugger
// Brainfuck interpreter and debugger in C
// Inspired by gdb, GNU debugger
// by Terry Hearst

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "0.0.1"

/* ----- GLOBAL VARIABLE DECLARATIONS ----- */

#define MEM_SIZE 30000
#define MAX_LOOPS 128

FILE *f; 	// File pointer

char *mem; 	// Memory buffer
int ind; 	// Current memory cell index

char *insts; 	// Buffer that holds the program instructions
int ip; 		// Instruction pointer


/* ----- FUNCTION FORWARD-DECLARATIONS ----- */

void handleCmdOptions(int, char *[]);
void printUsage();

int isValidInstruction(char);
void handleInstruction(char);
void stackPush(int);
int stackPop();


/* ----- MAIN ----- */

int main(int argc, char *argv[])
{
	// Parse command line arguments
	handleCmdOptions(argc, argv);
	
	// Prints startup information filler
	printf("bfd " VERSION "\n"
	       "A Brainfuck Debugger\n"
	       "Created by Terry Hearst\n"
	       "No copyrights or anything rn, idk how to do that and I'm lazy\n"
	       "For details, visit (https://github.com/thearst3rd/bfd).\n\n");
	
	// Make sure user file exists
	f = fopen(argv[1], "r");
	if (f == NULL)
	{
		printf("ERROR: File '%s' cannot be opened.\n", argv[1]);
		exit(1);
	}
	
	// The file exists, so lets really get started!!!
	mem = (char *) calloc(MEM_SIZE, sizeof(char));
	
	// Allocate memory for instructions
	long int fileSize;
	fseek(f, 0, SEEK_END); 	// Note: this isn't truly portable. I don't care.
	fileSize = ftell(f);
	rewind(f);
	
	// Dump instructions into buffer
	insts = (char *) calloc(fileSize, sizeof(char));
	long int numInsts = 0;
	while (!feof(f))
	{
		int c = fgetc(f);
		if (c != EOF)
		{
			char cc = (char) c;
			if (isValidInstruction(c))
			{
				insts[numInsts] = cc;
				numInsts++;
			}
		}
	}
	// If there were comment characters, resize buffer to save space
	if (numInsts < fileSize)
	{
		realloc(insts, numInsts);
	}
	fclose(f);
	
	// The code interpreting will go here...
	
	// Perform some cleanup and exit!
	free(mem);
	free(insts);
	
	return 0;
}


/* ----- FUNCTION IMPLEMENTATIONS ----- */

// Handles options which start with '-'. Checks if any are called and exit the
// program early if they are.
void handleCmdOptions(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("ERROR: Please enter exactly one argument.\n");
		printUsage();
		exit(1);
	}
	
	if ((strcmp(argv[1], "--help") == 0) | (strcmp(argv[1], "-h") == 0))
	{
		printUsage();
		exit(0);
	}
	
	if ((strcmp(argv[1], "--version") == 0) | (strcmp(argv[1], "-v") == 0))
	{
		printf("bfc " VERSION "\n");
		exit(0);
	}
	
	if (argv[1][0] == '-')
	{
		printf("ERROR: unknown option '%s'\n", argv[1]);
		printUsage();
		exit(1);
	}
}

// Prints usage information.
void printUsage()
{
	printf("bfd - A brainfuck debugger. USAGE:\n\n"
	       "    bfd <file-name> | --help | --version\n\n"
		   "Main usage:\n\n"
	       "  <file-name>        Runs the debugger configured with given "
	           "brainfuck file.\n\n"
		   "Options:\n\n"
	       "  --help (or -h)     Prints the current help screen and exits.\n"
	       "  --version (or -v)  Prints the version information and exits.\n");
}

// Returns 1 if given char is a valid bf instruction, otherwise 0
int isValidInstruction(char c)
{
	switch (c)
	{
		case '+': case '-': case '>': case '<':
		case '[': case ']': case '.': case ',':
			return 1;
		default:
			return 0;
	}
}

// Handles the given instruction. Assumes input instruction is valid, does
// nothing (with no indication) if instruction is not valid
void handleInstruction(char c)
{
	switch (c)
	{
		case '+':
			mem[ind]++;
			break;
		case '-':
			mem[ind]--;
			break;
		case '>':
			ind++;
			break;
		case '<':
			ind--;
			break;
		case '[':
			if (mem[ind] == 0)
			{
				// Skip to matching ']'
			}
			else
			{
				// Push IP to stack
			}
			break;
		case ']':
			if (mem[ind] == 0)
			{
				// Discard IP on stack
			}
			else
			{
				// Pop IP on stack, go to it
			}
			break;
		case '.':
			putchar(mem[ind]);
			break;
		case ',':
			mem[ind] = getchar();
			break;
	}
}

void stackPush(int val)
{
	val++; 	// This is only to make the compiler not complain about unused val
}

int stackPop()
{
	return 0;
}