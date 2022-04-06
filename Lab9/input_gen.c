/** Riya Patel
CSC 345-01
Lab 09 Exercise 1
**/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PAGE 10

int main(int argc, char** argv){
    srand(time(NULL));
    int numRefs = atoi(argv[1]);
    FILE* fp = fopen("input.txt","wt");
    fprintf(fp, "%d\n", numRefs);

    for(int i = 0; i< numRefs; ++i){
        fprintf(fp, "%d ", rand()%MAX_PAGE);
    }

    fprintf(fp, "\n");
    fclose(fp);
    return 0;


}