/*
----------------- COMP 310/ECSE 427 Winter 2018 -----------------
I declare that the awesomeness below is a genuine piece of work
and falls under the McGill code of conduct, to the best of my knowledge.
-----------------------------------------------------------------
*/

//Please enter your name and McGill ID below
//Name: <Yingnan Zhao>
//McGill ID: <260563769>

//all the header files you would require
#include <stdio.h>    //for standard IO
#include <unistd.h>   //for execvp/dup/dup2
#include <string.h>   //for string manipulation
#include <stdlib.h>   //for fork
#include <ctype.h>    //for character type check (isnum,isalpha,isupper etc)
#include <sys/wait.h> //for waitpid
#include <fcntl.h>    //open function to open a file. type "man 2 open" in terminal
#include <time.h>     //to handle time
#include <sys/stat.h>
//pointer to Linked list head
struct node *head_job = NULL;

//pointer to
struct node *current_job = NULL;

//global variable used to store process id of process
//that has been just created
//can be used while adding a job to linked list
pid_t process_id;

//flag variable to check if redirection of output is required
int isred = 0;

//structure of a single node
//donot modify this structure
struct node
{
    int number;        //the job number
    int pid;           //the process id of the process
    char *cmd;         //string to store the command name
    time_t spawn;      //time to store the time it was spawned
    struct node *next; //when another process is called you add to the end of the linked list
};

// Add a job to the linked list
void addToJobList(char *args[])
{
    //allocate memory for the new job
    struct node *job = malloc(sizeof(struct node));
    //If the job list is empty, create a new head
    if (head_job == NULL)
    {
        //init the job number with 1
        job->number = 1;
        //set its pid from the global variable process_id
        job->pid = process_id;
        //cmd can be set to arg[0]
        job->cmd = args[0];
        //set the job->next to point to NULL.
        job->next = NULL;
        //set the job->spawn using time function
        job->spawn = (unsigned int)time(NULL);
        //set head_job to be the job
        head_job = job;
        //set current_job to be head_job
        current_job = head_job;
    }

    //Otherwise create a new job node and link the current node to it
    else
    {
        //point current_job to head_job
        current_job = head_job;
        //traverse the linked list to reach the last job
        while (current_job->next != NULL)
        {
            current_job = current_job->next;
        }

        //init all values of the job like above num,pid,cmd.spawn
        job->number = current_job->number + 1;
        //set its pid from the global variable process_id
        job->pid = process_id;
        //cmd can be set to arg[0]
        job->cmd = args[0];
        //set the job->next to point to NULL.
        job->next = NULL;
        //set the job->spawn using time function
        job->spawn = (unsigned int)time(NULL);

        //make next of current_job point to job
        current_job->next = job;
        //make job to be current_job
        current_job = current_job->next;
        //set the next of job to be NULL
        current_job->next = NULL;
    }
}

//Function to refresh job list
//Run through jobs in linked list and check
//if they are done executing then remove it
void refreshJobList()
{
    //pointer require to perform operation
    //on linked list
    struct node *current_job;
    struct node *prev_job;

    //variable to store returned pid
    pid_t ret_pid;

    //perform init for pointers
    current_job = head_job;
    prev_job = head_job;

    //traverse through the linked list
    while (current_job != NULL)
    {

        //use waitpid to init ret_pid variable
        ret_pid = waitpid(current_job->pid, NULL, WNOHANG);
        //one of the below needs node removal from linked list
        if (ret_pid == 0)
        {
            //not finished
            //continue tranverse
            prev_job = current_job;
            current_job = prev_job->next;
        }
        else
        {
            if (prev_job == current_job)
            {

                prev_job = prev_job->next;
                free(head_job);
                head_job = prev_job;
                current_job = prev_job;
            }
            else
            {
                prev_job->next = current_job->next;
                free(current_job);
                current_job = prev_job->next;
            }
        }
    }
    return;
}

//Function that list all the jobs
void listAllJobs()
{
    struct node *current_job;
    int ret_pid;

    //refresh the linked list
    refreshJobList();

    //init current_job with head_job
    if (head_job != NULL)
    {
        current_job = head_job;
        //heading row print only once.
        printf("\nID\tPID\tCmd\tstatus\tspawn-time\n");
        while (current_job != NULL)
        {
            //traverse the linked list and print using the following statement for each job
            printf("%d\t%d\t%s\tRUNNING\t%s\n", current_job->number, current_job->pid, current_job->cmd, ctime(&(current_job->spawn)));
            current_job = current_job->next;
        }
    }
    return;
}

// wait till the linked list is empty
// you would have to look for a place
// where you would call this function.
// donot modify this function
void waitForEmptyLL(int nice, int bg)
{
    if (nice == 1 && bg == 0)
    {
        while (head_job != NULL)
        {
            sleep(1);
            refreshJobList();
        }
    }
    return;
}

//function to perform word count
int wordCount(char *filename, char *flag)
{
    int cnt = 0;
    FILE *fp;
    fp = fopen(filename, "r");
    char c;
    if(fp == NULL){
        if (!(!strcmp("-l", flag)||!strcmp("-w",flag))){
            printf("unrecognized flag\n");
        }
        printf ("file does not exist\n");
        return 0;
    }
    if (!strcmp("-l", flag))
    {
        while ((c = fgetc(fp)) != EOF)
        {
            if (c == '\n')
            {
                cnt = cnt + 1;
            }
        }
    }
    else if (!strcmp("-w",flag))
    {
        while ((c = fgetc(fp)) != EOF)
        {
            if (c == '\n' || c == ' ')
            {
                while ((c = fgetc(fp)) == ' ')
                {
                    cnt = cnt;
                }
                cnt = cnt + 1;
            }
        }
    }
    else
    {
        printf("unrecognized flag\n");
    }

    fclose(fp);
    return cnt;
}

// function to augment waiting times for a process
// donot modify this function
void performAugmentedWait()
{
    int w, rem;
    time_t now;
    srand((unsigned int)(time(&now)));
    w = rand() % 15;
    printf("sleeping for %d\n", w);
    rem = sleep(w);
    return;
}

//simulates running process to foreground
//by making the parent process wait for
//a particular process id.
int waitforjob(char *jobnc)
{
    struct node *trv;
    int jobn = (*jobnc) - '0';
    trv = head_job;
    //traverse through linked list and find the corresponding job
    //hint : traversal done in other functions too
    while (trv->number != jobn)
    {

        trv = trv->next;
    }
    //if correspoding job is found
    if (trv->number == jobn)
    {
        //use its pid to make the parent process wait.
        //waitpid with proper argument needed here
        printf("Bringing jobno %d and pid %d to foreground\n",trv->number,trv->pid);
        waitpid(trv->pid, NULL, WUNTRACED);
    }
    else
    {
        printf("job not find\n");
    }
    return 0;
}

// splits whatever the user enters and sets the background/nice flag variable
// and returns the number of tokens processed
// donot modify this function
int getcmd(char *prompt, char *args[], int *background, int *nice)
{
    int length, i = 0;
    char *token, *loc;
    char *line = NULL;
    size_t linecap = 0;
    printf("%s", prompt);
    length = getline(&line, &linecap, stdin);
    if (length <= 0)
    {
        exit(-1);
    }
    // Check if background is specified..
    if ((loc = index(line, '&')) != NULL)
    {
        *background = 1;
        *loc = ' ';
    }
    else
        *background = 0;
    while ((token = strsep(&line, " \t\n")) != NULL)
    {
        for (int j = 0; j < strlen(token); j++)
            if (token[j] <= 32)
                token[j] = '\0';
        if (strlen(token) > 0)
        {
            if (!strcmp("nice", token))
            {
                *nice = 1;
            }
            else
            {
                args[i++] = token;
            }
        }
    }
    return i;
}

// this initialises the args to All null.
// donot modify
void initialize(char *args[])
{
    for (int i = 0; i < 20; i++)
    {
        args[i] = NULL;
    }
    return;
}

int main(void)
{
    //args is a array of charater pointers
    //where each pointer points to a string
    //which may be command , flag or filename
    char *args[20];

    //flag variables for background, status and nice
    //bg set to 1 if the command is to executed in background
    //nice set to 1 if the command is nice
    //status
    int bg, status, nice;

    //variable to store the process id.
    pid_t pid;

    //variable to store the file des
    //helpful in output redirection
    int fd1, fd2;

    //your terminal executes endlessly unless
    //exit command is received
    while (1)
    {
        //init background to zero
        bg = 0;
        //init nice to zero
        nice = 0;
        //init args to null
        initialize(args);
        //get the user input command
        int cnt = getcmd("\n>> ", args, &bg, &nice);
        //keep asking unless the user enters something
        while (!(cnt >= 1))
            cnt = getcmd("\n>> ", args, &bg, &nice);
        waitForEmptyLL(nice, bg);
        //use the if-else ladder to handle built-in commands
        //built in commands don't need redirection
        //also no need to add them to jobs linked list
        //as they always run in foreground
        if (!strcmp("jobs", args[0]))
        {
            //call the listalljobs function
            listAllJobs();
        }
        else if (!strcmp("exit", args[0]))
        {
            //exit the execution of endless while loop
            exit(0);
        }
        else if (!strcmp("fg", args[0]))
        {
            //bring a background process to foreground
            waitforjob(args[1]);
        }
        else if (!strcmp("cd", args[0]))
        {
            int result = 0;
            // if no destination directory given
            // change to home directory
            int length = 0;
            for (int k = 0; k < sizeof(args); ++k)
            {
                if (args[k] != NULL)
                {
                    length = length + 1;
                }
                else
                {
                    break;
                }
            }
            if (length == 1)
            {
                result = chdir("/home");
            }
            else
            {
                result = chdir(args[1]);
            }
            //if given directory does not exist
            //print directory does not exit
            if (result == -1)
            {
                printf("%s no such file or directory\n",args[1]);
            }
            //if everthing is fine
            //change to destination directory
        }
        else if (!strcmp("pwd", args[0]))
        {
            //use getcwd and print the current working directory
            char result[1024];
            if (getcwd(result, sizeof(result)) != NULL)
            {
                printf("%s\n", result);
            }
            else
            {
                printf("pmd not success");
            }
        }
        else if (!strcmp("wc", args[0]))
        {
            //call the word count function
            int count = wordCount(args[2],args[1]);
            printf("%d", count);
        }
        else
        {
            //Now handle the executable commands here
            /* the steps can be..:
            (1) fork a child process using fork()
            (2) the child process will invoke execvp()
            (3) if background is not specified, the parent will wait,
                otherwise parent starts the next command... */

            //hint : samosas are nice but often there
            //is a long waiting line for it.

            //create a child
            pid = fork();

            //to check if it is parent
            if (pid > 0)
            {
                //we are inside parent
                // printf("inside the parent\n");

                if (bg == 0)
                {

                    // waitpid with proper argument required
                    pid = waitpid(pid, NULL, WUNTRACED);
                }
                else
                {

                    process_id = pid;
                    addToJobList(args);
                    pid = waitpid(pid, NULL, WNOHANG);

                    // waitpid with proper argument required
                }
            }
            else
            {
                // we are inside the child
                sleep(20);
                //introducing augmented delay
                //performAugmentedWait();
                //check for redirection
                //now you know what does args store
                //check if args has ">"
                //if yes set isred to 1
                //else set isred to 0
                int i = 0;
                int length = 0;
                for (int k = 0; k < sizeof(args); ++k)
                {
                    if (args[k] != NULL)
                    {
                        length = length + 1;
                    }
                    else
                    {
                        break;
                    }
                }
                for (int j = 0; j < length; ++j)
                {

                    if (!strcmp(args[j], ">"))
                    {
                        isred = 1;
                        i = j;
                        break;
                    }
                    else
                    {
                        isred = 0;
                    }
                }
                //if redirection is enabled
                if (isred == 1)
                {
                    //open file and change output from stdout to that
                    //make sure you use all the read write exec authorisation flags
                    //while you use open (man 2 open) to open file
                    fd2 = dup(1);
                    fd1 = open(args[i + 1], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
                    dup2(fd1, 1);
                    //set ">" and redirected filename to NULL
                    args[i] = NULL;
                    args[i + 1] = NULL;

                    //run your command
                    execvp(args[0], args);
                    dup2(fd2, 1);
                    //restore to stdout
                    fflush(stdout);
                }
                else
                {

                    //simply execute the command.
                    execvp(args[0], args);
                }
            }
        }
    }

    return 0;
}