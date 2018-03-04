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
#include<fcntl.h>
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
int hours = 10;
sem_t full;
sem_t empty;
pthread_mutex_t mutex;
/*Producer Function: Simulates an Airplane arriving and dumping 5-10 passengers to the taxi platform */
void *FnAirplane(void *cl_id)
{
    int passengerID, emptyValue, planeID;
    planeID = *((int *)cl_id);
    for (int h = 0; h < hours; h++)
    {
        int numPassengers = rand() % 6 + 5;
        printf("Airplane %d arrives with %d passengers\n", planeID, numPassengers);
        for (int i = 0; i < numPassengers; i++)
        {
            passengerID = 1 * 1000000 + planeID * 1000 + i;
            sem_getvalue(&empty, &emptyValue);
            if (emptyValue > 0)
            {
                sem_wait(&empty);
                pthread_mutex_lock(&mutex);
                enqueue(queue, passengerID);
                printf("Passenger %d of airplane %d arrives to platform\n", passengerID, planeID);
                pthread_mutex_unlock(&mutex);
                sem_post(&full);
            }
            else
            {
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
    int taxiID = *((int *)pr_id);
    int fullValue;
    while (1)
    {
        int travelTime = rand() % 21 + 10;
        usleep(travelTime);
        printf("Taxi driver %d arrives\n", taxiID);
        sem_getvalue(&full, &fullValue);
        if (fullValue == 0)
        {
            printf("Taxi driver %d waits for passengers to enter the platform\n", taxiID);
        }
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int passengerID = dequeue(queue);
        printf("Taxi driver %d picked up client %d from the platform\n", taxiID, passengerID);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

    
int main(int argc, char *argv[])
{

    int num_airplanes;
    int num_taxis;

    num_airplanes = atoi(argv[1]);
    num_taxis = atoi(argv[2]);
    int bak,fd;
    bak = dup(1);//make a copy of the current file descriptor
    close(1);//close stdout at the default location
    fd = open("simulation_out.txt",O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    if(fd <0){//handle error
        return -1;
    }
    dup(fd);
    printf("You entered: %d airplanes per hour\n", num_airplanes);
    printf("You entered: %d taxis\n", num_taxis);
    //initialize queue
    queue = createQueue(BUFFER_SIZE);

    //declare arrays of threads and initialize semaphore(s)
    pthread_t planes[num_airplanes];
    pthread_t taxis[num_taxis];
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    pthread_mutex_init(&mutex, NULL);

    //create arrays of integer pointers to ids for taxi / airplane threads
    int *taxi_ids[num_taxis];
    int *airplane_ids[num_airplanes];

    //create threads for airplanes

    for (int i = 0; i < num_airplanes; i++)
    {

        airplane_ids[i] = malloc(sizeof(int));
        *airplane_ids[i] = i;
        printf("Creating airplane thread %d\n", i);
        if (pthread_create(&planes[i], NULL, FnAirplane, airplane_ids[i]) != 0)
        {
            printf("Error when creating thread for plane thread number : %d\n", i);
        }
    }

    //create threads for taxis
    for (int i = 0; i < num_taxis; i++)
    {
        taxi_ids[i] = malloc(sizeof(int));
        *taxi_ids[i] = i;
        //printf("Creating airplane thread %d\n",i);
        if (pthread_create(&taxis[i], NULL, FnTaxi, taxi_ids[i]) != 0)
        {
            printf("Error when creating thread for taxi thread number : %d\n", i);
        }
    }

    pthread_exit(NULL);
}
