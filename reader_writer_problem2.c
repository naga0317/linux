#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef enum { READER_PREF, WRITER_PREF } rw_policy_t;

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t can_read;
    pthread_cond_t can_write;

    int readers;          // active readers
    int writers;          // active writers (0 or 1)
    int waiting_writers;  // number of writers waiting
    rw_policy_t policy;   // scheduling policy
} rwlock_t;

// ------------------ LOCK IMPLEMENTATION ------------------ //
void rwlock_init(rwlock_t *l, rw_policy_t policy) {
    pthread_mutex_init(&l->mutex, NULL);
    pthread_cond_init(&l->can_read, NULL);
    pthread_cond_init(&l->can_write, NULL);
    l->readers = 0;
    l->writers = 0;
    l->waiting_writers = 0;
    l->policy = policy;
}

void rwlock_destroy(rwlock_t *l) {
    pthread_mutex_destroy(&l->mutex);
    pthread_cond_destroy(&l->can_read);
    pthread_cond_destroy(&l->can_write);
}

// Acquire read lock
void rwlock_acquire_read(rwlock_t *l) {
    pthread_mutex_lock(&l->mutex);

    if (l->policy == WRITER_PREF) {
        // Writers are prioritized
        while (l->writers > 0 || l->waiting_writers > 0) {
            pthread_cond_wait(&l->can_read, &l->mutex);
        }
    } else {
        // Reader preference: only block if writer active
        while (l->writers > 0) {
            pthread_cond_wait(&l->can_read, &l->mutex);
        }
    }

    l->readers++;
    pthread_mutex_unlock(&l->mutex);
}

// Release read lock
void rwlock_release_read(rwlock_t *l) {
    pthread_mutex_lock(&l->mutex);
    l->readers--;

    if (l->readers == 0) {
        pthread_cond_signal(&l->can_write); // wake writer if waiting
    }

    pthread_mutex_unlock(&l->mutex);
}

// Acquire write lock
void rwlock_acquire_write(rwlock_t *l) {
    pthread_mutex_lock(&l->mutex);
    l->waiting_writers++;

    while (l->writers > 0 || l->readers > 0) {
        pthread_cond_wait(&l->can_write, &l->mutex);
    }

    l->waiting_writers--;
    l->writers = 1;
    pthread_mutex_unlock(&l->mutex);
}

// Release write lock
void rwlock_release_write(rwlock_t *l) {
    pthread_mutex_lock(&l->mutex);
    l->writers = 0;

    if (l->policy == WRITER_PREF && l->waiting_writers > 0) {
        pthread_cond_signal(&l->can_write); // priority to writers
    } else {
        pthread_cond_broadcast(&l->can_read); // let readers in
        if (l->waiting_writers > 0) {
            pthread_cond_signal(&l->can_write);
        }
    }

    pthread_mutex_unlock(&l->mutex);
}

// ------------------ DEMO APPLICATION ------------------ //
rwlock_t lock;
int shared_data = 0;

void *reader(void *arg) {
    int id = *(int *)arg;
    while (1) {
        rwlock_acquire_read(&lock);
        printf("Reader %d: read shared_data = %d\n", id, shared_data);
        rwlock_release_read(&lock);
        usleep(100000); // 0.1s
    }
    return NULL;
}

void *writer(void *arg) {
    int id = *(int *)arg;
    while (1) {
        rwlock_acquire_write(&lock);
        shared_data++;
        printf("Writer %d: wrote shared_data = %d\n", id, shared_data);
        rwlock_release_write(&lock);
        usleep(250000); // 0.25s
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t rthreads[5], wthreads[2];
    int ids[5] = {1, 2, 3, 4, 5};
    int wids[2] = {1, 2};

    // Choose policy from command line
    rw_policy_t policy = WRITER_PREF;
    if (argc > 1 && argv[1][0] == 'R') {
        policy = READER_PREF;
        printf("Running with READER preference policy\n");
    } else {
        printf("Running with WRITER preference policy\n");
    }

    rwlock_init(&lock, policy);

    // Create readers
    for (int i = 0; i < 5; i++) {
        pthread_create(&rthreads[i], NULL, reader, &ids[i]);
    }

    // Create writers
    for (int i = 0; i < 2; i++) {
        pthread_create(&wthreads[i], NULL, writer, &wids[i]);
    }

    for (int i = 0; i < 5; i++) pthread_join(rthreads[i], NULL);
    for (int i = 0; i < 2; i++) pthread_join(wthreads[i], NULL);

    rwlock_destroy(&lock);
    return 0;
}

/*
Compile with: gcc rwlock.c -o rwlock -lpthread
Run with writer preference : ./rwlock
Run with reader preference: ./rwlock R

*/
