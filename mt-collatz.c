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

#define LIMIT 1
#define NUM_THREADS 2
#define MAX_LINES 1000

int COUNTER = 2;
int histogram[MAX_LINES];
pthread_mutex_t lock;
timespec start, stop;

void initHistogram() {
    int i;
    for(i = 0; i < MAX_LINES; i++)
        histogram[i] = 0;
}

int collatz(int n) {
    int i = 0;
    while(n != 1) {
        if(n % 2 == 0)
            n = n / 2;
        else
            n = 3 * n + 1;
        ++i;
    }

    return i - 1;
}

void *tCollatz(void *param){
    int n, limit = *(int *)param;

    while(COUNTER <= limit) {
        pthread_mutex_lock(&lock);
        n = collatz(COUNTER);
        ++COUNTER;
        pthread_mutex_unlock(&lock);
        histogram[n]++;
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
        
    int limit = atoi(argv[LIMIT]);
    int numThreads = atoi(argv[NUM_THREADS]);
    
    pthread_t tids[numThreads];
    
    pthread_mutex_init(&lock, NULL);
    for(i = 0; i < numThreads; i++) {
        if(pthread_create(&tids[i], NULL, tCollatz, (void *) &limit))
            printf("ERROR: Thread failed to create\n");
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




