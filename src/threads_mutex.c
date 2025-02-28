#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define TRANSACTIONS 1000

int balance = 0;  // Shared resource
pthread_mutex_t balance_mutex;  // Mutex lock

void *deposit(void *thread_id) {
    long tid = (long)thread_id;
    for (int i = 0; i < TRANSACTIONS; i++) {
        pthread_mutex_lock(&balance_mutex);  // Lock before modifying shared resource
        balance += 10;  // Deposit $10
        pthread_mutex_unlock(&balance_mutex);  // Unlock after modification
    }
    printf("Thread %ld finished transactions.\n", tid);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Initialize the mutex
    pthread_mutex_init(&balance_mutex, NULL);

    // Create threads
    for (long t = 0; t < NUM_THREADS; t++) {
        pthread_create(&threads[t], NULL, deposit, (void *)t);
    }

    // Wait for all threads to finish
    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&balance_mutex);

    printf("Final balance: $%d\n", balance);
    return 0;
}
