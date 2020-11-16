#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t lock_1;   //mutex
pthread_mutex_t lock_2;   //mutex

void* thread_a(void* arg)
{
    (void)arg;
    pthread_mutex_lock(&lock_1);
    sleep(1);
    pthread_mutex_lock(&lock_2);
    pthread_mutex_unlock(&lock_1);
    pthread_mutex_unlock(&lock_2);
    return NULL;
}
void* thread_b(void* arg)
{
    (void)arg;
    pthread_mutex_lock(&lock_2);
    sleep(1);
    pthread_mutex_lock(&lock_1);
    pthread_mutex_unlock(&lock_2);
    pthread_mutex_unlock(&lock_1);
    return NULL;
}
int main()
{
    pthread_t tid[2];
    pthread_mutex_init(&lock_1,NULL);
    pthread_mutex_init(&lock_2,NULL);

   if(pthread_create(&tid[0],NULL,thread_a,NULL)<0)
   {
       perror("pthread_create");
       return -1;
   }
   if(pthread_create(&tid[1],NULL,thread_b,NULL)<0)
   {
       perror("pthread_create");
       return -1;
   }
    int i=0;
    for(;i<2;i++)
    {
        pthread_join(tid[i],NULL);
    }
    pthread_mutex_destroy(&lock_1);
    pthread_mutex_destroy(&lock_2);
    while(1)
    {
        printf("this is linux\n");
    }
    return 0;
}
