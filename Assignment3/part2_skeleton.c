#include <stdio.h>  //for printf and scanf
#include <stdlib.h> //for malloc

#define LOW 0
#define HIGH 199
#define START 55

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
    //sort the array
    qsort(request, numRequest, sizeof(int), cmpfunc);
    int currLocation = START;
    //for find the closest request to the starting point
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
        //after the closest point has been find change the start point to that point
        //swap the closest point with the next request position
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
    //sort the request array
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
    //if the start point is closer to the HIGH position go to the HIGH direction first
    if (abs(curr - LOW) > abs(curr - HIGH))
    {
        //starting from the smallest request find the first request that is larger than
        //the current location
        for (int i = 0; i < numRequest; i++)
        {
            if (request[i] >= curr)
            {
                firstLocation = i;
                break;
            }
            //if all requests are smaller than the START location, handle this as the special
            //case, the first location will be -1
            //the other special case, if all requests are larger than Start, the first location will be 0
            if (i == numRequest - 1)
            {
                specialCase = 1;
            }
        }
        //if normal case
        if (firstLocation > 0)
        {
            //first handle all request to the HIGH direction in a ascending order
            int nextLoc = 0;
            for (int i = firstLocation; i < numRequest; i++)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
            //hit the HIGH
            newRequest[nextLoc] = HIGH;
            nextLoc++;
            //reverse the direction handle all requests to the LOW direction in a descending order
            for (int i = firstLocation - 1; i >= 0; i--)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
        }
        // if everything is smaller than the currerent location
        // access every request in a descenting order without hiting the boundaries
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
        // access every request in a ascenting order without hiting the boundaries
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
    //if the start point is closer to the LOW position go to the LOW direction first
    else
    {
        //starting from the smallest request find the first request that is larger than
        //the current location, set the first location as the previous one
        for (int i = 0; i < numRequest; i++)
        {
            if (request[i] > curr)
            {
                firstLocation = i - 1;
                break;
            }
            //handle the case where there is a request that is equal to the START
            else if (request[i] == curr)
            {
                firstLocation = i;
                break;
            }
            //if all requests are smaller than the START location, handle this as the special
            //case, the first location will be -1
            //the other special case, if all requests are larger than Start, the first location will be 0
            if (i == numRequest - 1)
            {
                specialCase = 1;
            }
        }
        //if normal case
        if (firstLocation >= 0)
        {
            //first handle all request to the LOW direction in a descending order
            int nextLoc = 0;
            for (int i = firstLocation; i >= 0; i--)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
            //hit the LOW
            newRequest[nextLoc] = LOW;
            nextLoc++;
            //reverse the direction handle all requests to the High direction in a ascending order
            for (int i = firstLocation + 1; i < numRequest; i++)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
        }
        // if everything is smaller than the currerent location
        // access every request in a descenting order without hiting the boundaries
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
        // access every request in a ascenting order without hiting the boundaries
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
    //most of the logic is the same as the accessSCAN, the place that is different
    //will be commented below
    printf("\n----------------\n");
    printf("CSCAN :");
    int curr = START;
    int firstLocation = -1;
    int newCnt = numRequest + 2;
    int *newRequest = malloc(newCnt * sizeof(int));
    int specialCase = 0;
    if (abs(curr - LOW) > abs(curr - HIGH))
    {

        for (int i = 0; i < numRequest; i++)
        {
            if (request[i] >= curr)
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
            //hit the high
            newRequest[nextLoc] = HIGH;
            nextLoc++;
            // go to the low position quickly and start in the same direction again
            newRequest[nextLoc] = LOW;
            nextLoc++;
            for (int i = 0; i < firstLocation; i++)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
        }
        //everything is larger than currerent location
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
            else if (request[i] == curr)
            {
                firstLocation = i;
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
            //hit the low 
            newRequest[nextLoc] = LOW;
            nextLoc++;
            //go to the high location quickly and start in the same direction again
            newRequest[nextLoc] = HIGH;
            nextLoc++;
            for (int i = numRequest - 1; i > firstLocation; i--)
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

//access the disk location in LOOK
void accessLOOK(int *request, int numRequest)
{
    
    qsort(request, numRequest, sizeof(int), cmpfunc);
    //write your logic here
    printf("\n----------------\n");
    printf("LOOK :");
    //most of the logic is the same as the accessSCAN, the place that is different
    //the only difference is that it will not hit either of the boundaries
    int curr = START;
    int firstLocation = -1;
    int newCnt = numRequest;
    int *newRequest = malloc(newCnt * sizeof(int));
    int specialCase = 0;
    if (abs(curr - LOW) > abs(curr - HIGH))
    {

        for (int i = 0; i < numRequest; i++)
        {
            if (request[i] >= curr)
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
            else if (request[i] == curr)
            {
                firstLocation = i;
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

//access the disk location in CLOOK
void accessCLOOK(int *request, int numRequest)
{
    //write your logic here
    //most of the logic is the same as the accessCSCAN, the place that is different
    //the only difference is that it will not hit either of the boundaries
    printf("\n----------------\n");
    printf("CLOOK :");
    int curr = START;
    int firstLocation = -1;
    int newCnt = numRequest + 1;
    int specialCase = 0;
    int *newRequest = malloc(newCnt * sizeof(int));
    if (abs(curr - LOW) > abs(curr - HIGH))
    {

        for (int i = 0; i < numRequest; i++)
        {
            if (request[i] >= curr)
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
            newRequest[nextLoc] = LOW;
            nextLoc++;
            for (int i = 0; i < firstLocation; i++)
            {
                newRequest[nextLoc] = request[i];
                nextLoc++;
            }
        }
        //everything is larger than currerent location
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
            else if (request[i] == curr)
            {
                firstLocation = i;
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
            newRequest[nextLoc] = HIGH;
            nextLoc++;
            for (int i = numRequest - 1; i > firstLocation; i--)
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


    default:
        break;
    }
    return 0;
}