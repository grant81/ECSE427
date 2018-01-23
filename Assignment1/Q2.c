#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
    
    int forkval;
    int link[2];
    //create an a array for the pipe
    char inputChar[1000];
    //declear a buffer to receive the data from the pipe
    char *parmList[] = {"-a", NULL};
    //parameters for the execvp command
    if (pipe(link)==-1)
    //create the pipe
    {
        printf("Pipe Failed" );
        return 1;
    }
    forkval=fork();

    if (forkval==0)
    {   //child process
        close(link[0]);
        //redirect the stdout to the writing end of the pipe
        dup2(link[1],1);
        //run the command
        execvp("ls", parmList);

    }
    else
    {   
        //parent process
        close(link[1]);
        //read from the reading end of the pipe
        read(link[0], inputChar ,100);
        close(link[0]);
        //display the output
        printf("Output:\n%s\n",inputChar);
    }
   

}


