/** Riya Patel
CSC 345-01
Project 1
**/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#define MAX_LINE 80

void deleteEnd (char* myStr){  /* method to remove most recent directory from char *cwd when using cd */
    char *del = &myStr[strlen(myStr)];
    if (strstr(myStr, "/")==0){
        while (del > myStr && *del != ':') /* when returning to parnet directory */
                del--;
        if (*del== ':')
            *del= '\0';
    } else {
        while (del > myStr && *del != '/') /* when going up one child directory */
                        del--;
                if (*del== '/')
                    *del= '\0';
    }
    return;
}

int main(void){
    char *args[MAX_LINE/2+1];
    int should_run = 1;
    pid_t id;
    char* buffer;
    char cwd_buff[MAX_LINE];
    strcat(cwd_buff,"osh>"); /* OG directory name */
    int args_size = 0, saved=0, same_time = 0; /* size & flag ints */
 

    buffer = (char*) malloc((MAX_LINE+1)*sizeof(char));

    while(should_run){
        printf("%s", cwd_buff); /* print cwd */
        fflush(stdout);

        char* input;
        input = (char*) malloc((MAX_LINE+1)*sizeof(char));

        size_t buffer_size = MAX_LINE;
        getline(&input, &buffer_size, stdin); /* &buffer_size = address of first character of input string */

        if (strcmp(input, "!!\n")==0){ /* if input = history command check history records */
            if (saved == 0){ /* saved = buffer flag aka buffer empty */
                printf("No commands to recall.\n");
            } else{
                strcpy(input, buffer); /* set input as buffer (past command) */
                printf("%s\n",input);
                fflush(stdout);
            }
        } else {
            strcpy(buffer, input);/* save current command as buffer */
            saved = 1;/* update flag */
        }
        
        char delimiters[] = " \t\n\a\r";
        char* pointer = strtok(input, delimiters);

        while(pointer==NULL){ /* while no input / only delimiters */
            printf("%s>",cwd_buff);
            fflush(stdout);
            getline(&input, &buffer_size, stdin); 

            if (strcmp(input, "!!")==0){ /* if input = history command check history records */
                if (saved == 0){ /* saved = buffer flag aka buffer empty */
                    printf("No commands to recall.\n");
                } else{
                    strcpy(input, buffer); /* set input as buffer (past command) */
                    printf("%s\n",input);
                    fflush(stdout);
                }
            } else {
                strcpy(buffer, input);/* save current command as buffer */
                saved = 1;/* update flag */
            }
            pointer = strtok(input, delimiters);
        }

        int i = 0;
        while(pointer != NULL){ /* set i = number of arguments */
            args[i]=pointer;
            pointer = strtok(NULL, delimiters);
            i=i+1;
        }

        if(strcmp(args[i-1],"&")==0){ /* concurrence flag set */
            same_time = 0;
            i = i-1;
        } else same_time = 1;
        args[i] =NULL;
        args_size = i;

        if(strcmp(args[0], "exit") ==0){ /* exit command */
            should_run = 0;
            return 0;
        } else if (strcmp(args[0],"cd") == 0) { /* manual cd */
            if(chdir(args[1])==0){
                if(strcmp(args[1],".")==0){ /* go up one directory */
                    chdir(".");
                    deleteEnd(cwd_buff); /* remove tail end */
                }
                 else { /* enter subdirectory */
                cwd_buff[strlen(cwd_buff)-1] = '\0'; /* format string properly */
                    if(strlen(cwd_buff)==4){
                        strcat(cwd_buff,":");
                    } else strcat(cwd_buff,"/");
                    strcat(cwd_buff, args[1]); 
                }
                strcat(cwd_buff, ">");
            } else {
                printf("Incorrect cd command syntax./n");
            }
            continue;
        }

        pid_t pid = fork();

        if (pid<0){ /* if fork fails, throw error */
            fprintf(stderr, "Fork Failed");
            return 1;
        }
        else if (pid == 0){ /* child created */
            fflush(stdout);
            int j = 0;
            while(j<args_size){ /* while there are arguments */
                if(strcmp(args[j], "<")==0){ 
                    fflush(stdout);
                    if ( (j==(args_size-1)) | (strcmp(args[j+1],">")==0) | ((strcmp(args[i-1],"<")==0))){ /* no file provided or <> next to each other */
                        printf("Incorrect Error\n");
                        fflush(stdout);
                        args[0]=NULL;    
                    } else { /* otherwise good syntax, import from file */
                        int fd = open(args[j+1], O_RDONLY); /* open file in read mode */
                        dup2(fd, STDIN_FILENO); /* duplicate to file input */
                        close(fd); /* close file */
                        args[j] = NULL; /* delete next arguments so doenst misread redirection <>'s as command*/
                        args[j+1] = NULL;
                    }
                    fflush(stdout);
                    j=args_size + 2; /* exit loop after command is completed */
                } else if (strcmp(args[j], ">")==0){
                    fflush(stdout);
                    if ( (j==(args_size-1)) | (strcmp(args[j+1],"<")==0) | ((strcmp(args[i-1],">")==0))){ /* no file provided or <> next to each other */
                        printf("Incorrect Error\n");
                        fflush(stdout);
                        args[0]=NULL;    
                    } else { /* otherwise good syntax, export command to file */
                        int fd = open(args[j+1], O_CREAT | O_WRONLY, 0644); /* open file in write/create if DNE mode */
                        dup2(fd, STDOUT_FILENO); /* duplicate to file input */
                        close(fd); /* close file */
                        args[j] = NULL; /* delete next arguments so doesn't misread redirection <>'s as command*/
                        args[j+1] = NULL;
                    }
                    j=args_size + 2; /* exit loop after command is completed */
                } else if (strcmp(args[j], "|") ==0){
                    args[j] = NULL;
                    int fd[2];
                    if(pipe(fd) == -1){
                        fprintf(stderr, "Pipe failed. Please try again.");
                    }
                    pid_t pid1 = fork(); /* create child process */
                    char* args2[MAX_LINE/2+1];
                    for (int i = 0; i < j; i++){
                        args2[i] = args[i];  /* copying arguments to pass into children */
                    }
                    args2[j]=NULL; /* clear arg to prevent misread */
                    for (int x = j+1; x<= args_size; x++){ /* shift args over */
                        args[x-j-1] = args[x];
                    }

                    if (pid1<0){
                        fprintf(stderr, "Fork Failed");
                        return 1;
                    } else if (pid1 == 0){
                        close(fd[0]);
                        dup2(fd[1], STDOUT_FILENO); /* check write end of pipeline */
                        close(fd[1]);
                        if(execvp(args2[0], args2)==-1){
                            printf("Instruction execution error! \n");
                            return 1;
                        }
                    } else if (pid1 > 1){
                        wait(NULL);
                        dup2(fd[0],STDIN_FILENO);
                        close(fd[1]);
                        args[j] = NULL;
                        execvp(args[j + 1], &args[j + 1]);
                    }
                    else{
                        waitpid(pid1, NULL, 0); /* wait */
                        close(fd[0]);
                        dup2(fd[0], STDOUT_FILENO); /* check read end of pipeline */
                        close(fd[0]);
                    }
                    j=args_size + 2; /* exit loop after command is completed */
                } 
                j=j+1;
            }
            if(execvp(args[0], args)==-1){ /* execute child */
                printf("Instruction execution error! \n");
                return 1;
            }
        }
        else if (same_time == 1){  /* check concurrence flag, wait for child if true */
            waitpid(pid, NULL, 0);
        }
    }
    return 0;
}

