/** Riya Patel
CSC 345-01
Lab 2 Exercise 4
**/

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/mman.h>

int main(int argc, char** argv){

    int n = atoi(argv[1]);
    pid_t id = fork();

    const char* name = "OS";
    const int SIZE = 4096;
    int shm.fd;
    void *ptr;

    shm.fd = shm.open(name, O.RDONLY, 0666);
    ftruncate (shm.fd, SIZE);
    ptr = mmap(0, SIZE, PROT.READ, MAP.SHARED, shm.rd, 0);


    if (id ==0){

    }  else {

    }
    return 0;
}