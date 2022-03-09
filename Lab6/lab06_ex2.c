/** Riya Patel
CSC 345-01
Lab 6 Exercise 2
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <wait.h>

#define MAX_WAIT 2;

typedef struct _FARMER{
    pthread_t t;
    int isNorth;
    int idx;
    int waitfor;
} FARMER;

/* Bridge */
pthread_mutex_t c_mutex;
pthread_cond_t  c_condv;

int num_Nwaiting = 0; 
int num_Swaiting = 0; 
int crossing = 0; 
int prev = 0; // to indicate whose turn it was before

void enter_bridgeN(FARMER* f){
    num_Nwaiting++;	
	printf("North farmer %d is waiting to cross bridge.\n", f->idx+1);
	
	pthread_mutex_lock(&c_mutex);
	while (crossing == 1 && (prev == 0))
		pthread_cond_wait(&c_condv, &c_mutex);

	num_Nwaiting--;
	prev = 1;
	crossing = 1;
	printf("North farmer %d has entered the bridge.\n", f->idx+1);
}

void exit_bridgeN(FARMER* f)
{
	crossing = 0;
	printf("North farmer %d about to exit the bridge.\n", f->idx+1);

	// notify everyone waiting to pass the bridge (regardless of direction)
	pthread_mutex_unlock(&c_mutex);
	pthread_cond_broadcast(&c_condv);

	printf("North farmer %d has exited the bridge.\n", f->idx+1);
}

void enter_bridgeS(FARMER* f)
{
	num_Swaiting++;
	printf("South farmer %d is waiting to cross the bridge.\n", f->idx+1);

	pthread_mutex_lock(&c_mutex);
	while (crossing == 1 && prev == 1 )
		pthread_cond_wait(&c_condv, &c_mutex);

	num_Swaiting--;
	prev = 0;
	crossing = 1;
	printf("South farmer %d has entered the bridge.\n", f->idx+1);
}

void exit_bridgeS(FARMER* f)
{
	crossing = 0;
	printf("South farmer %d about to exit the bridge.\n", f->idx+1);

	// notify everyone waiting to pass the bridge (regardless of direction)
	pthread_mutex_unlock(&c_mutex);
	pthread_cond_broadcast(&c_condv);

	printf("South farmer %d has exited the bridge.\n", f->idx+1);
}

void* pass_bridge(void* param)
{
	FARMER* f = (FARMER*)param;

	/* Enter Section */
	if (f->isNorth) {
		enter_bridgeN(f);
	} else {
		enter_bridgeS(f);
	}

	/* Critical Section */
	if (f->isNorth == 1) { /* North Farmer */
		printf("ETA : North Farmer %d is passing bridge in %d seconds.\n", f->idx+1, f->waitfor);
	} else { /* South Farmer */
		printf("ETA : South Farmer %d is passing bridge in %d seconds.\n", f->idx+1, f->waitfor);
	}
	sleep(f->waitfor);

	/* Exit Section */	
	if (f->isNorth) {
		exit_bridgeN(f);
	} else {
		exit_bridgeS(f);
	}
	pthread_exit(0);
}

int main (int argc, char** argv)
{
	int nNorth = atoi(argv[1]);
	int nSouth = atoi(argv[2]);

	/* Initialize Mutex Lock & Conditional Variable */
	pthread_mutex_init(&c_mutex, NULL);

	/* Create an Array of North and South Farmers */
	FARMER* fNorth = (FARMER*)malloc(sizeof(FARMER) * nNorth);
	FARMER* fSouth = (FARMER*)malloc(sizeof(FARMER) * nSouth);

	/* Prepare Farmer Information */
	for (int i = 0; i < nNorth; ++i) {
		fNorth[i].isNorth = 1; /* all 1 because all North farmers */
		fNorth[i].idx = i;
		fNorth[i].waitfor = rand() % MAX_WAIT; /* generate random wait time */
	}
	for (int i = 0; i < nSouth; ++i) {
		fSouth[i].isNorth = 0; /* all 0 because all South farmers */
		fSouth[i].idx = i;
		fSouth[i].waitfor = rand() % MAX_WAIT; /* generate random wait time */
	}

	/* Create Threads */
	for (int i = 0; i < nNorth; ++i) {
		pthread_create(&(fNorth[i].t), NULL, pass_bridge, &(fNorth[i]));
	}
	for (int i = 0; i < nSouth; ++i) {
		pthread_create(&(fSouth[i].t), NULL, pass_bridge, &(fSouth[i]));
	}

	/* Wait for threads to complete AKA all farmers have crossed the bridge */
	for (int i = 0; i < nNorth; ++i) {
		pthread_join(fNorth[i].t, NULL);
	}
	for (int i = 0; i < nSouth; ++i) {
		pthread_join(fSouth[i].t, NULL);
	}

	/* free allocated memory & delete condition variable */
	free(fNorth);
	free(fSouth);
	pthread_cond_destroy(&c_condv);

    return 0;
} 