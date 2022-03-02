/** Riya Patel
CSC 345-01
Lab 5 Exercise 1
**/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 10

int buf[BUF_SIZE];

pthread_mutex_t lock;

void *prod(void *param){
    /* pthread_mutex_lock(&lock); */
    for (int i = 0; i<BUF_SIZE; ++i){
        buf[i] = i;
    }
    /* pthread_mutex_unlock(&lock); */ /* 1 */

    pthread_exit(0);
}

void *cons(void *param){
    /* pthread_mutex_lock(&lock); */ /* 1 */
    for (int i = 0; i < BUF_SIZE; ++i){
       printf("Buffer index %d = %d\n", i, buf[i]); 
    }
    /* pthread_mutex_unlock(&lock); */ /* 1 */

    pthread_exit(0);
}

int main(int argc, char** argv){
    pthread_t t_prod, t_cons;
     /* pthread_mutex_init(&lock, NULL); */ /* 1 */

     pthread_create(&t_prod, 0, prod, NULL);
     /* sleep(0.5); */
     pthread_create(&t_cons, 0, cons, NULL);

     pthread_join(t_prod, NULL);
     pthread_join(t_cons, NULL);
     /* pthread_mutex_destroy(&lock); */ /* 1 */

     return 0;
}