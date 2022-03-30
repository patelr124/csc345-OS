/** Riya Patel
CSC 345-01
Lab 8 Exercise 1
**/

#include <stdio.h>
#include <stdlib.h>
#define PAGE_SIZE 4096

int main (int argc, char** argv){
    unsigned int addr = (unsigned int)atoi(argv[1]);
    unsigned int page = addr / PAGE_SIZE;
    unsigned int offset = addr % PAGE_SIZE;

    printf("The address is %u is located at:\n", addr);
    printf("page number = %u\n", page);
    printf("offset = %u\n", offset);

    return 0;
}