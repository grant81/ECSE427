#include <stdio.h>  //for printf and scanf
#include <stdlib.h> //for malloc

#define LOW 0
#define HIGH 199
#define START 120

//compare function for qsort
//you might have to sort the request array
//use the qsort function
// an argument to qsort function is a function that compares 2 quantities
//use this there.
int cmpfunc(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

//function to swap 2 integers
void swap(int *a, int *b)
{
    if (*a != *b)
    {
        *a = (*a ^ *b);
        *b = (*a ^ *b);
        *a = (*a ^ *b);
        return;
    }
}

//Prints the sequence and the performance metric
void printSeqNPerformance(int *request, int numRequest)
{
    int i, last, acc = 0;
    last = START;
    printf("\n");
    printf("%d", START);
    for (i = 0; i < numRequest; i++)
    {
        printf(" -> %d", request[i]);
        acc += abs(last - request[i]);
        last = request[i];
    }
    printf("\nPerformance : %d\n", acc);
    return;
}

//access the disk location in FCFS
void accessFCFS(int *request, int numRequest)
{
    //simplest part of assignment
    printf("\n----------------\n");
    printf("FCFS :");
    printSeqNPerformance(request, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in SSTF
void accessSSTF(int *request, int numRequest)
{
    //write your logic here
    printf("\n----------------\n");
    printf("SSTF :");
    qsort(request, numRequest, sizeof(int), cmpfunc);
    int currLocation = START;

    for (int i = 0; i < numRequest; i++)
    {
        int min = 200;
        int minPosition = i;
        for (int j = i; j < numRequest; j++)
        {
            if (abs(request[j] - currLocation) < min)
            {
                min = abs(request[j] - currLocation);
                minPosition = j;
            }
        }
        currLocation = request[minPosition];
        swap(&request[minPosition], &request[i]);
    }
    printSeqNPerformance(request, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in SCAN
void accessSCAN(int *request, int numRequest)
{
    qsort(request, numRequest, sizeof(int), cmpfunc);
    //write your logic here
    printf("\n----------------\n");
    printf("SCAN :");
    //hitting the boundaries

    int curr = START;
    int firstLocation = -1;
    int newCnt = numRequest + 1;
    int *newRequest = malloc(newCnt * sizeof(int));
    int specialCase = 0;
    if (abs(curr - LOW) > abs(curr - HIGH))
    {

        for (int i = 0; i < numRequest; i++)
        {
            if (request[i] > curr)
            {
                firstLocation = i;
                break;
            }
            if (i == numRequest - 1)
            {
                specialCase = 1;
            }
        }
        if (firstLocation > 0)
        {
            int nextLoc = 0;
            for (int i = firstLocation; i < numRequest; i++)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
            newRequest[nextLoc] = HIGH;
            nextLoc++;
            for (int i = firstLocation - 1; i >= 0; i--)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
        }
        else if (specialCase == 1)
        {

            newCnt = numRequest;
            newRequest = realloc(newRequest, numRequest);
            int counter = numRequest;
            for (int i = 0; i < numRequest; i++)
            {
                counter--;
                newRequest[counter] = request[i];
            }
        }
        //everything is larger than currerent location
        else
        {
            newCnt = numRequest;
            newRequest = realloc(newRequest, numRequest);
            for (int i = 0; i < numRequest; i++)
            {
                newRequest[i] = request[i];
            }
        }
    }
    else
    {
        for (int i = 0; i < numRequest; i++)
        {
            if (request[i] > curr)
            {
                firstLocation = i - 1;
                break;
            }
            if (i == numRequest - 1)
            {
                specialCase = 1;
            }
        }
        if (firstLocation >= 0)
        {
            int nextLoc = 0;
            for (int i = firstLocation; i >= 0; i--)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
            newRequest[nextLoc] = LOW;
            nextLoc++;
            for (int i = firstLocation + 1; i < numRequest; i++)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
        }
        else if (specialCase == 1)
        {
            newCnt = numRequest;
            newRequest = realloc(newRequest, numRequest);
            int counter = numRequest;
            for (int i = 0; i < numRequest; i++)
            {
                counter--;
                newRequest[counter] = request[i];
            }
        }
        else
        {
            newCnt = numRequest;
            newRequest = realloc(newRequest, numRequest);
            for (int i = 0; i < numRequest; i++)
            {
                newRequest[i] = request[i];
            }
        }
    }
    printSeqNPerformance(newRequest, newCnt);
    printf("----------------\n");
    return;
}

//access the disk location in CSCAN
void accessCSCAN(int *request, int numRequest)
{
    //write your logic here
    printf("\n----------------\n");
    printf("CSCAN :");
    int curr = START;
    int firstLocation = -1;
    int newCnt = numRequest + 2;
    int *newRequest = malloc(newCnt * sizeof(int));
    if (abs(curr - LOW) > abs(curr - HIGH))
    {

        for (int i = numRequest - 1; i >= 0; i--)
        {
            if (request[i] < curr)
            {
                firstLocation = i + 1;
                break;
            }
        }
        if (firstLocation >= 0)
        {
            int nextLoc = 0;
            for (int i = firstLocation; i < numRequest; i++)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
            newRequest[nextLoc] = HIGH;
            nextLoc++;
            newRequest[nextLoc] = LOW;
            nextLoc++;
            for (int i = 0; i < firstLocation; i++)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
        }
        //everything is larger than currerent location
        else
        {
            newCnt = numRequest;
            newRequest = realloc(newRequest, numRequest);
            for (int i = 0; i < numRequest; i++)
            {
                newRequest[i] = request[i];
            }
        }
    }
    else
    {
        for (int i = 0; i < numRequest; i++)
        {
            if (request[i] > curr)
            {
                firstLocation = i - 1;
                break;
            }
        }
        if (firstLocation >= 0)
        {
            int nextLoc = 0;
            for (int i = firstLocation; i >= 0; i--)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
            newRequest[nextLoc] = LOW;
            nextLoc++;
            newRequest[nextLoc] = HIGH;
            nextLoc++;
            for (int i = numRequest - 1; i > firstLocation; i--)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
        }
        else
        {
            newCnt = numRequest;
            newRequest = realloc(newRequest, numRequest);
            int counter = numRequest;
            for (int i = 0; i < numRequest; i++)
            {
                counter--;
                newRequest[counter] = request[i];
            }
        }
    }
    printSeqNPerformance(newRequest, newCnt);
    printf("----------------\n");
    return;
}

//access the disk location in LOOK
void accessLOOK(int *request, int numRequest)
{
    //write your logic here
    qsort(request, numRequest, sizeof(int), cmpfunc);
    //write your logic here
    printf("\n----------------\n");
    printf("LOOK :");
    //hitting the boundaries

    int curr = START;
    int firstLocation = -1;
    int newCnt = numRequest;
    int *newRequest = malloc(newCnt * sizeof(int));
    if (abs(curr - LOW) > abs(curr - HIGH))
    {

        for (int i = numRequest - 1; i >= 0; i--)
        {
            if (request[i] < curr)
            {
                firstLocation = i + 1;
                break;
            }
        }
        if (firstLocation >= 0)
        {
            int nextLoc = 0;
            for (int i = firstLocation; i < numRequest; i++)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
            for (int i = firstLocation - 1; i >= 0; i--)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
        }
        //everything is larger than currerent location
        else
        {

            for (int i = 0; i < numRequest; i++)
            {
                newRequest[i] = request[i];
            }
        }
    }
    else
    {
        for (int i = 0; i < numRequest; i++)
        {
            if (request[i] > curr)
            {
                firstLocation = i - 1;
                break;
            }
        }
        if (firstLocation >= 0)
        {
            int nextLoc = 0;
            for (int i = firstLocation; i >= 0; i--)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
            for (int i = firstLocation + 1; i < numRequest; i++)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
        }
        else
        {
            int counter = numRequest;
            for (int i = 0; i < numRequest; i++)
            {
                counter--;
                newRequest[counter] = request[i];
            }
        }
    }

    printSeqNPerformance(newRequest, newCnt);
    printf("----------------\n");
    return;
}

//access the disk location in CLOOK
void accessCLOOK(int *request, int numRequest)
{
    //write your logic here
    printf("\n----------------\n");
    printf("CLOOK :");
    int curr = START;
    int firstLocation = -1;
    int newCnt = numRequest + 1;
    int *newRequest = malloc(newCnt * sizeof(int));
    if (abs(curr - LOW) > abs(curr - HIGH))
    {

        for (int i = numRequest - 1; i >= 0; i--)
        {
            if (request[i] < curr)
            {
                firstLocation = i + 1;
                break;
            }
        }
        if (firstLocation >= 0)
        {
            int nextLoc = 0;
            for (int i = firstLocation; i < numRequest; i++)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
            newRequest[nextLoc] = LOW;
            nextLoc++;
            for (int i = 0; i < firstLocation; i++)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
        }
        //everything is larger than currerent location
        else
        {
            newCnt = numRequest;
            newRequest = realloc(newRequest, numRequest);
            for (int i = 0; i < numRequest; i++)
            {
                newRequest[i] = request[i];
            }
        }
    }
    else
    {
        for (int i = 0; i < numRequest; i++)
        {
            if (request[i] > curr)
            {
                firstLocation = i - 1;
                break;
            }
        }
        if (firstLocation >= 0)
        {
            int nextLoc = 0;
            for (int i = firstLocation; i >= 0; i--)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
            newRequest[nextLoc] = HIGH;
            nextLoc++;
            for (int i = numRequest - 1; i > firstLocation; i--)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
        }
        else
        {
            newCnt = numRequest;
            newRequest = realloc(newRequest, numRequest);
            int counter = numRequest;
            for (int i = 0; i < numRequest; i++)
            {
                counter--;
                newRequest[counter] = request[i];
            }
        }
    }
    printSeqNPerformance(newRequest, newCnt);
    printf("----------------\n");
    return;
}

int main()
{
    int *request, numRequest, i, ans;

    //allocate memory to store requests
    printf("Enter the number of disk access requests : ");
    scanf("%d", &numRequest);
    request = malloc(numRequest * sizeof(int));

    printf("Enter the requests ranging between %d and %d\n", LOW, HIGH);
    for (i = 0; i < numRequest; i++)
    {
        scanf("%d", &request[i]);
    }

    printf("\nSelect the policy : \n");
    printf("----------------\n");
    printf("1\t FCFS\n");
    printf("2\t SSTF\n");
    printf("3\t SCAN\n");
    printf("4\t CSCAN\n");
    printf("5\t LOOK\n");
    printf("6\t CLOOK\n");
    printf("----------------\n");
    scanf("%d", &ans);

    switch (ans)
    {
    //access the disk location in FCFS
    case 1:
        accessFCFS(request, numRequest);
        break;

    //access the disk location in SSTF
    case 2:
        accessSSTF(request, numRequest);
        break;

        //access the disk location in SCAN
    case 3:
        accessSCAN(request, numRequest);
        break;

        //access the disk location in CSCAN
    case 4:
        accessCSCAN(request, numRequest);
        break;

    //access the disk location in LOOK
    case 5:
        accessLOOK(request, numRequest);
        break;

    //access the disk location in CLOOK
    case 6:
        accessCLOOK(request, numRequest);
        break;
    case 7:
        accessFCFS(request, numRequest);
        accessSSTF(request, numRequest);
        accessSCAN(request, numRequest);
        accessCSCAN(request, numRequest);
        accessLOOK(request, numRequest);
        accessCLOOK(request, numRequest);
        break;

    default:
        break;
    }
    return 0;
}