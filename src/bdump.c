#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t getFileSize(char file[]);              // Declare Functions and globals

unsigned char * readBinaryFile(char file[]);
size_t size;

int main(int argc, char *argv[]) {              // Do the silly stuff that makes the program do the thing
    unsigned char * data = readBinaryFile(argv[1]);
    for (uint64_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
    return 0;
}

uint64_t getFileSize(char file[]) {             // Get the size of a file and return it
    FILE *fptr = fopen(file, "rb");
    if (fptr == NULL) {
        return -1;
    }
    fseek(fptr, 0L, SEEK_END);
    size_t size = ftell(fptr);
    fclose(fptr);
    return size;
}

unsigned char * readBinaryFile(char file[]) {   // Read the file and store it's contents in an array in memory
    size = getFileSize(file);
    unsigned char *dptr = calloc(size, 1);
    FILE *fptr = fopen(file, "rb");
    uint64_t dataRead = fread(dptr, 1, size, fptr);
    return dptr;
}