/** Riya Patel
CSC 345-01
Lab 5 Exercise 3
**/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>


pthread_t tid[4];

double pi;
int pts_total = 0;
int pts_circle = 0;

pthread_mutex_t(lock);

void* worker (void* param){
    // generate points total within square
    // x in [-1, 1], y in [-1, 1]
    int n_to_draw = *((int*) param);
    for (int i = 0; i< n_to_draw; ++i){
        double x = 2.0*(double) rand() / (double)RAND_MAX - 1.0;
        double y = 2.0*(double) rand() / (double)RAND_MAX - 1.0;

        if ((x*x + y*y) < 1.0){
            pts_circle+=1;
            pthread_mutex_lock(&lock);
            pthread_mutex_unlock(&lock);
        }
    }
    pthread_exit(0);
}

int main (int argc, char** argv){

    if(argc < 2){
        printf("Give arguments! \n");
        exit(0);
    }

    pts_total = atoi(argv[1]);
    int pts_todraw = pts_total/4;;

    pthread_t tid[4];
    pthread_create(&tid[0], NULL , worker, &pts_todraw);
    pthread_create(&tid[1], NULL , worker, &pts_todraw);
    pthread_create(&tid[2], NULL , worker, &pts_todraw);
    int pts_todraw_last = pts_todraw * pts_total % 4;
    pthread_create(&tid[3], NULL , worker, &pts_todraw_last);

    
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);

    double pi = 4.0 * (double)pts_circle / (double) pts_total;
    printf("PI = %f\n", pi);

    return 0;
}