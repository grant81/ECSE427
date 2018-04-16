/*
 ----------------- COMP 310/ECSE 427 Winter 2018 -----------------
  Name: <Yingnan Zhao>
  McGill ID: <260563769>
  Assignment 3 Q3 1
 
 -----------------------------------------------------------------
 I declare that the awesomeness below is a genuine piece of work
 and falls under the McGill code of conduct, to the best of my knowledge.
 -----------------------------------------------------------------
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/*
Simulates resource requests by processes
*/
int numProcess = 0;
int numResource = 0;
pthread_mutex_t mutex;
int *aviResource;
int **maxTable;
int **need;
int **hold;

void printArray(int param[], int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%d ", param[i]);
    }
    printf("\n");
}

void request_simulator(int pr_id, int *request_vector)
{
    printf("simulating request for %d needing: ", pr_id);
    printArray(need[pr_id], numResource);
    for (int i = 0; i < numResource; i++)
    {
        if (need[pr_id][i] != 0)
        {

            request_vector[i] = rand() % (need[pr_id][i] + 1);
        }
        else
        {
            request_vector[i] = 0;
        }
    }
    printf("request vector:  ");
    printArray(request_vector, numResource);
}
/*
Implementation of isSafe() as described in the slides
*/
int isSafe()
{
    int *work = malloc(sizeof(int) * numResource);
    for (int j = 0; j < numResource; j++)
    {
        work[j] = aviResource[j];
    }
    int isSafe = 0;
    int finish[numProcess];
    for (int i = 0; i < numProcess; i++)
    {
        finish[i] = 0;
    }
    int sum = 0;

    for (int i = 0; i < numProcess; i++)
    {
        if (finish[i] == 0)
        {
            for (int j = 0; j < numResource; j++)
            {
                if (need[i][j] > work[j])
                {
                    break;
                }
                else if (j == numResource - 1)
                {
                    for (int k = 0; k < numResource; k++)
                    {
                        work[k] += hold[i][k];
                    }
                    finish[i] = 1;
                    sum += 1;
                    i = -1; //loop extra
                }
            }
        }
        if (i == numProcess - 1 && sum == 0)
        {
            return isSafe;
        }
    }
    if (sum == numProcess)
    {
        isSafe = 1;
    }
    return isSafe;
}
/*
Implementation of Bankers Algorithm as described in the slides
returns 1 if safe allocation 0 if not safe
*/
int bankers_algorithm(int pr_id, int *request_vector)
{
    printf("bankers for %d\n", pr_id);
    for (int j = 0; j < numResource; j++)
    {
        if (request_vector[j] > need[pr_id][j])
        {
            printf("Allocation is not safe for process %d\n", pr_id);
            return 0;
        }
        else if (request_vector[j] > aviResource[j])
        {
            printf("Allocation is not safe for process %d\n", pr_id);
            return 0;
        }
    }
    for (int j = 0; j < numResource; j++)
    {
        // pthread_mutex_lock(&mutex);
        aviResource[j] -= request_vector[j];
        hold[pr_id][j] += request_vector[j];
        need[pr_id][j] -= request_vector[j];
        // pthread_mutex_unlock(&mutex);
    }
    if (isSafe() == 1)
    {
        printf("process %d is safe, still needing: ", pr_id);
        printArray(need[pr_id], numResource);
        return 1;
    }
    else
    {
        for (int j = 0; j < numResource; j++)
        {
            // pthread_mutex_lock(&mutex);
            aviResource[j] += request_vector[j];
            hold[pr_id][j] -= request_vector[j];
            need[pr_id][j] += request_vector[j];
            // pthread_mutex_unlock(&mutex);
        }
        printf("Allocation is not safe for process %d\n", pr_id);
    }
    return 0;
}

/*
Simulates processes running on the system.

*/
void *process_simulator(void *pr_id)
{
    int request_vector[numResource];
    for (int i = 0; i < numResource; i++)
    {
        request_vector[i] = 0;
    }
    int done = 0;
    int needSleep =0;
    int processID = *((int *)pr_id);
    printf("process %d running\n", processID);

    while (1)
    {
        request_simulator(processID, request_vector);
        pthread_mutex_lock(&mutex);
        needSleep = 0;
        if (bankers_algorithm(processID, request_vector) == 1)
        {
            for (int j = 0; j < numResource; j++)
            {
                if (need[processID][j] != 0)
                {
                    needSleep =1;
                    break;
                }
                else if (j == numResource - 1)
                {
                    done = 1;
                }
            }
        }
        if (done == 1)
        {
            for (int j = 0; j < numResource; j++)
            {
                // pthread_mutex_lock(&mutex);
                aviResource[j] += hold[processID][j];
                hold[processID][j] = 0;
                // pthread_mutex_unlock(&mutex);
            }
        }
        pthread_mutex_unlock(&mutex);
        if (done == 1)
        {
            break;
        }
        else if(needSleep==1){
            sleep(3);
        }
    }
    printf("process %d finished\n", processID);
}

int main()
{

    //Initialize all inputs to banker's algorithm
    char inputBuffer[20];
    pthread_mutex_init(&mutex, NULL);
    while (numProcess == 0 || numResource == 0)
    {
        printf("Please input the number of processes\n");
        scanf("%d", &numProcess);
        printf("Please input the number of resources\n");
        scanf("%d", &numResource);
    }
    aviResource = malloc(sizeof(int) * numResource);

    for (int i = 0; i < numResource; i++)
    {
        int temp = 0;
        printf("Please input the availability of resource number %d\n", i);
        scanf("%d", &temp);
        aviResource[i] = temp;
    }
    maxTable = malloc(numProcess * sizeof(int *));
    need = malloc(numProcess * sizeof(int *));
    hold = malloc(numProcess * sizeof(int *));
    for (int i = 0; i < numResource; i++)
    {
        maxTable[i] = malloc(numResource * sizeof(int));
        need[i] = malloc(numResource * sizeof(int));
        hold[i] = malloc(numResource * sizeof(int));
    }

    for (int i = 0; i < numProcess; i++)
    {
        printf("please input the Maximum resource process number %d can claim\n", i);
        for (int j = 0; j < numResource; j++)
        {

            scanf("%d", &maxTable[i][j]);
            if (maxTable[i][j] > aviResource[j])
            {
                printf("the required resource is larger than available resources\n");
                return 0;
            }
            need[i][j] = maxTable[i][j];
            hold[i][j] = 0;
        }
    }
    // memcpy(need, maxTable, sizeof(maxTable));
    printf("the number of process: %d\n", numProcess);
    printf("the number of resource: %d\n", numResource);
    printf("the number of available resource:\n");
    printArray(aviResource, numResource);
    printf("Maximum Claim Table:\n");
    for (int i = 0; i < numProcess; i++)
    {
        printArray(maxTable[i], numResource);
    }
    // create threads simulating processes (process_simulator)
    pthread_t processes[numProcess];
    int *process_ids[numProcess];
    for (int i = 0; i < numProcess; i++)
    {
        printf("Creating process %d\n", i);
        process_ids[i] = malloc(sizeof(int));
        //assign the value of the plane id to that pointer
        *process_ids[i] = i;
        //create plane threads passing in the plane function and plane id pointer
        if (pthread_create(&processes[i], NULL, process_simulator, process_ids[i]) != 0)
        {
            printf("Error when creating thread for process thread number : %d\n", i);
        }
    }

    // create a thread that takes away resources from the available pool (fault_simulator)

    //create a thread to check for deadlock (deadlock_checker)
    pthread_exit(NULL);
    return 0;
}