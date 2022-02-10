/** Riya Patel
CSC 345-01
Lab 3 Exercise 2
**/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double pi;
int pts_total = 0;
int pts_circle = 0;

void* runner (void* param){
    // generate points total within square
    // x in [-1, 1], y in [-1, 1]
    for (int i = 0; i< pts_total; ++i){
        double x = 2.0*(double) rand() / (double)RAND_MAX - 1.0;
        double y = 2.0*(double) rand() / (double)RAND_MAX - 1.0;

        if ((x*x + y*y) < 1.0){
            pts_circle+=1;
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

    pthread_t tid;
    pthread_create(&tid, 0 , runner, NULL);
    pthread_join(tid, NULL);

    double pi = 4.0 * (double)pts_circle / (double) pts_total;
    printf("PI = %f\n", pi);

    return 0;
}