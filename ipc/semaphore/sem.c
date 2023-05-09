#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex;
int count=1;

void* even(void* arg)
{
    //wait
    sem_wait(&mutex);
    //printf("\nEntered thread\n");

    //critical section
    //sleep(4);

    //signal
    while(count<=100)
    {
        if(count%2==0)
         printf("%d ", count++);

        sem_post(&mutex);
    }
    //sem_post(&mutex);
}

void* odd(void* arg)
{
    //wait
    sem_wait(&mutex);
    //printf("\nEntered thread\n");

    //critical section
    //sleep(4);

    //signal
    while(count<=100)
    {
        if(count%2!=0)
         printf("%d ", count++);
	
        sem_post(&mutex);
    }
    //sem_post(&mutex);
}

int main()
{
    sem_init(&mutex, 0, 1);
    pthread_t t1,t2;
    pthread_create(&t1,NULL,odd,NULL);
    sleep(2);
    pthread_create(&t2,NULL,even,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    sem_destroy(&mutex);
    return 0;
}
