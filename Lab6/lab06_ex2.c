/** Riya Patel
CSC 345-01
Lab 6 Exercise 2
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct _FARMER{
    pthread_t t;
    int isNorth;
    int idx;
    int waitfor;
} FARMER;