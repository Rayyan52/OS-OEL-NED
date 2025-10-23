#include <stdio.h>
#include <stdlib.h>
#include <time.h>    // For measuring time
#include <pthread.h> // For Threads

// This structure holds all data needed for the calculation.
typedef struct
{
    double *a;
    double *b;
    double sum;
    int veclen;
} DOTDATA;

// This structure holds data needed for threads
typedef struct
{
    int start;
    int end;
} ThreadData;

#define VECLEN 1000000// Using a larger vector for more meaningful timing

DOTDATA dotstr;

pthread_mutex_t mutexsum; // For Lock Mechanism


// The function that performs the dot product calculation.
void *dotprod(void *arg)
{
    // read thread data
    ThreadData *data = (ThreadData *)arg;

    // Optional print statements to visualize Threads
    // printf("Thread Strating, Chunk = [%d, %d]\n", data->start, data->end);

    double mySum = 0.0;
    double *x = dotstr.a;
    double *y = dotstr.b;



    for (int i = data->start; i < data->end; i++)
    {
        mySum += x[i] * y[i];
    }

    // Lock
    pthread_mutex_lock(&mutexsum);

    // Update
    dotstr.sum += mySum;

    // Unlock
    pthread_mutex_unlock(&mutexsum);

    pthread_exit(NULL);
}

// Number of Threads
int NUM_THREADS = 4;


// The main program initializes data, calls the function, and prints the result.
int main(int argc, char *argv[])
{
    int i, len;
    double *a, *b;

    // Assign storage and initialize values
    len = VECLEN;
    a = (double *)malloc(len * sizeof(double));
    b = (double *)malloc(len * sizeof(double));

    for (i = 0; i < len; i++)
    {
        a[i] = 1.0;
        b[i] = a[i];
    }
    
    dotstr.veclen = len;
    dotstr.a = a;
    dotstr.b = b;
    dotstr.sum = 0;
    
    // Array with 4 threads
    pthread_t threads[NUM_THREADS];
    
    // Thread Data for all threads
    ThreadData thread_data[NUM_THREADS];
    
    // Initialize Mutex
    pthread_mutex_init(&mutexsum, NULL);

    // --- Time Measurement Start ---
    clock_t start = clock();

    // Number of Threads

    // Chunk per thread
    int chunk = len / NUM_THREADS;

    // Perform the dot product
    for (int i = 0; i < NUM_THREADS; i++)
    {
        // Assign Chunk to the thread
        thread_data[i].start = i * chunk;
        thread_data[i].end = thread_data[i].start + chunk;
        
        // Creating Threads
        pthread_create(&threads[i], NULL, dotprod, (void *)&thread_data[i]);
    }

    // Wait For Threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // --- Time Measurement End ---
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    // Print result and release storage
    printf("Parallel Execution Finished.\n");
    printf("Sum = %f \n", dotstr.sum);
    printf("Execution Time: %f seconds\n", time_spent);

    // Destroy Mutex Lock
    pthread_mutex_destroy(&mutexsum);

    free(a);
    free(b);

    return 0;
}
