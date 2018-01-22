#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
    int i;
    i = 10;
    int forkval;
    forkval=fork();
    printf("forkval = %d\n", forkval);
    if (forkval==0)
    {
        printf("I am the child\n");
        i += 10;
    }
    else
    {
        printf("I am the parent\n");
        i += 20;
    }
    //if (fork() == 0) i += 20;
    printf("i= %d \n", i);
    }
// struct person{
//     char name[15];
//     int age;
// };

// int main()
// {
//     struct person p1;
//     scanf("%s",p1.name);
//     scanf("%d",&p1.age);
    
//     printf("name: %s, age: %d",p1.name,p1.age);
//     if (p1.age<18){
//         printf("too youg to watch (%d)",p1.age);
//     }
//     else{
//         printf("egjoy the porn");
//     }
//     for (int i =0; i<100; i++){
//         printf("sexy\n");
//     }
//     return 0;
// }