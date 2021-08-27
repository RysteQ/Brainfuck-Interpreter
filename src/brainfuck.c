#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 256

struct programInformation {
    int programSize;
    char* programData;
    char memory[MEMORY_SIZE];
};


#pragma region declareFuncs

// runtime functions
void initProgramInformationStruct(struct programInformation* toInit);
void readFileData(FILE* inputFile, struct programInformation* program);
void brainfuckInterpreter(struct programInformation program);

// debug functions
void displayProgramInformationData(struct programInformation program);

#pragma endregion declareFuncs


/*
Name: brainfuck.c
Input: Filename
Output: None
Description: This is my implementation of the brainfuck interpeter written in C, it takes the name of the input file, saves the
data of the file to the programInformation struct, and after that is done it starts executing the program in the
brainfuckInterpreter function.

Function order: initProgramInformationStruct -> readFileData -> brainfuckInterpreter
*/



int main(int argc, char** argv) {
    if (argc == 1) {
        printf("No input file detected !\n");
        return -1;
    }

    FILE *inputFile = fopen(argv[1], "r");

    struct programInformation program;  
    initProgramInformationStruct(&program);
    readFileData(inputFile, &program);
    fclose(inputFile);

    brainfuckInterpreter(program);

    return 0;
}


#pragma region funcs

// runtime function
void initProgramInformationStruct(struct programInformation* toInit) {
    for (int i = 0; i < MEMORY_SIZE; i++)
        toInit->memory[i] = 0;

    toInit->programData = malloc(sizeof(char));
    toInit->programSize = 1;
}

// runtime function
void readFileData(FILE* inputFile, struct programInformation* program) {
    char inputChar;

    while ((inputChar = fgetc(inputFile)) != EOF) {
        program->programData = realloc(program->programData, program->programSize);
        *(program->programData + (program->programSize++) - 1) = inputChar;
    }

    (program->programSize)--;
}

// runtime function
void brainfuckInterpreter(struct programInformation program) {
    int programCounter = 0;
    int memoryPointer = 0;
    int toSkipBrackets = 0;
    char userInput;

    while (programCounter < program.programSize) {
        switch(*(program.programData + programCounter)) {
            case '.':
                printf("%c", program.memory[memoryPointer]);
                break;

            case ',':
                scanf("%c", &userInput);
                program.memory[memoryPointer] = userInput;
                break;

            case '+':
                program.memory[memoryPointer]++;
                break;

            case '-':
                program.memory[memoryPointer]--;
                break;

            case '>':
                memoryPointer++;
                break;

            case '<':
                memoryPointer--;
                break;

            case '[':
                if (program.memory[memoryPointer] == 0) {
                    do {
                        while (programCounter < program.programSize) {
                            if (*(program.programData + (++programCounter)) == ']') {
                                if (toSkipBrackets == 0) {
                                    break;
                                } toSkipBrackets--;
                            } else if (*(program.programData + programCounter) == '[') {
                                toSkipBrackets++;
                            }
                        }
                    } while (toSkipBrackets != 0);
                }

                break;
            
            case ']':
                if (program.memory[memoryPointer] != 0) {
                    do {
                        while (programCounter > 0) {
                            if (*(program.programData + programCounter--) == '[') {
                                if (toSkipBrackets == 0) {
                                    break;
                                } toSkipBrackets--;
                            } else if (*(program.programData + programCounter) == ']') {
                                toSkipBrackets++;
                            }
                        }
                    } while (toSkipBrackets != 0);
                }

                break;
            
            default:
                printf("Invalid input data at character %i, character is %c\n", (programCounter + 1), *(program.programData + programCounter));
                programCounter = program.programSize;
                break;
        }

        programCounter++;
    }
}

// debug function
void displayProgramInformationData(struct programInformation program) {
    printf("Total bytes: %i\n", program.programSize);

    for (int i = 0; i < program.programSize; i++) {
        printf("%c", *(program.programData + i));
    }
}

#pragma endregion funcs