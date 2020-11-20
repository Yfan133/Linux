#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#define THREADCOUNT 10
pthread_mutex_t lock;   //互斥锁
pthread_cond_t producter_cond;  //生产者队列
pthread_cond_t consumer_cond;   //消费者队列

int g_val=0;    //临界资源

void* producter_thread(void* arg)  //生产者入口函数
{
    (void)arg;
    while(1)    //做面
    {
        pthread_mutex_lock(&lock);      //上锁,成功则访问临界资源
        while(g_val>0)
        {
            pthread_cond_wait(&producter_cond,&lock);    //入生产者队列等待
        }
        g_val++;
        //printf("i am producter:%p，i made noodles：%d\n",pthread_self(),g_val);
        printf("i am producter:%p\n",pthread_self());
        pthread_mutex_unlock(&lock);    //解锁
        pthread_cond_signal(&consumer_cond); //唤醒消费者
    }
    return NULL;
}
void* consumer_thread(void* arg)  //消费者入口函数
{
    (void)arg;
    while(1)    //吃面
    {
        pthread_mutex_lock(&lock);      //上锁,成功则访问临界资源
        while(g_val==0)
        {
            pthread_cond_wait(&consumer_cond,&lock);    //入消费者队列等待
        }
        g_val--;
        //printf("i am consumer:%p，i ate noodles：%d\n",pthread_self(),g_val);
        pthread_mutex_unlock(&lock);    //解锁
        pthread_cond_signal(&producter_cond); //唤醒生产者
    }
}
int main()
{
    pthread_t consumer[THREADCOUNT];
    pthread_t producter[THREADCOUNT];
    pthread_mutex_init(&lock,NULL);     //初始化锁资源
    pthread_cond_init(&consumer_cond,NULL); //初始化顾客队列
    pthread_cond_init(&producter_cond,NULL); //初始化生产者队列
    for(int i=0;i<THREADCOUNT;i++)
    {
        if(pthread_create(&consumer[i],NULL,consumer_thread,NULL)<0)
        {
            perror("pthread_create");
            return -1;
        }
    }
    for(int i=0;i<THREADCOUNT;i++)
    {
        if(pthread_create(&producter[i],NULL,producter_thread,NULL)<0)
        {
            perror("pthread_create");
            return -1;
        }
    }
    for(int i=0;i<THREADCOUNT;i++)
    {
        pthread_join(consumer[i],NULL);
        pthread_join(producter[i],NULL);
    }
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&consumer_cond);
    pthread_cond_destroy(&producter_cond);
    return 0;
}
