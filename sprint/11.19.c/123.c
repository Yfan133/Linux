#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include<pthread.h>
pthread_mutex_t lock;
pthread_cond_t cond_1;
pthread_cond_t cond_2;
pthread_cond_t cond_3;

int g_val=1;
class Foo
{
    public:
        void one()
        {
            printf("one");
        }
        void two()
        {
            printf("two");
        }
        void three()
        {
            printf("three\n");
        }
};
void* thread_1(void* arg)
{
    Foo* ti=(Foo*)arg;
    while(1)
    {
        pthread_mutex_lock(&lock);
        while(g_val%3!=1)
        {
            pthread_cond_wait(&cond_1,&lock);
        }
        ++g_val;
        ti->one();
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond_2);
    }
    return NULL;
}
void* thread_2(void* arg)
{
    Foo* ti=(Foo*)arg;
    while(1)
    {
        pthread_mutex_lock(&lock);
        while(g_val%3!=2)
        {
            pthread_cond_wait(&cond_2,&lock);
        }
        ++g_val;
        ti->two();
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond_3);
    }
    return NULL;
}
void* thread_3(void* arg)
{
    Foo* ti=(Foo*)arg;
    while(1)
    {
        pthread_mutex_lock(&lock);
        while(g_val%3!=0)
        {
            pthread_cond_wait(&cond_3,&lock);
        }
        ++g_val;
        ti->three();
        sleep(1);
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond_1);
    }
    return NULL;
}
int main()
{
    pthread_t tid[3];
    Foo *fo=new Foo;
    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&cond_1,NULL);
    pthread_cond_init(&cond_2,NULL);
    pthread_cond_init(&cond_3,NULL);
    pthread_create(&tid[0],NULL,thread_1,(void*)fo);
    pthread_create(&tid[1],NULL,thread_2,(void*)fo);
    pthread_create(&tid[2],NULL,thread_3,(void*)fo);
    for(int i=0;i<3;i++)
    {
        pthread_join(tid[i],NULL);
    }
    return 0;
}
