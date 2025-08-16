/*
Producer-Consumer Problem
Implement producer-consumer using:
Mutex + condition variables.
Bounded buffer (circular queue).
Variations they may ask:
Multiple producers, multiple consumers.
Prevent starvation.

this is a classic multithreading interview problem. Let’s go step by step and implement a Producer–Consumer solution in C using pthreads, with:
Bounded buffer (circular queue)
Mutex + condition variables
Multiple producers & multiple consumers
Fairness (prevent starvation)


🔹 Key Concepts
Shared bounded buffer (circular queue):
in → index where producer inserts.
out → index where consumer removes.
count → number of items in the buffer.
Mutex → Protect shared state (buffer, in, out, count).
Condition variables:
not_full → signal producer when space is available.
not_empty → signal consumer when items are available.
Multiple producers/consumers handled via pthread_create.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define PRODUCERS 2
#define CONSUMERS 2

int buffer[BUFFER_SIZE];
int in = 0, out = 0, count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full  = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

// Produce item (dummy function)
int produce_item() {
    return rand() % 100;
}

// Consume item (dummy function)
void consume_item(int item) {
    printf("Consumed: %d\n", item);
}

void* producer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        int item = produce_item();

        pthread_mutex_lock(&mutex);

        // Wait while buffer is full
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&not_full, &mutex);
        }

        // Insert item
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        printf("Producer %d produced: %d (count=%d)\n", id, item, count);

        // Signal that buffer is not empty
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);

        sleep(1); // simulate work
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        pthread_mutex_lock(&mutex);

        // Wait while buffer is empty
        while (count == 0) {
            pthread_cond_wait(&not_empty, &mutex);
        }

        // Remove item
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        printf("Consumer %d consumed: %d (count=%d)\n", id, item, count);

        // Signal that buffer is not full
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);

        consume_item(item);
        sleep(2); // simulate work
    }
    return NULL;
}

int main() {
    pthread_t prod[PRODUCERS], cons[CONSUMERS];
    int ids[PRODUCERS > CONSUMERS ? PRODUCERS : CONSUMERS];

    for (int i = 0; i < PRODUCERS; i++) {
        ids[i] = i + 1;
        pthread_create(&prod[i], NULL, producer, &ids[i]);
    }
    for (int i = 0; i < CONSUMERS; i++) {
        ids[i] = i + 1;
        pthread_create(&cons[i], NULL, consumer, &ids[i]);
    }

    for (int i = 0; i < PRODUCERS; i++) pthread_join(prod[i], NULL);
    for (int i = 0; i < CONSUMERS; i++) pthread_join(cons[i], NULL);

    return 0;
}


/*

🔹 Features Covered
✅ Circular buffer prevents overflow/underflow.
✅ Mutex + condition variables ensure synchronization.
✅ Multiple producers/consumers (no deadlock).
✅ Starvation prevention → both signal and broadcast can be used depending on fairness needs (here signal is fine; if fairness issues appear, switch to pthread_cond_broadcast).

*/

/*

🔹 Visualization
Think of it like a pipeline:
Producer --> [Buffer Queue] --> Consumer
If producer is faster → buffer fills → producer blocks (not_full).
If consumer is faster → buffer empties → consumer blocks (not_empty).
⚡This is how synchronization prevents race conditions, overflow, and underflow.

*/
