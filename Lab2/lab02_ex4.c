/** Riya Patel
CSC 345-01
Lab 2 Exercise 4
**/

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/mman.h>

int main(int argc, char** argv){

    int n = atoi(argv[1]);
    pid_t id = fork();

    const char* name = "OS";
    const int SIZE = 4096;
    int shm_fd;
    void *ptr;

    shm_fd = shm_open(name, SHM_RDONLY, 0666);
    ftruncate (shm_fd, SIZE);
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);


    if (id ==0){
        while ( n > 1 ){
            sprintf(ptr, "%d", n);
            ptr++;
            if( n % 2 == 0){
                n = n/2;
            } else {
                n = n*3 + 1;
            }
        }
        sprintf(ptr, "%d", n);
        ptr++;

    }  else {
        wait(NULL);
        printf("%s/n", (char *)ptr);
        shm_unlink(name);
    }
    return 0;
}