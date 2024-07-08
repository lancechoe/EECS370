/*
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 *
 * Make sure to NOT modify printState or any of the associated functions
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//DO NOT CHANGE THE FOLLOWING DEFINITIONS 

// Machine Definitions
#define NUMMEMORY 65536 /* maximum number of words in memory (maximum number of lines in a given file)*/
#define NUMREGS 8 /*total number of machine registers [0,7]*/

// File Definitions
#define MAXLINELENGTH 1000 /* MAXLINELENGTH is the max number of characters we read */

typedef struct 
stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);

static inline int convertNum(int32_t);

int 
main(int argc, char **argv)
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s , please ensure you are providing the correct path", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; ++state.numMemory) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address  %d\n. Please ensure you are providing a machine code file.", state.numMemory);
            perror("sscanf");
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    //Your code starts here!

    for(int i = 0; i < NUMREGS; i++){
        state.reg[i] = 0;
    }
    
    state.pc = 0;

    int pcCounter = 0;

    while(state.mem[state.pc] != 25165824){ // not halt

        printState(&state);

        int mcCode = state.mem[state.pc]; // memory[num] = machine code in decimal
        int opcode = mcCode >> 22;
        int regA = (mcCode >> 19) & (0b111);
        int regB = (mcCode >> 16) & (0b111);
        int destReg = mcCode & (0b111); 
        int offset = convertNum(state.mem[state.pc] & (0x0000FFFF));
    
        if(opcode == 0){ // add
            state.reg[destReg] = state.reg[regA] + state.reg[regB];
        }
        else if(opcode == 1){ // nor
            state.reg[destReg] = ~(state.reg[regA] | state.reg[regB]);
        }
        else if(opcode == 2){ // lw
            state.reg[regB] = state.mem[state.reg[regA] + offset]; 
            // load regB from memory (memory address is formed by adding offset + regA)
        }
        else if(opcode == 3){ // sw
            state.mem[state.reg[regA] + offset] = state.reg[regB];
            // store regB into memory
        }
        else if(opcode == 4){ // beq
            if(state.reg[regA] == state.reg[regB]){
                state.pc = state.pc + offset;
            }
            // Branch if contents of regA and regB are the same
            // branch address = pc+1+offset.
        }
        else if(opcode == 5){ // jalr
            state.reg[regB] = state.pc + 1;
            state.pc = state.reg[regA]-1;
            // Store the value pc+1 into regB
            // Then branch to address contained regA
        }
        else if(opcode == 6){ // halt
            
        }
        else if(opcode == 7){ // noop
    
        }

        state.pc++;
        pcCounter++;
    }

    printState(&state);
    printf("machine halted\n");
    printf("total of %d instructions executed\n", pcCounter+1);
    printf("final state of machine:\n");
    state.pc++;
    printState(&state);

    //Your code ends here! 

    return(0);
}

/*
* DO NOT MODIFY ANY OF THE CODE BELOW. 
*/

void 
printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) 
              printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) 
              printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    printf("end state\n");
}

// convert a 16-bit number into a 32-bit Linux integer
static inline int convertNum(int num) 
{
    return num - ( (num & (1<<15)) ? 1<<16 : 0 );
}

/*
* Write any helper functions that you wish down here. 
*/