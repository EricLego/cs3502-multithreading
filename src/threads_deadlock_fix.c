#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TRANSFERS 1000

int account1 = 1000, account2 = 1000;  // Shared resources
pthread_mutex_t lock1, lock2;  // Two locks

void *transfer_AtoB(void *arg) {
    for (int i = 0; i < TRANSFERS; i++) {
        // Always lock in the same order: lock1 -> lock2
        pthread_mutex_lock(&lock1);
        usleep(100);  // Simulate processing time
        pthread_mutex_lock(&lock2);

        account1 -= 10;
        account2 += 10;

        pthread_mutex_unlock(&lock2);
        pthread_mutex_unlock(&lock1);
    }
    printf("Transfer A to B completed.\n");
    pthread_exit(NULL);
}

void *transfer_BtoA(void *arg) {
    for (int i = 0; i < TRANSFERS; i++) {
        // Always lock in the same order: lock1 -> lock2 (Prevents deadlock)
        pthread_mutex_lock(&lock1);
        usleep(100);
        pthread_mutex_lock(&lock2);

        account2 -= 10;
        account1 += 10;

        pthread_mutex_unlock(&lock2);
        pthread_mutex_unlock(&lock1);
    }
    printf("Transfer B to A completed.\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2;

    // Initialize mutexes
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    // Create threads
    pthread_create(&thread1, NULL, transfer_AtoB, NULL);
    pthread_create(&thread2, NULL, transfer_BtoA, NULL);

    // Wait for both threads
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Destroy mutexes
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    printf("Final account balances: Account 1 = $%d, Account 2 = $%d\n", account1, account2);
    return 0;
}
