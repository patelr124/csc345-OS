/** Riya Patel
CSC 345-01
Lab 2 Exercise 3
**/

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main (int argc, char** argv){
    pid_t pid = fork();


    if(pid == 0){

    } else if (pid >0){
        sleep(30);
    }

    return 0;
}

// ran with ./lab03_ex3 &
// ps -l