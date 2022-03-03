/** Riya Patel
CSC 345-01
Lab 6 Exercise 1
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

#define MAX_WAIT 3;

sem_t sem;
void enter_bridge(){
    sem_wait(&sem);
}

void exit_bridge(){
    sem_post(&sem);
}

void* pass_bridge(void* param){
    FARMER* f = (FARMER*) param;

    enter_bridge(); // on the bridge
    if(f->isNorth == 1){
            printf("*** NORTH FARMER #%d ON THE BRIDGE FOR %d SECS***\n", f->idx, f->waitfor);
    } else{
            printf("*** SOUTH FARMER #%d ON THE BRIDGE FOR %d SECS***\n", f->idx, f->waitfor);

    }

    sleep(f->waitfor);

    exit_bridge();
    pthread_exit(0);
}

int main (int argc, char** argv){
    if (argc != 3){
        printf("./lab06ex1 nNorth nSouth \n");
    }

    int nNorth = atoi(argv[1]);
    int nSouth = atoi(argv[2]);

    sem_init(&sem, 0, 1);

    FARMER* fN = (FARMER*) malloc(sizeof(FARMER)*nNorth);
    FARMER* fS = (FARMER*) malloc(sizeof(FARMER)*nSouth);

    for(int i =0; i < nNorth; i++){
        fN[i].isNorth = 1;
        fN[i].idx = i;
        fN[i].waitfor = rand() % MAX_WAIT;
        pthread_create(&(fN[i].t), NULL, pass_bridge, &(fN[i]));
    }

    for(int i =0; i < nSouth; i++){
        fS[i].isNorth = 0;
        fS[i].idx = i;
        fS[i].waitfor = rand() % MAX_WAIT;
        pthread_create(&(fS[i].t), NULL, pass_bridge, &(fS[i]));
    }

    for(int i =0; i < nNorth; i++){
        pthread_join(fN[i].t, NULL);
    }
    for(int i =0; i < nSouth; i++){
        pthread_join(fS[i].t, NULL);
    }
    

    free(fN);
    free(fS);
    sem_destroy(&sem);

    return 0;
}