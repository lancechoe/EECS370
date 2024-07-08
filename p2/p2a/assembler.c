/**
 * Project 1
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
static inline int isNumber(char *);
int transRinstruct(char*, char*, char*, char*);
int transIinstruct(char*, char*, char*);
int transJinstruct(char*, char*, char*);
int transOinstruct(char*);
void checkReg(char*);

int
main(int argc, char **argv)
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r"); 
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w"); 
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    int fillCount = 0;
    int instCount = 0;
    int lineCounter = 0;
    char labelArray[MAXLINELENGTH][7]; 

    // -------------- Project 2 ----------------

    struct symbols{
        char global[7];
        char type[2];
        int lineOffset;
    };

    struct relocations{
        int lineOffset;
        char opcode[7];
        char label[7];
    };

    struct symbols symbolTable[50];
    struct relocations relocTable[50];

    int countGlobals = 0;
    int countReloc = 0;

    // -----------------------------------------

    
    // counting fill, inst
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        if(!strcmp(opcode, ".fill")){
            fillCount++;
        }
        else{
            instCount++;
        }
    }

    rewind(inFilePtr);

    // Get all the labels in the labelArray
    // the address of labelArray[lineCounter] is lineCounter 
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        for(int i = 0; i < lineCounter; i++){
              if
              (!strcmp(labelArray[i], label) && strlen(labelArray[i]) != 0){
                  printf("error : duplicate definition of labels '%s'\n", label);
                  exit(1); // Duplicate
              }
        }
        if(isNumber(label) == 1){
            exit(1);
        }
        strcpy(labelArray[lineCounter], label);

        // -------------- Project 2 ----------------

        // Put global (defined) label to symbolTable
        if(isupper(label[0])){ 
            // Name
            strcpy(symbolTable[countGlobals].global, label);

            // Type & Offset
            if(!strcmp(opcode, ".fill")){
                strcpy(symbolTable[countGlobals].type, "D");
                symbolTable[countGlobals].lineOffset = lineCounter - instCount; 
            }
            else{
                strcpy(symbolTable[countGlobals].type, "T");
                symbolTable[countGlobals].lineOffset = lineCounter;
            }

            countGlobals++;
        }

        // -----------------------------------------

        lineCounter++;
    }
    rewind(inFilePtr);

    // -------------- Project 2 ----------------

    int curLine = 0;

    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        
        if(!strcmp(opcode, "lw") || !strcmp(opcode, "sw")){ // Relocation needed in lw,sw
            if(!isNumber(arg2)){ // if it is not number -> global label or local
                relocTable[countReloc].lineOffset = curLine; // lineOffset
                strcpy(relocTable[countReloc].opcode, opcode);
                strcpy(relocTable[countReloc].label, arg2);
                countReloc++;

                // Look for (undefined) global label
                if(isupper(arg2[0])){
                    bool found = false;

                    for(int i = 0; i < lineCounter; i++){
                        if(!strcmp(labelArray[i], arg2)){
                            found = true;
                        }
                    }
                    for(int i = 0; i < countGlobals; i++){
                        if(!strcmp(symbolTable[i].global, arg2)){
                            found = true;
                        }
                    }
                    if(!found){
                        strcpy(symbolTable[countGlobals].global, arg2);
                        strcpy(symbolTable[countGlobals].type, "U");
                        symbolTable[countGlobals].lineOffset = 0;
                        countGlobals++;
                    }
                }
            }
        }

        else if(!strcmp(opcode, ".fill")){
            if(!isNumber(arg0)){
                relocTable[countReloc].lineOffset = curLine - instCount;
                strcpy(relocTable[countReloc].opcode, opcode);
                strcpy(relocTable[countReloc].label, arg0);
                countReloc++;
            }

            if(isupper(arg0[0])){
                bool found = false;

                    for(int i = 0; i < lineCounter; i++){
                        if(!strcmp(labelArray[i], arg0)){
                            found = true;
                        }
                    }
                    for(int i = 0; i < countGlobals; i++){
                        if(!strcmp(symbolTable[i].global, arg0)){
                            found = true;
                        }
                    }
                    if(!found){
                        strcpy(symbolTable[countGlobals].global, arg0);
                        strcpy(symbolTable[countGlobals].type, "U");
                        symbolTable[countGlobals].lineOffset = 0;
                        countGlobals++;
                    }
            }
        }
        
        curLine++;
    }

    // Print Header
    fprintf(outFilePtr, "%d %d %d %d\n", instCount, fillCount, countGlobals, countReloc);

    // -----------------------------------------


    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);
    int address = 0;

    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        // if(address < (lineCounter-fillCount) && !strcmp(opcode, ".fill")){
        //     printf("error : .fill is before other instructions\n");
        //     exit(1);
        // }
        
        if(!strcmp(opcode, "add") || !strcmp(opcode, "nor")){
            checkReg(arg0);
            checkReg(arg1);
            checkReg(arg2);
            fprintf(outFilePtr, "%d\n", transRinstruct(opcode, arg0, arg1, arg2));
        }
        
        else if
        (!strcmp(opcode,"lw") || !strcmp(opcode,"sw")){
            checkReg(arg0);
            checkReg(arg1);
            int offsetDecimal = -1;
            if(isNumber(arg2) == 1){
                if(atoi(arg2) < -32768 || atoi(arg2) > 32767){
                    printf("error : offsetfields don't fit in 16bits\n");
                    exit(1);
                }
                offsetDecimal = atoi(arg2);
            }
            else{
                for(int i=0; i<lineCounter; i++){
                    if(!strcmp(arg2, labelArray[i])){
                        offsetDecimal = i;
                    }
                }
                if(offsetDecimal == -1){
                    if(isupper(arg2[0])){
                        offsetDecimal = 0;
                    }
                    else{
                        printf("error : use of undefined labels '%s'\n", arg2);
                        exit(1); // Unused label
                    }
                }
            }
            if(offsetDecimal < -32768 || offsetDecimal > 32767){
                    printf("error : offsetfields don't fit in 16bits\n");
                    exit(1);
            }
            offsetDecimal &= 0xffff;
            fprintf
            (outFilePtr, "%d\n", transIinstruct(opcode, arg0, arg1)+offsetDecimal);
        }
        
        else if(!strcmp(opcode,"beq")){
            checkReg(arg0);
            checkReg(arg1);
            int offsetDecimal = -1;
            if(isNumber(arg2) == 1){
                if(atoi(arg2) < -32768 || atoi(arg2) > 32767){
                    printf("error : offsetfields don't fit in 16bits\n");
                    exit(1);
                }
                offsetDecimal = atoi(arg2);
            }
            else{
                for(int i=0; i<lineCounter; i++){
                    if(!strcmp(arg2, labelArray[i])){
                        offsetDecimal = i;
                    }
                }
                if(offsetDecimal == -1){
                    exit(1); // Unused label
                }
                offsetDecimal = offsetDecimal - 1 - address;
            }

            if(offsetDecimal < -32768 || offsetDecimal > 32767){
                    printf("error : offsetfields don't fit in 16bits\n");
                    exit(1);
            }
            offsetDecimal &= 0xffff;

            fprintf
            (outFilePtr, "%d\n", transIinstruct(opcode, arg0, arg1)+offsetDecimal);
        }
        
        else if(!strcmp(opcode, "jalr")){
            checkReg(arg0);
            checkReg(arg1);
            
            fprintf(outFilePtr, "%d\n", transJinstruct(opcode, arg0, arg1));
        }
        else if(!strcmp(opcode, "halt") || !strcmp(opcode, "noop")){
            
            fprintf(outFilePtr, "%d\n", transOinstruct(opcode));
        }
        else if(!strcmp(opcode, ".fill")){
            
            int b = -1;
            if(isNumber(arg0) == 1){
                b = atoi(arg0);
            }
            else{
                for(int i = 0; i < lineCounter; i++){
                    if(!strcmp(arg0, labelArray[i])){
                        b = i;
                        if(b < -32768 || b > 32767){
                            exit(1); // Error : offset out of range
                        }
                    }
                }
                if(b == -1){
                    if(isupper(arg0[0])){
                        b = 0;
                    }
                    else{
                    exit(1); // undefined label
                    }
                }
            }
            fprintf(outFilePtr, "%d\n", b);
        }
        else{
            printf("error : unrecognized opcdes '%s'\n", opcode);
            exit(1); // Error : Unrecognized opcodes
        }

        address++; 
    }

    // Print Symbol Table
    for(int i = 0; i < countGlobals; i++){
        fprintf(outFilePtr, "%s %s %d\n", symbolTable[i].global, symbolTable[i].type, symbolTable[i].lineOffset);
    }

    // Print Reloc Table
    for(int i = 0; i < countReloc; i++){
        fprintf(outFilePtr, "%d %s %s\n", relocTable[i].lineOffset, relocTable[i].opcode, relocTable[i].label);
    }

    return(0);
}

/*
* NOTE: The code defined below is not to be modifed as it is implimented correctly.
*/

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
    char *arg1, char *arg2)
{
    char line[MAXLINELENGTH]; 
    char *ptr = line;  

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) { 
        // read from inFilePtr and store it into line
        // it stops when newline character is read
        // if this is null it means nothing left on this line
	/* reached end of file */
        return(0);
    }

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH-1) {
	printf("error: line too long\n");
	exit(1);
    }

    // Treat a blank line as end of file.
    // Arguably, we could just ignore and continue, but that could
    // get messy in terms of label line numbers etc.
    char whitespace[4] = {'\t', '\n', '\r', ' '};
    int nonempty_line = 0;
    for(size_t line_idx = 0; line_idx < strlen(line); ++line_idx) {
        int line_char_is_whitespace = 0;
        for(int whitespace_idx = 0; whitespace_idx < 4; ++ whitespace_idx) {
            if(line[line_idx] == whitespace[whitespace_idx]) {
                ++line_char_is_whitespace;
                break;
            }
        }
        // If it all whitespace, then it would not pass this line so
        // the nonempty_line = 0
        if(!line_char_is_whitespace) {
            ++nonempty_line;
            break;
        }
    }
    if(nonempty_line == 0) {
        return 0;
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) { // store the value into label
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf
    (ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);

    return(1);
}

static inline int
isNumber(char *string)
{
    int num;
    char c;
    return((sscanf(string, "%d%c",&num, &c)) == 1);
}

int transRinstruct(char *opcode, char *arg0, char *arg1, char *arg2){
    
    int opcodeB = 0;
    int reg1B;
    int reg2B;
    int decimal = 0;

    if(!strcmp(opcode, "add")){
        opcodeB = 0;
    }
    if(!strcmp(opcode, "nor")){
        opcodeB = 1;
    }
    opcodeB = opcodeB << 22;
    decimal += opcodeB;

    // Need to check if it is inside [0,7]
    reg1B = atoi(arg0) << 19;
    decimal += reg1B;

    reg2B = atoi(arg1) << 16;
    decimal += reg2B;

    decimal += atoi(arg2);

    return decimal;
}

int transIinstruct(char *opcode, char *arg0, char *arg1){
    
    int opcodeB = 0;
    int reg1B;
    int reg2B;
    int decimal = 0;

    if(!strcmp(opcode, "lw")){
        opcodeB = 2;
    }
    if(!strcmp(opcode, "sw")){
        opcodeB = 3;
    }
    if(!strcmp(opcode, "beq")){
        opcodeB = 4;
    }
    opcodeB = opcodeB << 22;
    decimal += opcodeB;

    // Need to check if it is inside [0,7]
    reg1B = atoi(arg0) << 19;
    decimal += reg1B;

    reg2B = atoi(arg1) << 16;
    decimal += reg2B;
    
    return decimal;
}

int transJinstruct(char *opcode, char *arg0, char *arg1){
    
    int opcodeB = 0;
    int reg1B;
    int reg2B;
    int decimal = 0;

    if(!strcmp(opcode, "jalr")){
        opcodeB = 5;
    }
    opcodeB = opcodeB << 22;
    decimal += opcodeB;

    // Need to check if it is inside [0,7]
    reg1B = atoi(arg0) << 19;
    decimal += reg1B;

    reg2B = atoi(arg1) << 16;
    decimal += reg2B;


    return decimal;
}

int transOinstruct(char *opcode){
    
    int opcodeB;
    int decimal = 0;

    if(!strcmp(opcode, "halt")){
        opcodeB = 6;
    }
    if(!strcmp(opcode, "noop")){
        opcodeB = 7;
    }
    opcodeB = opcodeB << 22;
    decimal += opcodeB;

    return decimal;
}

void checkReg(char *arg0){
    int n = 0;
    int m = atoi(arg0);
    if(m % 1 != 0){
        printf("error : non-integer register arguments\n");
        exit(1);
    }
    if(sscanf(arg0, "%d", &n) != 1){
        printf("error : non-integer register arguments\n");
        exit(1);
    }
    if(m < 0 || m > 7){
        printf("error : registers outside the range [0,7]\n");
        exit(1);
    }
}
