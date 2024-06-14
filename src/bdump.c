#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int64_t getFileSize(char file[]);
unsigned char * readBinaryFile(char file[], int amount);

int main(int argc, char *argv[]) {
    size_t maxAlloc = 10000;
    size_t fsize = getFileSize(argv[1]);
    if (fsize < 0) { // Check if file could be opened
        printf("%s: file not found!", argv[1]);
        return -1;
    } 
    if (fsize <= maxAlloc) { // Check if we are on the last block
        unsigned char * data = readBinaryFile(argv[1], fsize);
        for (uint64_t i = 0; i < fsize; i++) {
            printf("%02X ", data[i]);
        }
    } else { // Print content in 10KiB blocks
        uint64_t blocks = fsize / maxAlloc;
        if (fsize % maxAlloc != 0) {
            blocks++;
        }
        for (uint64_t i = 0; i < blocks; i++) {
            unsigned char * data;
            if (i == blocks - 1) {
                int bytesLeft = fsize - (i * maxAlloc);
                data = readBinaryFile(argv[1], bytesLeft);
                for (uint64_t i = 0; i < bytesLeft; i++) {
                    printf("%02X ", data[i]);
                }
                break;
            }
            data = readBinaryFile(argv[1], maxAlloc);
            for (uint64_t i = 0; i < maxAlloc; i++) {
                printf("%02X ", data[i]);
            }
        }
    }
    printf("\n");
    return 0;
} 

int64_t getFileSize(char file[]) { // Get the size of a given file and return it, if file can't be opened, return -1
    FILE *fptr = fopen(file, "rb");
    if (fptr == NULL) {
        return -1;
    }
    fseek(fptr, 0L, SEEK_END);
    size_t size = ftell(fptr);
    fclose(fptr);
    return size;
}

unsigned char * readBinaryFile(char file[], int amount) { // Read a given amount of data from a binary file and return a pointer to the array where the data is stored
    unsigned char * dptr = calloc(amount, 1);
    FILE *fptr = fopen(file, "rb");
    uint64_t dataRead = fread(dptr, 1, amount, fptr);
    fclose(fptr);
    return dptr;
}