#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

pthread_t tid[2];
unsigned int shared_data = 1;
pthread_mutex_t mutex;
unsigned int rc;
//prototypes for callback functions

void* even(void*);
void* odd(void*);

int main(void)
{
    pthread_create(&tid[0], NULL, &even, 0);
    pthread_create(&tid[1], NULL, &odd, 0);
    sleep(3);

    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    printf("\n");
    return 0;
}

void* even(void *ptr)
{
    do
    {
       rc = pthread_mutex_lock(&mutex);
       if(shared_data%2 == 0)
       {
         printf("%d ",shared_data);
         shared_data++;
       } 
       rc=pthread_mutex_unlock(&mutex);//if number is odd, do not print, release mutex
    } while(shared_data < 100);
}

void* odd(void* ptr1)
{
    do
    {
       rc = pthread_mutex_lock(&mutex);
       if(shared_data%2 != 0)
       {
          printf("%d ",shared_data);
          shared_data++;
       }
       rc = pthread_mutex_unlock(&mutex);//if number is even, do not print, release mutex
    } while(shared_data < 100);
}
