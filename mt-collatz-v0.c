/**
 * This program is designed to compute a Collatz sequence by creating 
 * multiple threads
 *
 * @author Mason Riley
 * @author Cesar Santiago
 * @info Course COP4634
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SEQUENCE_LIMIT 1
#define NUM_THREADS 2
#define MAX_LINES 1000

typedef struct bounds {
    int lower;
    int upper;
} bounds;

int histogram[MAX_LINES];
pthread_mutex_t lock;
timespec start, stop;

void initHistogram() {
    int i;
    for(i = 0; i < MAX_LINES; i++)
        histogram[i] = 0;
}

int collatz(int n) {
    int counter = 0;
    while(n != 1) {
        if(n % 2 == 0)
            n = n / 2;
        else
            n = 3 * n + 1;
        ++counter;
    }

    return counter - 1;
}

void* tCollatz(void* boundaries){ 
    bounds b = *((bounds*) boundaries);
    
    int i;
    for(i = b.lower; i < b.upper + 1; i++) {
        pthread_mutex_lock(&lock); 
        histogram[collatz(i)]++;
        pthread_mutex_unlock(&lock);
    } 
    return NULL;
}

void printHistogram() {
    int i, j;
    for(i = 0; i < MAX_LINES; i++) {
        if(histogram[i] > 0) { 
            printf("%d:", i + 1);
            for(j = 0; j < histogram[i]; j++)
                printf("*");
            printf("\n");
        }
    }
}

/*Runs the program
 *
 *@param argc the number of arguments given
 *@param argv an array of arguments as strings
 */
int main(int argc, char** argv) { 
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    int i = 0;

    //Check for the correct number of arguments
    if(argc > 3 || argc < 3) {
        printf("Invalid number of arguments, please rerun the program in the form:\n");
        printf("./mt-collatz [upper bound] [number of threads]\n");
        return 0;
    }

    initHistogram();
        
    int sequenceLimit = atoi(argv[SEQUENCE_LIMIT]);
    int numThreads = atoi(argv[NUM_THREADS]);
    
    pthread_t tids[numThreads];
    bounds bArr[numThreads];
    
    //Get the range/size of each step
    int step = sequenceLimit / numThreads;
    //The lower bound is always n = 2;
    int from = 2;
    //The upper bound is the step + the lower bound - the base case
    int to = from + step - 2; 
    
    pthread_mutex_init(&lock, NULL);
    for(i = 0; i < numThreads; i++) {
        bArr[i].lower = from;
        if(i != numThreads - 1)
            bArr[i].upper = to;
        else
            bArr[i].upper = sequenceLimit; 
        
        if(pthread_create(&tids[i], NULL, tCollatz, (void *) &bArr[i]))
            printf("ERROR: Thread failed to create\n");

        from = to + 1;
        to = from + step - 1;
    }
    
    for(i = 0; i < numThreads; ++i) {
        printf("Waiting for thread %lu to join...\n", tids[i]);
        pthread_join(tids[i], NULL);
        printf("Thread %lu joined successfully\n", tids[i]);
    }

    pthread_mutex_destroy(&lock);
    clock_gettime(CLOCK_MONOTONIC_RAW, &stop);
    printf("Total time = %f seconds\n", (stop.tv_nsec - start.tv_nsec) / 1000000000.0 + (stop.tv_sec - start.tv_sec));
    printHistogram();
}




