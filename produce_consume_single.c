// Single producer/consumer problem using mutex & conditional variable


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 3   // Small size to trigger waits

int buffer[BUFFER_SIZE];
int in = 0, out = 0, count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full  = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    int id = *((int *)arg);
    int item = 0;

    while (1) {
        sleep(1);  // Simulate production time
        item++;

        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE) {
            printf("Producer %d waiting (buffer full)\n", id);
            pthread_cond_wait(&not_full, &mutex);
        }

        buffer[in] = item;
        printf("Producer %d produced %d at buffer[%d]\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&not_empty);  // Wake consumer
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *consumer(void *arg) {
    int id = *((int *)arg);

    while (1) {
        sleep(2);  // Simulate consumption time

        pthread_mutex_lock(&mutex);

        while (count == 0) {
            printf("Consumer %d waiting (buffer empty)\n", id);
            pthread_cond_wait(&not_empty, &mutex);
        }

        int item = buffer[out];
        printf("Consumer %d consumed %d from buffer[%d]\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&not_full);  // Wake producer
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t prod1, cons1;
    int id1 = 1, id2 = 1;

    pthread_create(&prod1, NULL, producer, &id1);
    pthread_create(&cons1, NULL, consumer, &id2);

    pthread_join(prod1, NULL);
    pthread_join(cons1, NULL);

    return 0;
}
