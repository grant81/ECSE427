/*
 ----------------- COMP 310/ECSE 427 Winter 2018 -----------------
 Dimitri Gallos
 Assignment 2 skeleton
 
 -----------------------------------------------------------------
 I declare that the awesomeness below is a genuine piece of work
 and falls under the McGill code of conduct, to the best of my knowledge.
 -----------------------------------------------------------------
 */

//Please enter your name and McGill ID below
//Name: <Yingnan Zhao>
//McGill ID: <260563769>

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
int BUFFER_SIZE = 100; //size of queue

// A structure to represent a queue
struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int *array;
};

// function to create a queue of given capacity.
// It initializes size of queue as 0
struct Queue *createQueue(unsigned capacity)
{
    struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1; // This is important, see the enqueue
    queue->array = (int *)malloc(queue->capacity * sizeof(int));
    return queue;
}

// Queue is full when size becomes equal to the capacity
int isFull(struct Queue *queue)
{
    return ((queue->size) >= queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(struct Queue *queue)
{
    return (queue->size == 0);
}

// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue *queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    printf("%d enqueued to queue\n", item);
}

// Function to remove an item from queue.
// It changes front and size
int dequeue(struct Queue *queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
int front(struct Queue *queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}

// Function to get rear of queue
int rear(struct Queue *queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

void print(struct Queue *queue)
{
    if (queue->size == 0)
    {
        return;
    }

    for (int i = queue->front; i < queue->front + queue->size; i++)
    {

        printf(" Element at position %d is %d \n ", i % (queue->capacity), queue->array[i % (queue->capacity)]);
    }
}

struct Queue *queue;
//initialize hours to be 10
int hours = 10;
//create shared semaphores and mutex
//create semaphore full and empty
sem_t full;
sem_t empty;
//create mutex called mutex
pthread_mutex_t mutex;
/*Producer Function: Simulates an Airplane arriving and dumping 5-10 passengers to the taxi platform */
void *FnAirplane(void *cl_id)
{
    //create three integers to hold the passenger ID the value of the empty semaphore and the plane ID
    int passengerID, emptyValue, planeID;
    //cast the void pointer to a int pointer and get its value which is the plane id
    planeID = *((int *)cl_id);
    //this for loop is to run the plane(producer) procedure for a finite amount of time specified by user
    //this could be changes to a infinite loop to run the simulation forever as shown below
    //while(1)
    for (int h = 0; h < hours; h++)
    {
        //generate the number of passengers rand()%6 have a range 0-5 plus 5 gives us the desired number of
        //passengers 5 -10
        int numPassengers = rand() % 6 + 5;
        printf("Airplane %d arrives with %d passengers\n", planeID, numPassengers);
        //for each one of passenger, run the procedure below
        for (int i = 0; i < numPassengers; i++)
        {
            //generate passenger id for each passenger
            passengerID = 1 * 1000000 + planeID * 1000 + i;
            //check the value of the empty semaphore, store it in emptyValue varible
            sem_getvalue(&empty, &emptyValue);
            //if the empty value is larger than 0, which means there is space in the queue
            // we could use the isFull function to check if queue full,
            // but if another thread is changing the queue it will not be accruate
            //if (!isFull(queue))
            if (emptyValue > 0)
            {
                //decrease the empty semaphore by one
                sem_wait(&empty);
                //critical section
                //lock the mutex, if other thread want to enter, wait
                pthread_mutex_lock(&mutex);
                //put the passenger id into the queue
                enqueue(queue, passengerID);
                printf("Passenger %d of airplane %d arrives to platform\n", passengerID, planeID);
                //unlock the mutex to allow other thread enter critial section
                pthread_mutex_unlock(&mutex);
                //increase the full semaphore by one
                sem_post(&full);
            }
            else
            {
                //if the value of the empty semaphore is 0
                //print and break the loop the rest of passengers take bus
                printf("Platform is full: Rest of passengers of plane %d take the bus\n", planeID);
                break;
            }
        }
        sleep(1);
    }
}

/* Consumer Function: simulates a taxi that takes n time to take a passenger home and come back to the airport */
void *FnTaxi(void *pr_id)
{
    //cast the void pointer to a int pointer and get its value which is the taxi id
    int taxiID = *((int *)pr_id);
    int fullValue;
    //this for loop is to run the taxi(consumer) procedure for an infinite amount of time
    //assuming the taxi always go back to take new passengers
    while (1)
    {
        printf("Taxi driver %d arrives\n", taxiID);
        //check the value of the full semaphore and store in the fullValue varible
        sem_getvalue(&full, &fullValue);
        //if the value of the full semaphore is 0, print taxi waiting for passengers
        //we could use the isEmpty function to check if queue empty,
        // but if another thread is changing the queue it will not be accruate
        //if (isEmpty(queue))
        if (fullValue == 0)
        {
            printf("Taxi driver %d waits for passengers to enter the platform\n", taxiID);
        }
        //decrease the full semaphore by one if the value of full semaphore is 0, wait
        sem_wait(&full);
        //critical section
        //lock the mutex, if other thread want to enter, wait
        pthread_mutex_lock(&mutex);
        //remove a passenger from the queue and store
        int passengerID = dequeue(queue);
        printf("Taxi driver %d picked up client %d from the platform\n", taxiID, passengerID);
        //unlock the mutex to allow other thread enter critial section
        pthread_mutex_unlock(&mutex);
        //increase the empty semaphore by one
        sem_post(&empty);
        //simulate the transport time (10-30) mins
        //by create a random number from 10 - 30, then divide by 60 to calculate the portion of an hour it takes
        float tempTime = (float)(rand() % 21 + 10);
        int travelTime = (int)(tempTime / 60 * 1000000);
        usleep(travelTime);
    }
}

int main(int argc, char *argv[])
{

    int num_airplanes;
    int num_taxis;

    num_airplanes = atoi(argv[1]);
    num_taxis = atoi(argv[2]);
    /* int bak,fd;
    bak = dup(1);//make a copy of the current file descriptor
    close(1);//close stdout at the default location
    fd = open("simulation_out.txt",O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    if(fd <0){//handle error
        return -1;
    }
    dup(fd);*/
    printf("You entered: %d airplanes per hour\n", num_airplanes);
    printf("You entered: %d taxis\n", num_taxis);
    //initialize queue
    queue = createQueue(BUFFER_SIZE);

    //declare arrays of threads and initialize semaphore(s)
    //create the a array of threads for a given number of planes
    pthread_t planes[num_airplanes];
    //create the a array of threads for a given number of taxi
    pthread_t taxis[num_taxis];
    //initialize the full and empty semaphore and the mutex
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    pthread_mutex_init(&mutex, NULL);

    //create arrays of integer pointers to ids for taxi / airplane threads
    int *taxi_ids[num_taxis];
    int *airplane_ids[num_airplanes];

    //create threads for airplanes
    for (int i = 0; i < num_airplanes; i++)
    {
        //give the i th plane id pointer the size of an int
        airplane_ids[i] = malloc(sizeof(int));
        //assign the value of the plane id to that pointer
        *airplane_ids[i] = i;
        printf("Creating airplane thread %d\n", i);
        //create plane threads passing in the plane function and plane id pointer
        if (pthread_create(&planes[i], NULL, FnAirplane, airplane_ids[i]) != 0)
        {
            printf("Error when creating thread for plane thread number : %d\n", i);
        }
    }

    //create threads for taxis
    for (int i = 0; i < num_taxis; i++)
    {
        //give the i th taxi id pointer the size of an int
        taxi_ids[i] = malloc(sizeof(int));
        //assign the value of the taxi id to that pointer
        *taxi_ids[i] = i;
        //create taxi threads passing in the taxi function and taxi id pointer
        if (pthread_create(&taxis[i], NULL, FnTaxi, taxi_ids[i]) != 0)
        {
            printf("Error when creating thread for taxi thread number : %d\n", i);
        }
    }

    pthread_exit(NULL);
}
