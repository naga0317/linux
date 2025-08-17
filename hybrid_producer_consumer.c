// hybrid Producer-Consumer solution that uses both semaphores (for efficient resource counting) and a mutex + condition variable (to prevent starvation and allow complex conditions).


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

sem_t empty;   // counts empty slots
sem_t full;    // counts filled slots

// Insert item into circular buffer
void put_item(int item) {
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
}

// Remove item from circular buffer
int get_item() {
    int item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    return item;
}

void* producer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 10; i++) {
        int item = rand() % 100;

        sem_wait(&empty);  // wait for an empty slot
        pthread_mutex_lock(&mutex);

        put_item(item);
        printf("Producer %d produced %d\n", id, item);

        pthread_cond_broadcast(&cond); // wake consumers (fairness)

        pthread_mutex_unlock(&mutex);
        sem_post(&full);   // signal a full slot

        sleep(rand() % 2);
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 10; i++) {
        sem_wait(&full);  // wait for an item
        pthread_mutex_lock(&mutex);

        // Extra fairness: wait if condition not met
        while (buffer[out] % 2 != 0) {  // example condition: only consume even numbers
            pthread_cond_wait(&cond, &mutex);
        }

        int item = get_item();
        printf("Consumer %d consumed %d\n", id, item);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);  // signal an empty slot

        sleep(rand() % 3);
    }
    return NULL;
}

int main() {
    pthread_t producers[NUM_PRODUCERS], consumers[NUM_CONSUMERS];
    int ids[NUM_PRODUCERS > NUM_CONSUMERS ? NUM_PRODUCERS : NUM_CONSUMERS];

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &ids[i]);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        ids[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, &ids[i]);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) pthread_join(producers[i], NULL);
    for (int i = 0; i < NUM_CONSUMERS; i++) pthread_join(consumers[i], NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
