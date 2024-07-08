/**
 * Project 2
 * LC-2K Linker
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 300
#define MAXLINELENGTH 1000
#define MAXFILES 6

typedef struct FileData FileData;
typedef struct SymbolTableEntry SymbolTableEntry;
typedef struct RelocationTableEntry RelocationTableEntry;
typedef struct CombinedFiles CombinedFiles;

struct SymbolTableEntry {
	char label[7];
	char location;
	int offset;
};

struct RelocationTableEntry {
	int offset;
	char inst[7];
	char label[7];
	int file;
};

struct FileData {
	int textSize;
	int dataSize;
	int symbolTableSize;
	int relocationTableSize;
	int textStartingLine; // in final executable
	int dataStartingLine; // in final executable
	int text[MAXSIZE];
	int data[MAXSIZE];
	SymbolTableEntry symbolTable[MAXSIZE];
	RelocationTableEntry relocTable[MAXSIZE];
};

struct CombinedFiles {
	int text[MAXSIZE];
	int data[MAXSIZE];
	SymbolTableEntry     symTable[MAXSIZE];
	RelocationTableEntry relocTable[MAXSIZE];
	int textSize;
	int dataSize;
	int symTableSize;
	int relocTableSize;
};

int main(int argc, char *argv[])
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr; 
	int i, j;

	if (argc <= 2) {
		printf("error: usage: %s <obj file> ... <output-exe-file>\n",
				argv[0]);
		exit(1);
	}

	outFileString = argv[argc - 1];

	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	FileData files[MAXFILES];

  // read in all files and combine into a "master" file
	for (i = 0; i < argc - 2; i++) {
		inFileString = argv[i+1];

		inFilePtr = fopen(inFileString, "r");
		printf("opening %s\n", inFileString);

		if (inFilePtr == NULL) {
			printf("error in opening %s\n", inFileString);
			exit(1);
		}

		char line[MAXLINELENGTH];
		int sizeText, sizeData, sizeSymbol, sizeReloc;

		// parse first line of file
		fgets(line, MAXSIZE, inFilePtr);
		sscanf(line, "%d %d %d %d",
				&sizeText, &sizeData, &sizeSymbol, &sizeReloc);

		files[i].textSize = sizeText;
		files[i].dataSize = sizeData;
		files[i].symbolTableSize = sizeSymbol;
		files[i].relocationTableSize = sizeReloc;

		// read in text section
		int instr;
		for (j = 0; j < sizeText; j++) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			instr = atoi(line);
			files[i].text[j] = instr;
		}

		// read in data section
		int data;
		for (j = 0; j < sizeData; j++) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			data = atoi(line);
			files[i].data[j] = data;
		}

		// read in the symbol table
		char label[7];
		char type;
		int addr;
		for (j = 0; j < sizeSymbol; j++) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			sscanf(line, "%s %c %d",
					label, &type, &addr);
			files[i].symbolTable[j].offset = addr;
			strcpy(files[i].symbolTable[j].label, label);
			files[i].symbolTable[j].location = type;
		}

		// read in relocation table
		char opcode[7];
		for (j = 0; j < sizeReloc; j++) {
			fgets(line, MAXLINELENGTH, inFilePtr);
			sscanf(line, "%d %s %s",
					&addr, opcode, label);
			files[i].relocTable[j].offset = addr;
			strcpy(files[i].relocTable[j].inst, opcode);
			strcpy(files[i].relocTable[j].label, label);
			files[i].relocTable[j].file	= i;
		}
		fclose(inFilePtr);
	} // end reading files

	// *** INSERT YOUR CODE BELOW ***
	//    Begin the linking process
	//    Happy coding!!!

	CombinedFiles linkedFiles;
	int countText = 0;
	int countData = 0;
	for(int i = 0; i < argc-2; i++){
		countText = countText + files[i].textSize;
		countData = countData + files[i].dataSize;
	}

	// Writing text of linkedFiles
	for(int i = 0, j = 0; j < argc - 2; j++){
		files[j].textStartingLine = i; // starting from 0
		for(int k = 0; k < files[j].textSize; k++, i++){
			linkedFiles.text[i] = files[j].text[k];
			linkedFiles.textSize++;
		}
	}

	// Writing data of linkedFiles
	for(int i = 0, j = 0; j < argc - 2; j++){
		files[j].dataStartingLine = i + linkedFiles.textSize;
		for(int k = 0; k < files[j].textSize; k++, i++){
			linkedFiles.data[i] = files[j].data[k];
			linkedFiles.dataSize++;
		}
	}

	// symbol table
	linkedFiles.symTableSize = 0;
	for(int i = 0; i < argc - 2; i++){
		for(int j = 0; j < files[i].symbolTableSize; j++){
			if(files[i].symbolTable[j].location == 'U'){
				continue;
			}

			if(!strcmp(files[i].symbolTable[j].label, "Stack")){
				printf("Stack in object file.\n");
				exit(1);
			}

			for(int k = 0; k < linkedFiles.symTableSize; k++){
				if(!strcmp(linkedFiles.symTable[k].label, files[i].symbolTable[j].label)){
					printf("Duplicate.\n");
					exit(1);
				}
			}

			// label
			strcpy(linkedFiles.symTable[linkedFiles.symTableSize].label, files[i].symbolTable[j].label);
			// location
			linkedFiles.symTable[linkedFiles.symTableSize].location = files[i].symbolTable[j].location;
			
			// offset
			linkedFiles.symTable[linkedFiles.symTableSize].offset = files[i].symbolTable[j].offset;
			// 기존 offset 에서 더해
			if(files[i].symbolTable[j].location == 'T'){
				for(int k = 0; k < i; k++){
					linkedFiles.symTable[linkedFiles.symTableSize].offset += files[k].textSize; // X
				}
				//files[i].symbolTable[j].offset + linkedFiles.textSize;
			}
			else{
				for(int k = 0; k < i; k++){
					linkedFiles.symTable[linkedFiles.symTableSize].offset += files[k].dataSize;
					
				}
				//files[i].symbolTable[j].offset + linkedFiles.dataSize + countText;
			}
			linkedFiles.symTableSize++;
		}
	}

	// reloc
	for(int i = 0; i < argc - 2; i++){
		for(int j = 0; j < files[i].relocationTableSize; j++){
			// .fill
			if(files[i].relocTable[j].inst[0] == '.'){ // .fill
				if(!isupper(files[i].relocTable[j].label[0])){ // if local label	
					if((files[i].data[files[i].relocTable[j].offset]&0xFFFF) < files[i].textSize){ // label in T ?
						files[i].data[files[i].relocTable[j].offset] += files[i].textStartingLine;
					}
					else{ // label in D ?
						files[i].data[files[i].relocTable[j].offset] += files[i].textStartingLine;
						for(int k = i+1; k < argc-2; k++){
							files[i].data[files[i].relocTable[j].offset] += files[k].textSize;
						}
						for(int k = 0; k < i; k++){
							files[i].data[files[i].relocTable[j].offset] += files[k].dataSize;
						}
					}
				}
				else{ // Global
					int numSym = 0;
					while(strcmp(files[i].symbolTable[numSym].label, files[i].relocTable[j].label) && numSym < files[i].symbolTableSize){
						numSym++;
					}
					if(numSym == files[i].symbolTableSize){
						if(!strcmp(files[i].relocTable[j].label, "Stack")){
							files[i].data[files[i].relocTable[j].offset] = countData + countText;
							
						}
						else{
							printf("Undefined global label\n");
							exit(1);
						}
					}
					else{
						int linkedOffset = 0; // linked symbol table index
						while(strcmp(linkedFiles.symTable[linkedOffset].label, files[i].relocTable[j].label)){
							linkedOffset++;
						}
						if(linkedFiles.symTable[linkedOffset].location == 'D'){
							files[i].data[files[i].relocTable[j].offset] = // += -> = 여기 고침
							linkedFiles.textSize + linkedFiles.symTable[linkedOffset].offset;
						}
						else if(linkedFiles.symTable[linkedOffset].location == 'T'){
							files[i].data[files[i].relocTable[j].offset] = // += -> = 여기 고침
							linkedFiles.symTable[linkedOffset].offset;
						}
						else{
							printf("Undefined global used\n");
							exit(1);
						}
					}
				}
			}
			// lw,sw
			else{
				int test = -1;
				for(size_t a = 0; a < files[i].relocationTableSize; a++){
					if(!strcmp(files[i].relocTable[j].label,files[i].symbolTable[a].label)
					&& files[i].symbolTable[a].location != 'U'){
						test = 0;
					}
				}

				if(!isupper(files[i].relocTable[j].label[0]) || 
					(isupper(files[i].relocTable[j].label[0]) && test == 0)){ // if local label
					test = -1;
					if((files[i].text[files[i].relocTable[j].offset] & 0xFFFF) < files[i].textSize){ // label in T ?
						files[i].text[files[i].relocTable[j].offset] += files[i].textStartingLine;
					}
					else{ // label in D ?
						files[i].text[files[i].relocTable[j].offset] += files[i].textStartingLine;
						for(int k = i+1; k < argc-2; k++){
							files[i].text[files[i].relocTable[j].offset] += files[k].textSize;
						}
						for(int k = 0; k < i; k++){
							files[i].text[files[i].relocTable[j].offset] += files[k].dataSize;
						}
						// files[i].text[files[i].relocTable[j].offset] += (countText - files[i].textSize);
						// + files[i].dataStartingLine;
						// if 0 lw five << offset = 6 이야 여기선
						// 근데 이 사이에 다음의 파일들의 textsize + 이전의 파일들의 datasize
					}
				}
				else if(isupper(files[i].relocTable[j].label[0])){ // if Global label
					test = -1;
					int numSym = 0; // symbol index
					while(strcmp(linkedFiles.symTable[numSym].label, files[i].relocTable[j].label) && numSym < linkedFiles.symTableSize){
						numSym++;
					}
					if(numSym == linkedFiles.symTableSize){
						if(!strcmp(files[i].relocTable[j].label, "Stack")){
							files[i].text[files[i].relocTable[j].offset] += countData + countText;
						}
						else{
							printf("Undefined global label\n");
							exit(1);
						}
					}
					else{
						int linkedOffset = 0; // linked symbol table index
						while(strcmp(linkedFiles.symTable[linkedOffset].label, files[i].relocTable[j].label)){
							linkedOffset++;
						}
						if(linkedFiles.symTable[linkedOffset].location == 'D'){ 
							files[i].text[files[i].relocTable[j].offset] += 
							linkedFiles.textSize + linkedFiles.symTable[linkedOffset].offset; 
						}
						else if(linkedFiles.symTable[linkedOffset].location == 'T'){
							files[i].text[files[i].relocTable[j].offset] += 
							linkedFiles.symTable[linkedOffset].offset; 
						}
						else{
							printf("Undefined global label\n");
							exit(1);
						}
					}
				}
			}
		}
	}

	int curText = 0;
	int curData = 0;
	for(int i = 0; i < argc - 2; i++){
		for(int j = 0; j < files[i].textSize; j++){
			linkedFiles.text[curText + j] = files[i].text[j];
		}

		for(int j = 0; j < files[i].dataSize; j++){
			linkedFiles.data[curData + j] = files[i].data[j];
		}

		curText += files[i].textSize;
		curData += files[i].dataSize;
	}

	for(int i = 0; i < countText; i++){
		fprintf(outFilePtr, "%d\n", linkedFiles.text[i]);
	}
	for(int i = 0; i < countData; i++){
		fprintf(outFilePtr, "%d\n", linkedFiles.data[i]);
	}

} // main
