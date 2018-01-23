#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
    // int i;
    // i = 10;
    // int forkval;
    // forkval=fork();
    // printf("forkval = %d\n", forkval);
    // if (forkval==0)
    // {
    //     printf("I am the child\n");
    //     i += 10;
    // }
    // else
    // {
    //     printf("I am the parent\n");
    //     i += 20;
    // }
    // //if (fork() == 0) i += 20;
    // printf("i= %d \n", i);
       pid_t pid;
     char *const parmList[] = {"-a", NULL};

     if ((pid = fork()) == -1)
        perror("fork() error");
     else if (pid == 0) {
        execvp("ls", parmList);
        printf("Return not expected. Must be an execvp() error.n");
     }

    }


