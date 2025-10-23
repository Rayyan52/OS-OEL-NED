#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>


#define VECLEN 1000000
#define NUM_THREADS 4

typedef struct {
    double *a;
    double *b;
    double sum;
    int veclen;
} DOTDATA;

DOTDATA dotstr;
pthread_mutex_t mutexsum;

typedef struct {
    int start;
    int end;
} ThreadData;

void *dotprod(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    double *x = dotstr.a;
    double *y = dotstr.b;
    double mysum = 0;

    for (int i = data->start; i < data->end; i++) {
        dotstr.sum += x[i] * y[i];
         usleep(1);
    }

   // pthread_mutex_lock(&mutexsum);
    dotstr.sum += mysum;
    //pthread_mutex_unlock(&mutexsum);

    pthread_exit(NULL);
}

int main() {
    int i, len;
    double *a, *b;
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    len = VECLEN;
    a = (double *)malloc(len * sizeof(double));
    b = (double *)malloc(len * sizeof(double));

    for (i = 0; i < len; i++) {
        a[i] = 1.0;
        b[i] = a[i];
    }

    dotstr.veclen = len;
    dotstr.a = a;
    dotstr.b = b;
    dotstr.sum = 0;

    pthread_mutex_init(&mutexsum, NULL);

    clock_t start = clock();

    int chunk = len / NUM_THREADS;
    for (i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * chunk;
        thread_data[i].end = (i == NUM_THREADS - 1) ? len : (i + 1) * chunk;
        pthread_create(&threads[i], NULL, dotprod, (void *)&thread_data[i]);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Multi-threaded Execution Finished.\n");
    printf("Sum = %f\n", dotstr.sum);
    printf("Execution Time: %f seconds\n", time_spent);

    pthread_mutex_destroy(&mutexsum);
    free(a);
    free(b);
    return 0;
}
