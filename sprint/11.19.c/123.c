#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
pthread_mutex_t lock;
pthread_cond_t cond_1;
pthread_cond_t cond_2;
pthread_cond_t cond_3;

int g_val=1;

void* thread_1(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        while(g_val%3!=1)
        {
            pthread_cond_wait(&cond_1,&lock);
        }
        ++g_val;
        printf("one ");
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond_2);
    }
    return NULL;
}
void* thread_2(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        while(g_val%3!=2)
        {
            pthread_cond_wait(&cond_2,&lock);
        }
        ++g_val;
        printf("two ");
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond_3);
    }
    return NULL;
}
void* thread_3(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        while(g_val%3!=0)
        {
            pthread_cond_wait(&cond_3,&lock);
        }
        ++g_val;
        printf("three\n");
        sleep(1);
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond_1);
    }
    return NULL;
}
int main()
{
    pthread_t tid;
    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&cond_1,NULL);
    pthread_cond_init(&cond_2,NULL);
    pthread_cond_init(&cond_3,NULL);
    pthread_create(&tid,NULL,thread_1,NULL);
    pthread_create(&tid,NULL,thread_2,NULL);
    pthread_create(&tid,NULL,thread_3,NULL);
    pthread_join(tid,NULL);
    return 0;
}
