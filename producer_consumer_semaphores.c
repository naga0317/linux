// Producer-Consumer with Semaphores

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define PRODUCERS   2
#define CONSUMERS   3
#define ITEMS_TO_PRODUCE 10

int buffer[BUFFER_SIZE];
int in = 0;   // next insert position
int out = 0;  // next remove position

// Synchronization
sem_t empty;   // counts empty slots
sem_t full;    // counts filled slots
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < ITEMS_TO_PRODUCE; i++) {
        int item = rand() % 100;

        sem_wait(&empty);              // wait for empty slot
        pthread_mutex_lock(&mutex);    // enter critical section

        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        printf("Producer %d produced %d\n", id, item);

        pthread_mutex_unlock(&mutex);  // leave critical section
        sem_post(&full);               // signal item available

        sleep(1); // simulate work
    }
    return NULL;
}

void *consumer(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < ITEMS_TO_PRODUCE; i++) {
        sem_wait(&full);               // wait for item
        pthread_mutex_lock(&mutex);    // enter critical section

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        printf("Consumer %d consumed %d\n", id, item);

        pthread_mutex_unlock(&mutex);  // leave critical section
        sem_post(&empty);              // signal empty slot available

        sleep(2); // simulate work
    }
    return NULL;
}

int main() {
    pthread_t prod[PRODUCERS], cons[CONSUMERS];
    int prod_id[PRODUCERS], cons_id[CONSUMERS];

    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    for (int i = 0; i < PRODUCERS; i++) {
        prod_id[i] = i + 1;
        pthread_create(&prod[i], NULL, producer, &prod_id[i]);
    }

    for (int i = 0; i < CONSUMERS; i++) {
        cons_id[i] = i + 1;
        pthread_create(&cons[i], NULL, consumer, &cons_id[i]);
    }

    for (int i = 0; i < PRODUCERS; i++) {
        pthread_join(prod[i], NULL);
    }

    for (int i = 0; i < CONSUMERS; i++) {
        pthread_join(cons[i], NULL);
    }

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}

/*

🔹 Key Points
sem_t empty starts at BUFFER_SIZE → ensures producers stop when buffer is full.
sem_t full starts at 0 → ensures consumers stop when buffer is empty.
Mutex ensures mutual exclusion when accessing the circular buffer.
This approach automatically prevents starvation because semaphores ensure fairness in waking threads.

*/
