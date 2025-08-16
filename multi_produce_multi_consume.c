// Multiple producers multiple consumers with Mutex + Condition Variables / Bounded buffer (circular queue) / pthread_cond_broadcast() to avoid starvation.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define PRODUCERS   2
#define CONSUMERS   3
#define ITEMS_TO_PRODUCE 10

int buffer[BUFFER_SIZE];
int count = 0;   // number of items in buffer
int in = 0;      // next insert position
int out = 0;     // next remove position

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full  = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < ITEMS_TO_PRODUCE; i++) {
        int item = rand() % 100;

        pthread_mutex_lock(&mutex);

        // Wait while buffer is full
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&not_full, &mutex);
        }

        // Add item to buffer
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        printf("Producer %d produced %d (count=%d)\n", id, item, count);

        // Wake ALL consumers so no one starves
        pthread_cond_broadcast(&not_empty);

        pthread_mutex_unlock(&mutex);

        sleep(1); // simulate work
    }
    return NULL;
}

void *consumer(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < ITEMS_TO_PRODUCE; i++) {
        pthread_mutex_lock(&mutex);

        // Wait while buffer is empty
        while (count == 0) {
            pthread_cond_wait(&not_empty, &mutex);
        }

        // Remove item from buffer
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;

        printf("Consumer %d consumed %d (count=%d)\n", id, item, count);

        // Wake ALL producers so no one starves
        pthread_cond_broadcast(&not_full);

        pthread_mutex_unlock(&mutex);

        sleep(2); // simulate work
    }
    return NULL;
}

int main() {
    pthread_t prod[PRODUCERS], cons[CONSUMERS];
    int prod_id[PRODUCERS], cons_id[CONSUMERS];

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

    return 0;
}
