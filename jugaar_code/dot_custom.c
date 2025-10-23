#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Configuration constants
#define VECLEN 1000000   // Length of each vector (number of elements)
#define NUM_THREADS 4     // Number of threads to create

// Structure to hold vector data and shared result
typedef struct {
    double *a;     // Pointer to first vector
    double *b;     // Pointer to second vector
    double sum;    // Shared variable to store final dot product result
    int veclen;    // Total length of vectors
} DOTDATA;

// Global shared data instance
DOTDATA dotstr;

// Mutex lock for protecting the shared sum variable
pthread_mutex_t mutexsum;

// Structure to define each thread's work range
typedef struct {
    int start;   // Start index (inclusive)
    int end;     // End index (exclusive)
} ThreadData;

// Thread function: computes partial dot product
void *dotprod(void *arg) {
    ThreadData *data = (ThreadData *)arg;  // Get thread-specific data
    double *x = dotstr.a;
    double *y = dotstr.b;
    double mysum = 0;                      // Local partial sum (private to thread)

    // Compute the partial dot product for assigned range
    for (int i = data->start; i < data->end; i++) {
        mysum += x[i] * y[i];
    }

    // Lock the mutex before updating the shared sum
    pthread_mutex_lock(&mutexsum);
    dotstr.sum += mysum;                   // Add local sum to global shared sum
    pthread_mutex_unlock(&mutexsum);       // Unlock after update

    // Exit the thread
    pthread_exit(NULL);
}

// Main function: sets up data, creates threads, and calculates time
int main() {
    int i, len;
    double *a, *b;
    pthread_t threads[NUM_THREADS];        // Array to hold thread IDs
    ThreadData thread_data[NUM_THREADS];   // Array to hold thread-specific data

    len = VECLEN;

    // Allocate memory for the two vectors
    a = (double *)malloc(len * sizeof(double));
    b = (double *)malloc(len * sizeof(double));

    // Initialize vectors with sample data (all 1.0)
    for (i = 0; i < len; i++) {
        a[i] = 1.0;
        b[i] = a[i];
    }

    // Store vector info in global structure
    dotstr.veclen = len;
    dotstr.a = a;
    dotstr.b = b;
    dotstr.sum = 0;

    // Initialize the mutex for synchronization
    pthread_mutex_init(&mutexsum, NULL);

    // Record start time
    clock_t start = clock();

    // Determine chunk size (number of elements per thread)
    int chunk = len / NUM_THREADS;

    // Create threads and assign ranges of data

    for (i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * chunk;
        // Last thread takes any leftover elements
        thread_data[i].end = (i == NUM_THREADS - 1) ? len : (i + 1) * chunk;

        // Create a thread to process its assigned chunk
        pthread_create(&threads[i], NULL, dotprod, (void *)&thread_data[i]);
    }

    // Wait for all threads to complete execution
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Record end time
    clock_t end = clock();

    // Calculate total execution time
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;


    printf("Multi-threaded Execution Finished.\n");
    printf("Sum = %f\n", dotstr.sum);
    printf("Execution Time: %f seconds\n", time_spent);

    // Destroy the mutex and free allocated memory
    pthread_mutex_destroy(&mutexsum);
    free(a);
    free(b);
    return 0;
}
