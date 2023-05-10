/* This file provides basic function to synchronize multiple processes using
 * System V Semaphores.  It uses semaphore id 786 so please make sure none of
 * your benchmarks uses that ID. */

#ifndef SEM_HELPER_H
#define SEM_HELPER_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

static int SEM_ID = 786;

int get_semaphore ()
{
    int sem_id;

    sem_id = semget(SEM_ID, 1, IPC_CREAT | 0666);

    if (sem_id == -1) {
        perror("get_semaphore: semget");
        exit(1);
    }

    return sem_id;
}

int set_semaphore (int sem_id, int val)
{
    return semctl(sem_id, 0, SETVAL, val);
}

void decrement_semaphore (int sem_id)
{
    struct sembuf sem_op;

    sem_op.sem_num  = 0;
    sem_op.sem_op   = -1;
    sem_op.sem_flg = 0;

    semop(sem_id, &sem_op, 1);
}

void wait_semaphore (int sem_id)
{
    struct sembuf sem_op;

    sem_op.sem_num  = 0;
    sem_op.sem_op   = 0;
    sem_op.sem_flg = 0;

    semop(sem_id, &sem_op, 1);
}

#endif
