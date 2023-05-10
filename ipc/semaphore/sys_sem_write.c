#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

union semun 
{                   
    /* Used in calls to semctl() */
    int  val;
    struct semid_ds *   buf;
    unsigned short *    array;
    #if defined(__linux__)
    struct seminfo *    __buf;
    #endif
};

#define MEM_SIZE 4096


int main(int argc, char *argv[])
{
    int semid, shmid;
    struct sembuf sop;
    char *addr;
    size_t len;


    shmid = shmget(IPC_PRIVATE, MEM_SIZE, IPC_CREAT | 0600);
    if (shmid == -1)
        errExit("shmget");

    semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if (semid == -1)
       errExit("semget");


    /* Attach shared memory into our address space and copy string
                  (including trailing null byte) into memory. */

    addr = shmat(shmid, NULL, 0);
    if (addr == (void *) -1)
       errExit("shmat");

    memcpy(addr, argv[3], len);

    /* Decrement semaphore to 0. */

    sop.sem_num = 0;
    sop.sem_op = -1;
    sop.sem_flg = 0;

    if (semop(semid, &sop, 1) == -1)
        errExit("semop");

    exit(EXIT_SUCCESS);
}
