/**
 * Project 2
 * LC-2K Linker
 */

// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
// #include <ctype.h>

// #define MAXSIZE 300
// #define MAXLINELENGTH 1000
// #define MAXFILES 6

// typedef struct FileData FileData;
// typedef struct SymbolTableEntry SymbolTableEntry;
// typedef struct RelocationTableEntry RelocationTableEntry;
// typedef struct CombinedFiles CombinedFiles;

// struct SymbolTableEntry {
// 	char label[7];
// 	char location;
// 	int offset;
// };

// struct RelocationTableEntry {
// 	int offset;
// 	char inst[7];
// 	char label[7];
// 	int file;
// };

// struct FileData {
// 	int textSize;
// 	int dataSize;
// 	int symbolTableSize;
// 	int relocationTableSize;
// 	int textStartingLine; // in final executable
// 	int dataStartingLine; // in final executable
// 	int text[MAXSIZE];
// 	int data[MAXSIZE];
// 	SymbolTableEntry symbolTable[MAXSIZE];
// 	RelocationTableEntry relocTable[MAXSIZE];
// };

// struct CombinedFiles {
// 	int text[MAXSIZE];
// 	int data[MAXSIZE];
// 	SymbolTableEntry     symTable[MAXSIZE];
// 	RelocationTableEntry relocTable[MAXSIZE];
// 	int textSize;
// 	int dataSize;
// 	int symTableSize;
// 	int relocTableSize;
// };

// //
// int linkG(char* label, int l, FileData files[], struct CombinedFiles* linkedFiles, int argc, int i){
// 	int offset = -1;
// 	for(int a = 0; a < argc-2; a++){

// 		for(int b = 0; b < files[a].symbolTableSize; b++){
// 			if(!strcmp(label, files[a].symbolTable[b].label)){
// 				if(a != i){
// 					if(files[a].symbolTable[b].location == 'T'){
// 						offset = files[a].textStartingLine + files[a].symbolTable[b].offset;
// 					}
// 					else if(files[a].symbolTable[b].location == 'D'){
// 						offset = files[a].dataStartingLine + files[a].symbolTable[b].offset;
// 					}
// 				}
// 			}
// 			else{
// 				if(files[a].symbolTable[b].location == 'T'){
// 					offset = 0;
// 					offset -= files[a].symbolTable[b].offset;
// 					offset += files[a].textStartingLine + files[a].symbolTable[b].offset;
// 				}
// 				else if(files[a].symbolTable[b].location == 'D'){
// 					offset = 0;
// 					offset -= files[a].textSize + files[a].symbolTable[b].offset;
// 					offset += files[a].dataStartingLine + files[a].symbolTable[b].offset;
// 				}
// 			}
// 		}
// 	}

// 	if(offset < 0){
// 		exit(1);
// 	}

// 	return offset;
// }


// int main(int argc, char *argv[])
// {
// 	char *inFileString, *outFileString;
// 	FILE *inFilePtr, *outFilePtr; 
// 	int i, j;

// 	if (argc <= 2) {
// 		printf("error: usage: %s <obj file> ... <output-exe-file>\n",
// 				argv[0]);
// 		exit(1);
// 	}

// 	outFileString = argv[argc - 1];

// 	outFilePtr = fopen(outFileString, "w");
// 	if (outFilePtr == NULL) {
// 		printf("error in opening %s\n", outFileString);
// 		exit(1);
// 	}

// 	FileData files[MAXFILES];

//   // read in all files and combine into a "master" file
// 	for (i = 0; i < argc - 2; i++) {
// 		inFileString = argv[i+1];

// 		inFilePtr = fopen(inFileString, "r");
// 		printf("opening %s\n", inFileString);

// 		if (inFilePtr == NULL) {
// 			printf("error in opening %s\n", inFileString);
// 			exit(1);
// 		}

// 		char line[MAXLINELENGTH];
// 		int sizeText, sizeData, sizeSymbol, sizeReloc;

// 		// parse first line of file
// 		fgets(line, MAXSIZE, inFilePtr);
// 		sscanf(line, "%d %d %d %d",
// 				&sizeText, &sizeData, &sizeSymbol, &sizeReloc);

// 		files[i].textSize = sizeText;
// 		files[i].dataSize = sizeData;
// 		files[i].symbolTableSize = sizeSymbol;
// 		files[i].relocationTableSize = sizeReloc;

// 		// read in text section
// 		int instr;
// 		for (j = 0; j < sizeText; j++) {
// 			fgets(line, MAXLINELENGTH, inFilePtr);
// 			instr = atoi(line);
// 			files[i].text[j] = instr;
// 		}

// 		// read in data section
// 		int data;
// 		for (j = 0; j < sizeData; j++) {
// 			fgets(line, MAXLINELENGTH, inFilePtr);
// 			data = atoi(line);
// 			files[i].data[j] = data;
// 		}

// 		// read in the symbol table
// 		char label[7];
// 		char type;
// 		int addr;
// 		for (j = 0; j < sizeSymbol; j++) {
// 			fgets(line, MAXLINELENGTH, inFilePtr);
// 			sscanf(line, "%s %c %d",
// 					label, &type, &addr);
// 			files[i].symbolTable[j].offset = addr;
// 			strcpy(files[i].symbolTable[j].label, label);
// 			files[i].symbolTable[j].location = type;
// 		}

// 		// read in relocation table
// 		char opcode[7];
// 		for (j = 0; j < sizeReloc; j++) {
// 			fgets(line, MAXLINELENGTH, inFilePtr);
// 			sscanf(line, "%d %s %s",
// 					&addr, opcode, label);
// 			files[i].relocTable[j].offset = addr;
// 			strcpy(files[i].relocTable[j].inst, opcode);
// 			strcpy(files[i].relocTable[j].label, label);
// 			files[i].relocTable[j].file	= i;
// 		}
// 		fclose(inFilePtr);
// 	} // end reading files

// 	// *** INSERT YOUR CODE BELOW ***
// 	//    Begin the linking process
// 	//    Happy coding!!!

// 	CombinedFiles linkedFiles;
// 	linkedFiles.textSize = 0;
// 	linkedFiles.dataSize = 0;
// 	linkedFiles.symTableSize = 0;
// 	linkedFiles.relocTableSize = 0;

// 	//////////////

// 	int idx = 0;
// 	for(int i = 0; i < argc-2; i++){
// 		for(int j = 0; j < files[i].symbolTableSize; j++){
// 			if(files[i].symbolTable[j].location != 'U' && !strcmp("Stack", files[i].symbolTable[j].label)){
// 				exit(1);
// 			}
// 			// checking label
// 			for(int k = 0; k < linkedFiles.symTableSize; k++){
// 				if(!strcmp(linkedFiles.symTable[k].label, files[i].symbolTable[j].label)){
// 					if(files[i].symbolTable[j].location != 'U' && linkedFiles.symTable[k].location != 'U'){
// 						exit(1);
// 					}
// 				}
// 			}

// 			strcpy(linkedFiles.symTable[idx].label, files[i].symbolTable[j].label);
// 			linkedFiles.symTable[idx].location = files[i].symbolTable[j].location;
// 			linkedFiles.symTable[idx].offset = files[i].symbolTable[j].offset;
// 			linkedFiles.symTableSize++;
// 			idx++;
// 		}
// 	}

// 	idx = 0;
// 	for(int i = 0; i < argc-2; i++){
// 		for(int j = 0; j < files[i].relocationTableSize; j++){
// 			linkedFiles.relocTable[idx].offset = files[i].relocTable[j].offset;
// 			strcpy(linkedFiles.relocTable[idx].label, files[i].relocTable[j].label);
// 			strcpy(linkedFiles.relocTable[idx].inst, files[i].relocTable[j].inst);
// 			linkedFiles.relocTable[idx].file = i;
// 			linkedFiles.relocTableSize++;
// 			idx++;
// 		}
// 	}

// 	idx = 0;
// 	for(int i = 0; i < argc-2; i++){
// 		files[i].textStartingLine = idx;
// 		for(int j = 0; j < files[i].textSize; j++){
// 			linkedFiles.text[idx] = files[i].text[j];
// 			linkedFiles.textSize++;
// 			idx++;
// 		}
// 	}

// 	idx = 0;
// 	for(int i = 0; i < argc-2; i++){
// 		files[i].dataStartingLine = idx + linkedFiles.textSize;
// 		for(int j = 0; j < files[i].dataSize; j++){
// 			linkedFiles.data[idx] = files[i].data[j];
// 			linkedFiles.dataSize++;
// 			idx++;
// 		}
// 	}

// 	strcpy(linkedFiles.symTable[linkedFiles.symTableSize].label, "Stack");
// 	linkedFiles.symTable[linkedFiles.symTableSize].offset = linkedFiles.textSize + linkedFiles.dataSize;
// 	linkedFiles.symTable[linkedFiles.symTableSize].location = 'G';
// 	linkedFiles.symTableSize++;

// 	for(int l = 0, i = 0; i < argc-2; i++){
// 		if(i == argc -3){
// 			for(; l < linkedFiles.textSize; l++){
// 				// 
// 				int num = l - files[i].textStartingLine;
// 				for(int j = 0 ; j < linkedFiles.relocTableSize; j++){
// 					if(linkedFiles.relocTable[j].offset == num && linkedFiles.relocTable[j].file == i && strcmp(".fill", linkedFiles.relocTable[j].inst)){
// 						if(isupper(linkedFiles.relocTable[j].label[0])){
// 							if(!strcmp("Stack", linkedFiles.relocTable[j].label)){
// 								linkedFiles.text[l] += linkedFiles.symTable[linkedFiles.symTableSize-1].offset;
// 							}
// 							else{
// 								linkedFiles.text[l] += linkG(linkedFiles.relocTable[j].label, l, files, &linkedFiles, argc, i);
// 							}
// 						}
// 					}
// 					else{
// 						if((uint16_t)linkedFiles.text[l] >= (uint16_t)files[i].textSize){
// 							linkedFiles.text[l] += files[i].textStartingLine;
// 							for(int k = i+1; k < argc-2; k++){
// 								linkedFiles.text[l] += files[k].textSize;
// 							}
// 							for(int k = 0; k != i; k++){
// 								linkedFiles.text[l] += files[k].dataSize;
// 							}
// 						}
// 						else{
// 							linkedFiles.text[l] += files[i].textStartingLine;
// 						}
// 					}
// 				}
// 			}
// 		}
// 		else{
// 			for(; l < files[i+1].textStartingLine; l++){
// 				int num = l - files[i].textStartingLine;
// 				for(int j = 0 ; j < linkedFiles.relocTableSize; j++){
// 					if(linkedFiles.relocTable[j].offset == num && linkedFiles.relocTable[j].file == i && strcmp(".fill", linkedFiles.relocTable[j].inst)){
// 						if(isupper(linkedFiles.relocTable[j].label[0])){
// 							if(!strcmp("Stack", linkedFiles.relocTable[j].label)){
// 								linkedFiles.text[l] += linkedFiles.symTable[linkedFiles.symTableSize-1].offset;
// 							}
// 							else{
// 								linkedFiles.text[l] += linkG(linkedFiles.relocTable[j].label, l, files, &linkedFiles, argc, i);
// 							}
// 						}
// 					}
// 					else{
// 						if((uint16_t)linkedFiles.text[l] >= (uint16_t)files[i].textSize){
// 							linkedFiles.text[l] += files[i].textStartingLine;
// 							for(int k = i+1; k < argc-2; k++){
// 								linkedFiles.text[l] += files[k].textSize;
// 							}
// 							for(int k = 0; k != i; k++){
// 								linkedFiles.text[l] += files[k].dataSize;
// 							}
// 						}
// 						else{
// 							linkedFiles.text[l] += files[i].textStartingLine;
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}

// 	for(int l = linkedFiles.textSize, i=0; i < argc-2; i++){
// 		if(i == argc - 3){
// 			for(; l < linkedFiles.dataSize + linkedFiles.textSize; l++){
// 				//
// 				int num = l - files[i].dataStartingLine;
// 				int line =  l - linkedFiles.textSize;
// 				for(int j = 0; j < linkedFiles.relocTableSize; j++){
// 					if(linkedFiles.relocTable[j].offset == num && linkedFiles.relocTable[j].file == i && !strcmp(".fill", linkedFiles.relocTable[j].inst)){
// 						if(isupper(linkedFiles.relocTable[j].label[0])){
// 							if(!strcmp("Stack", linkedFiles.relocTable[j].label)){
// 								linkedFiles.data[line] += linkedFiles.symTable[linkedFiles.symTableSize -1].offset;
// 							}
// 							else{
// 								linkedFiles.data[line] += linkG(linkedFiles.relocTable[j].label, l, files, &linkedFiles, argc, i);
// 							}
// 						}
// 						else{
// 							if((uint16_t)linkedFiles.data[line] >= (uint16_t)files[i].textSize){
// 								linkedFiles.data[line] += files[i].textStartingLine;
// 								for(int k = i+1; k < argc-2; k++){
// 									linkedFiles.data[line] += files[k].textSize;
// 								}
// 								for(int k = 0; k != i; k++){
// 									linkedFiles.data[line] += files[k].dataSize;
// 								}
// 							}
// 							else{
// 								linkedFiles.data[line] += files[i].textStartingLine;
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
// 		else{
// 			for(; l < files[i+1].dataStartingLine; l++){
// 				int num = l - files[i].dataStartingLine;
// 				int line =  l - linkedFiles.textSize;
// 				for(int j = 0; j < linkedFiles.relocTableSize; j++){
// 					if(linkedFiles.relocTable[j].offset == num && linkedFiles.relocTable[j].file == i && !strcmp(".fill", linkedFiles.relocTable[j].inst)){
// 						if(isupper(linkedFiles.relocTable[j].label[0])){
// 							if(!strcmp("Stack", linkedFiles.relocTable[j].label)){
// 								linkedFiles.data[line] += linkedFiles.symTable[linkedFiles.symTableSize -1].offset;
// 							}
// 							else{
// 								linkedFiles.data[line] += linkG(linkedFiles.relocTable[j].label, l, files, &linkedFiles, argc, i);
// 							}
// 						}
// 						else{
// 							if((uint16_t)linkedFiles.data[line] >= (uint16_t)files[i].textSize){
// 								linkedFiles.data[line] += files[i].textStartingLine;
// 								for(int k = i+1; k < argc-2; k++){
// 									linkedFiles.data[line] += files[k].textSize;
// 								}
// 								for(int k = 0; k != i; k++){
// 									linkedFiles.data[line] += files[k].dataSize;
// 								}
// 							}
// 							else{
// 								linkedFiles.data[line] += files[i].textStartingLine;
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}

// 	printf("%d %d %d %d\n", linkedFiles.textSize, linkedFiles.dataSize, linkedFiles.symTableSize, linkedFiles.relocTableSize);
	
	
// 	for(int a = 0; a < linkedFiles.textSize; a++){
// 		fprintf(outFilePtr, "%d\n", linkedFiles.text[a]);
// 	}
// 	for(int a = 0; a < linkedFiles.dataSize; a++){
// 		fprintf(outFilePtr, "%d\n", linkedFiles.data[a]);
// 	}


// 	for(int a = 0; a<linkedFiles.symTableSize; a++){
// 		printf("%s %c %d\n", linkedFiles.symTable[a].label, linkedFiles.symTable[a].location, linkedFiles.symTable[a].offset);
// 	}
// 	for(int a = 0; a<linkedFiles.relocTableSize; a++){
// 		printf("%d %s %s < file[%d]\n",
// 			linkedFiles.relocTable[a].offset, linkedFiles.relocTable[a].inst, linkedFiles.relocTable[a].label, linkedFiles.relocTable[a].file);
// 	}

//} // main
