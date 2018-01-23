#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
    
    int forkval;
    int link[2];
    char inputChar[1000];
    char *parmList[] = {"-a", NULL};
    if (pipe(link)==-1)
    {
        printf("Pipe Failed" );
        return 1;
    }
    forkval=fork();

    if (forkval==0)
    {
        close(link[0]);
        dup2(link[1],1);
        execvp("ls", parmList);

    }
    else
    {   
        close(link[1]);
        read(link[0], inputChar ,100);
        close(link[0]);
        printf("Output:\n%s\n",inputChar);
    }
   

}


