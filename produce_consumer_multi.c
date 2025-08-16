//Multiple producer/consumer problem using mutex & conditional variable

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 3

int buffer[BUFFER_SIZE];
int in = 0, out = 0, count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full  = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    int id = *((int *)arg);
    int item = 0;

    while (1) {
        sleep(rand() % 3 + 1);  // Random production time
        item++;

        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE) {
            printf("Producer %d waiting (buffer full)\n", id);
            pthread_cond_wait(&not_full, &mutex);
        }

        buffer[in] = item;
        printf("Producer %d produced %d at buffer[%d] (count=%d)\n", 
               id, item, in, count+1);
        in = (in + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&not_empty);  // Wake up a consumer
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *consumer(void *arg) {
    int id = *((int *)arg);

    while (1) {
        sleep(rand() % 4 + 1);  // Random consumption time

        pthread_mutex_lock(&mutex);

        while (count == 0) {
            printf("Consumer %d waiting (buffer empty)\n", id);
            pthread_cond_wait(&not_empty, &mutex);
        }

        int item = buffer[out];
        printf("Consumer %d consumed %d from buffer[%d] (count=%d)\n", 
               id, item, out, count-1);
        out = (out + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&not_full);  // Wake up a producer
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t prod_threads[NUM_PRODUCERS], cons_threads[NUM_CONSUMERS];
    int prod_ids[NUM_PRODUCERS], cons_ids[NUM_CONSUMERS];

    srand(time(NULL));

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        prod_ids[i] = i + 1;
        pthread_create(&prod_threads[i], NULL, producer, &prod_ids[i]);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        cons_ids[i] = i + 1;
        pthread_create(&cons_threads[i], NULL, consumer, &cons_ids[i]);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(prod_threads[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(cons_threads[i], NULL);
    }

    return 0;
}


/*

✅ Now multiple producers & consumers are running in parallel:
Producers compete for free slots in the buffer.
Consumers compete for filled slots.
pthread_cond_signal ensures fairness so no thread starves.

*/
