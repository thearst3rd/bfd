// bfd - Brainfuck Debugger
// Brainfuck interpreter and debugger in C
// Inspired by gdb, GNU debugger
// by Terry Hearst

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "0.0.2"

/* ----- GLOBAL VARIABLE DECLARATIONS ----- */

#define MEM_SIZE 30000
#define MAX_LOOPS 256
#define OUTPUT_BUF_SIZE 5000

FILE *f; 	// File pointer

char *insts; 	// Buffer that holds the program instructions
int ip; 		// Instruction pointer
long numInsts; 	// Number of instructions

char *mem; 		// Memory buffer
int ind; 		// Current memory cell index
int *stack; 	// Holds memory addresses for loops
int sCount; 	// Number of items on stack


/* ----- FUNCTION FORWARD-DECLARATIONS ----- */

void handleCmdLineOptions(int, char *[]);
void printUsage();

int isValidInstruction(char);
void handleInstruction(char);
int stackPush(int);
int stackPop();
int stackPeek();

int run();


/* ----- MAIN ----- */

int main(int argc, char *argv[])
{
	// Parse command line arguments
	handleCmdLineOptions(argc, argv);
	
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
	mem = (char *) malloc(MEM_SIZE * sizeof(char));
	stack = (int *) malloc(MAX_LOOPS * sizeof(int));
	
	// Allocate memory for instructions
	long fileSize;
	fseek(f, 0, SEEK_END); 	// Note: this isn't truly portable. I don't care.
	fileSize = ftell(f);
	rewind(f);
	
	// Dump instructions into buffer
	insts = (char *) malloc(fileSize * sizeof(char));
	numInsts = 0;
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
		insts = (char *) realloc(insts, numInsts);
	}
	fclose(f);
	
	// ip == -1 means program isn't running
	ip = -1;
	
	run();
	
	// TODO: check if it actually didn't
	printf("\nProgram exited successfully!\n");
	
	// Perform some cleanup and exit!
	free(mem);
	free(stack);
	free(insts);
	
	return 0;
}


/* ----- FUNCTION IMPLEMENTATIONS ----- */

// Handles options which start with '-'. Checks if any are called and exit the
// program early if they are.
void handleCmdLineOptions(int argc, char *argv[])
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
			ip++;
			break;
		case '-':
			mem[ind]--;
			ip++;
			break;
		case '>':
			ind++;
			if (ind > MEM_SIZE - 1)
			{
				// TODO: Error
			}
			ip++;
			break;
		case '<':
			ind--;
			ip++;
			// TODO: Check for error
			break;
		case '[':
			if (mem[ind] == 0)
			{
				// Skip to matching ']'
				int level = 1;
				while (level != 0)
				{
					ip++;
					if (ip >= numInsts)
					{
						// We're done with program
						ip--; // (cuz i'm about to ++ again)
						break;
					}
					if (insts[ip] == '[')
						level++;
					else if (insts[ip] == ']')
						level--;
				}
				ip++;
			}
			else
			{
				// Push value to the stack
				if (!stackPush(ip + 1))
				{
					// TODO: Error
				}
				ip++;
			}
			break;
		case ']':
			if (mem[ind] == 0)
			{
				// TODO: Check for error
				stackPop();
				ip++;
			}
			else
			{
				// Pop IP on stack, go to it
				ip = stackPeek();
			}
			break;
		case '.':
			putchar(mem[ind]);
			ip++;
			break;
		case ',':
			mem[ind] = getchar();
			ip++;
			break;
	}
}

// Pushes item onto stack. Returns if successful
int stackPush(int val)
{
	if (sCount == MAX_LOOPS)
	{
		// STACK IS FULL!!! OPERATION FAILED
		return 0;
	}
	
	// Not full, we gucci
	stack[sCount] = val;
	sCount++;
	
	return 1;
}

// Pops item off of stack and returns it. Returns -1 if unsuccessful
int stackPop()
{
	if (sCount == 0)
	{
		return -1;
	}
	
	sCount--;
	return stack[sCount];
}

// Returns top item on stack (without popping). Returns -1 if unsuccessful
int stackPeek()
{
	if (sCount == 0)
	{
		return -1;
	}
	
	return stack[sCount - 1];
}

// Runs the program. Returns if successful
int run()
{
	// Some setup
	ip = 0;
	ind = 0;
	for (int i = 0; i < MEM_SIZE; i++)
	{
		mem[i] = 0;
	}
	
	// Actually interpret each instruction
	while ((ip != -1) && (ip < numInsts))
	{
		handleInstruction(insts[ip]);
	}
	return 1;
}