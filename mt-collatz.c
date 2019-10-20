/**
 * This program is designed to compute a Collatz sequence up to a given limit using
 * a given number of threads, both specified by the user. Prints out the limit, number 
 * of threads, and run time to stderr, and a histogram of the resultant data to stdout.
 *
 * @author Mason Riley
 * @author Cesar Santiago
 * @date 10/19/2019
 * @info Course COP4634
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define LIMIT 1
#define NUM_THREADS 2
#define MAX_LINES 1000

int COUNTER = 1; //tracks the counter for all threads
int NOLOCK_FLAG = 0; //determines whether to use a lock or not
int histogram[MAX_LINES] = {0}; //stores Collatz data, frequency of stopping time vs stopping time
pthread_mutex_t lock; //used for locking
timespec start, stop; //stores start and stop times for calculating run time

/**
 *Computes the Collatz sequence for a given number n
 *
 *@param n The number for which to begin the Collatz sequence
 *
 *@return The stopping time
 */
void collatz(int n) {
    int stopTime = 0;
    while(n != 1) {
        if(n % 2 == 0)
            n = n / 2;
        else
            n = 3 * n + 1;

        ++stopTime;
    }

    histogram[stopTime - 1]++;
}

/**
 *Runs a series of Collatz sequences from 2 to N through threads
 *
 *@param param The N, or upper limit of sequences
 */ 
void *tCollatz(void *param){
    int stopTime, n, limit = *(int *)param;
    
    /*if -nolock flag is set at runtime, do not lock access to *
     *the global variable COUNTER. This is for test purposes.  */
    if(NOLOCK_FLAG != 0) {
        while(COUNTER < limit)  
            collatz(++COUNTER);
    }
    //if -nolock flag is not set, go through with locking COUNTER
    else {
        while(COUNTER < limit) {

            /* Despite locking, COUNTER may get incremented beyond the limit *
             * during while loop - check that this is not the case before    *
             * calling collatz()                                             */
            if(COUNTER < limit) {
               //Lock access to global variable COUNTER before incrementing
               pthread_mutex_lock(&lock);
               ++COUNTER;
               n = COUNTER;
               pthread_mutex_unlock(&lock);
               
               collatz(n); //Calculate stopping time for N 
            }
        }
    }

    pthread_exit(NULL);
    return NULL;
}

//Prints the histogram in the format "k, frequency(k)" as specified by the project
void printHistogram() {
    int i;
    for(i = 0; i < MAX_LINES; i++) 
        printf("%d, %d\n", i + 1, histogram[i]); 
}

/**
 *Checks for the -nolock flag, and if found sets NOLOCK_FLAG to 1 (nonzero)
 *
 *@param numArgs The number of command line arguments given
 *@param args A string array containing each individual argument
 */ 
void checkNoLock(int numArgs, char* args[]) {
    for(int i = 0; i < numArgs; i++) {
        if(strcmp(args[i], "-nolock") == 0) 
            NOLOCK_FLAG = 1;
    }
}

/**
 *Calculates the run time of the program
 *
 *@return The total run time of the program
 */ 
float calcRunTime() {
    float sec = stop.tv_sec - start.tv_sec;
    float nsec = (stop.tv_nsec - start.tv_nsec) / 1000000000.0;
    float time = sec + nsec;
    return time;   
}

/*Runs the program
 *
 *@param argc the number of arguments given
 *@param argv an array of arguments as strings
 */
int main(int argc, char** argv) { 
    int i;
    
    //Get start time
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    //Check for the correct number of arguments
    if(argc < 3) {
        printf("Invalid number of arguments, please rerun the program in the form:\n");
        printf("./mt-collatz [upper bound] [number of threads]\n");
        return 0;
    }

    //Check for the -nolock flag
    checkNoLock(argc, argv);
    
    //Get sequence upper limit from argv[1] 
    int limit = atoi(argv[LIMIT]);
    //Get number of threads from argv[2]
    int numThreads = atoi(argv[NUM_THREADS]);
    
    //Stores the tids of the threads 
    pthread_t tids[numThreads];
    
    //Initialize mutex lock
    if(pthread_mutex_init(&lock, NULL) != 0)
        fprintf(stderr, "ERROR: Mutext initialization failed.\n");

    //Create all threads and run the Collatz algorithm
    for(i = 0; i < numThreads; i++) {
        if(pthread_create(&tids[i], NULL, tCollatz, (void *) &limit))
            fprintf(stderr, "ERROR: Thread failed to create\n");
    }
    
    //Join all threads 
    for(i = 0; i < numThreads; i++) 
        pthread_join(tids[i], NULL);
    
    //Destroy mutex lock
    pthread_mutex_destroy(&lock);

    //Get stop time
    clock_gettime(CLOCK_MONOTONIC_RAW, &stop);
    //Calculate run time
    float time = calcRunTime();
    //Print out N,T,run time to stderr
    fprintf(stderr, "%d,%d,%f\n", limit, numThreads, time);

    printHistogram();
    return 0;
}




