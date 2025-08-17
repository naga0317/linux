/*

✅ Fair Reader-Writer Lock (using ticket queue)
We’ll implement using a turn-based system:
Each thread (reader/writer) gets a ticket number.
A shared next_turn counter decides whose turn it is.
Readers can group together if consecutive tickets are readers (good throughput).
Writers get exclusive access when their turn comes.

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef enum { READER, WRITER } req_type_t;

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    int active_readers;
    int active_writers;

    unsigned long ticket;      // next ticket to give
    unsigned long next_turn;   // ticket currently served
} fair_rwlock_t;

typedef struct {
    req_type_t type;
    unsigned long my_ticket;
} rw_request_t;

fair_rwlock_t lock;

// ------------------ FAIR RWLOCK ------------------ //
void fair_rwlock_init(fair_rwlock_t *l) {
    pthread_mutex_init(&l->mutex, NULL);
    pthread_cond_init(&l->cond, NULL);
    l->active_readers = 0;
    l->active_writers = 0;
    l->ticket = 0;
    l->next_turn = 0;
}

void fair_rwlock_destroy(fair_rwlock_t *l) {
    pthread_mutex_destroy(&l->mutex);
    pthread_cond_destroy(&l->cond);
}

unsigned long fair_get_ticket(fair_rwlock_t *l) {
    return l->ticket++;
}

void fair_acquire_read(fair_rwlock_t *l) {
    pthread_mutex_lock(&l->mutex);
    unsigned long my_ticket = fair_get_ticket(l);

    while (my_ticket != l->next_turn || l->active_writers > 0) {
        pthread_cond_wait(&l->cond, &l->mutex);
    }

    // allow *this reader* and possibly subsequent readers
    l->active_readers++;
    l->next_turn++;  
    pthread_cond_broadcast(&l->cond);

    pthread_mutex_unlock(&l->mutex);
}

void fair_release_read(fair_rwlock_t *l) {
    pthread_mutex_lock(&l->mutex);
    l->active_readers--;
    if (l->active_readers == 0) {
        pthread_cond_broadcast(&l->cond);
    }
    pthread_mutex_unlock(&l->mutex);
}

void fair_acquire_write(fair_rwlock_t *l) {
    pthread_mutex_lock(&l->mutex);
    unsigned long my_ticket = fair_get_ticket(l);

    while (my_ticket != l->next_turn || l->active_readers > 0 || l->active_writers > 0) {
        pthread_cond_wait(&l->cond, &l->mutex);
    }

    l->active_writers = 1;
    l->next_turn++;
    pthread_mutex_unlock(&l->mutex);
}

void fair_release_write(fair_rwlock_t *l) {
    pthread_mutex_lock(&l->mutex);
    l->active_writers = 0;
    pthread_cond_broadcast(&l->cond);
    pthread_mutex_unlock(&l->mutex);
}

// ------------------ DEMO APP ------------------ //
int shared_data = 0;

void *reader(void *arg) {
    int id = *(int *)arg;
    while (1) {
        fair_acquire_read(&lock);
        printf("Reader %d read value = %d\n", id, shared_data);
        fair_release_read(&lock);
        usleep(150000); // 0.15s
    }
    return NULL;
}

void *writer(void *arg) {
    int id = *(int *)arg;
    while (1) {
        fair_acquire_write(&lock);
        shared_data++;
        printf("Writer %d wrote value = %d\n", id, shared_data);
        fair_release_write(&lock);
        usleep(300000); // 0.3s
    }
    return NULL;
}

int main() {
    pthread_t rthreads[4], wthreads[2];
    int rids[4] = {1, 2, 3, 4};
    int wids[2] = {1, 2};

    fair_rwlock_init(&lock);

    // start readers
    for (int i = 0; i < 4; i++) {
        pthread_create(&rthreads[i], NULL, reader, &rids[i]);
    }
    // start writers
    for (int i = 0; i < 2; i++) {
        pthread_create(&wthreads[i], NULL, writer, &wids[i]);
    }

    for (int i = 0; i < 4; i++) pthread_join(rthreads[i], NULL);
    for (int i = 0; i < 2; i++) pthread_join(wthreads[i], NULL);

    fair_rwlock_destroy(&lock);
    return 0;
}

/*
🔹 Key Properties
No starvation: Readers and writers take turns strictly in ticket order.
Fairness: If a writer arrives before readers, it will execute before them.
Grouping readers: Consecutive readers can share access without interruption (efficient).
*/
