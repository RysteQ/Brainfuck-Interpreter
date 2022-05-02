#include <stdio.h>
#include <stdlib.h>


#define MEMORY_SIZE 32768


#pragma region declareFuncs

char *readFile(const char *filename);
void incrementMemoryCell(char *memory, int offset);
void decrementMemoryCell(char *memory, int offset);
void nextCell(int *offset);
void previousCell(int *offset);
void printChar(char *memory, int offset);
void getChar(char *memory, int offset);
void startLoop(char *memory, int offset, char *program, int *programCounter);
void endLoop(char *memory, int offset, char *program, int *programCounter);

#pragma endregion declareFuncs


int main(int argc, char** argv) {
        if (argc == 0) {
                printf("No input file\n");
                return -1;
        }

        char *memory = calloc(MEMORY_SIZE, sizeof(char));
        char *program = readFile(argv[1]);
        int programCounter = 0;
        int offset = 0;

        while (*(program + programCounter) != '\0') {
                char command = *(program + programCounter);

                switch (command) {
                        case '+':
                                incrementMemoryCell(memory, offset);
                                break;

                        case '-':
                                decrementMemoryCell(memory, offset);
                                break;

                        case '>':
                                nextCell(&offset);
                                break;

                        case '<':
                                previousCell(&offset);
                                break;

                        case '.':
                                printChar(memory, offset);
                                break;

                        case ',':
                                getChar(memory, offset);
                                break;

                        case '[':
                                startLoop(memory, offset, program, &programCounter);
                                break;

                        case ']':
                                endLoop(memory, offset, program, &programCounter);

                                if (programCounter < 0)
                                        return -2;

                                break;

                        default:
                                break;
                }

                programCounter++;
        }

        return 0;
}


#pragma region functions

char *readFile(const char *filename) {
        FILE *inputFile;
        char *toReturn;
        int  offset;
        char inputChar;

        inputFile = fopen(filename, "r");
        toReturn = malloc(sizeof(char) * 1);
        offset = 0;

        if (inputFile == NULL) {
                printf("File does not exist\n");
                exit(-3);
        }

        do {
                inputChar = fgetc(inputFile);
                *(toReturn + offset) = inputChar;
                toReturn = realloc(toReturn, (offset + 2) * sizeof(char));
                offset++;
        } while (inputChar != 255);

        *(toReturn + offset) = '\0';
        fclose(inputFile);

        return toReturn;
}

void incrementMemoryCell(char *memory, int offset) {
        (*(memory + offset))++;
}

void decrementMemoryCell(char *memory, int offset) {
        (*(memory + offset))--;
}

void nextCell(int *offset) {
        if (*offset != MEMORY_SIZE - 1)
                (*offset)++;
}

void previousCell(int *offset) {
        if (*offset != 0)
                (*offset)--;
}

void printChar(char *memory, int offset) {
        printf("%c", *(memory + offset));
}

void getChar(char *memory, int offset) {
        scanf("%c", (memory + offset));
}

void startLoop(char *memory, int offset, char *program, int *programCounter) {
        if (*(memory + offset) != 0)
                return;

        char currentInstruction = *(program + *programCounter);
        int toSkip = 0;

        while (currentInstruction != '\0' && currentInstruction != ']' && toSkip != 0) {
                if (currentInstruction == '[')
                        toSkip--;
                else if (currentInstruction == ']')
                        toSkip++;

                (*programCounter)++;
                currentInstruction = *(program + *programCounter);
        }
}

void endLoop(char *memory, int offset, char *program, int *programCounter) {
        if (*(memory + offset) == 0)
                return;

        char currentInstruction = *(program + *programCounter);
        int toSkip = 1;

        while (*programCounter != -1 && currentInstruction != '[' && toSkip != 0) {
                if (currentInstruction == ']')
                        toSkip--;
                else if (currentInstruction == '[')
                        toSkip++;

                (*programCounter)--;
                currentInstruction = *(program + *programCounter);
        }
}

#pragma endregion functions
