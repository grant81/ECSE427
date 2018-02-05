/*
----------------- COMP 310/ECSE 427 Winter 2018 -----------------
I declare that the awesomeness below is a genuine piece of work
and falls under the McGill code of conduct, to the best of my knowledge.
-----------------------------------------------------------------
*/

//Please enter your name and McGill ID below
//Name: <Yingnan Zhao>
//McGill ID: <260563769>
#include<stdio.h>
#include<fcntl.h>
#include <sys/stat.h>
#include<unistd.h>
int main(){

    printf("First : Print to stdout\n");
    int fd, bak;//create two varible to hold the file descriptors
    bak = dup(1);//make a copy of the current file descriptor
    close(1);//close stdout at the default location
    fd = open("redirect_out.txt",O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    //get the file descripter that write to a file 
    if(fd <0){//handle error
        return -1;
    }
    dup(fd);//copy the file writing descripter to the descriptor we just closed(lowest numbered) 
    printf("Second : Print to redirect_out.txt\n");
    close(1);//close the redirected output descriptor
    dup(bak);//copy the default stdout back
    printf("Third : Print to stdout\n");
    return 0;
}
