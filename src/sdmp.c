#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include <unistd.h>
//#include <string.h>

int64_t getFileSize(char file[]);
unsigned char * readBinaryFile(char file[], int amount);

int main(int argc, char *argv[]) {
    if (argc > 2) {
        printf("too many arguments!\n");
        return -1;
    } else if (argc < 2) {
        printf("please provide a file!\n");
        return -1;
    }
    int64_t maxAlloc = 10000;
    int64_t fsize = getFileSize(argv[1]);
    if (fsize == 0) { // Check if file could be opened
        printf("%s: file not found!\n", argv[1]);
        return -1;
    } 
    if (fsize <= maxAlloc) { // Check if file size is smaller or equal to the size of 1 block
        unsigned char * data = readBinaryFile(argv[1], fsize);
        for (int64_t i = 0; i < fsize; i++) {
            printf("%02X ", data[i]);
        }
        free(data);
    } else { // Print content in 10KiB blocks
        int64_t blocks = fsize / maxAlloc;
        if (fsize % maxAlloc != 0) {
            blocks++;
        }
        for (int64_t i = 0; i < blocks; i++) { // Data grab and print loop
            unsigned char * data;
            if (i == blocks - 1) { // Check if we are on last block
                int bytesLeft = fsize - (i * maxAlloc);
                data = readBinaryFile(argv[1], bytesLeft);
                for (int64_t i = 0; i < bytesLeft; i++) {
                    printf("%02X ", data[i]);
                }
                free(data);
                break;
            }
            data = readBinaryFile(argv[1], maxAlloc);
            for (int64_t i = 0; i < maxAlloc; i++) {
                printf("%02X ", data[i]);
            }
            free(data);
        }
    }
    printf("\n");
    return 0;
} 

int64_t getFileSize(char file[]) { // Get the size of a given file and return it, if file can't be opened, return -1
    FILE *fptr = fopen(file, "rb");
    if (fptr == NULL) {
        return 0;
    }
    fseek(fptr, 0L, SEEK_END);
    size_t size = ftell(fptr);
    fclose(fptr);
    return size;
}

unsigned char * readBinaryFile(char file[], int amount) { // Read a given amount of data from a binary file and return a pointer to the array where the data is stored
    unsigned char * dptr = calloc(amount, 1);
    FILE *fptr = fopen(file, "rb");
    fread(dptr, 1, amount, fptr);
    fclose(fptr);
    return dptr;
}