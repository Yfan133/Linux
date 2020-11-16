#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t lock;   //定义一个锁资源
pthread_cond_t cond;    //定义一个条件变量
int noodles=0;

void* thread_a(void* arg)   //顾客
{
    (void)arg;
    while(1)        //循环吃面
    {
        pthread_mutex_lock(&lock);  //加锁
        if(noodles==0)
        {
            pthread_cond_wait(&cond,&lock); //如果没有面，则调用wait接口阻塞并入队，等待厨子做面。注意这里传了lock的地址，接口底层封装了解锁(phtread_mutex_unlock)和被唤醒时加锁(pthread_mutex_lock)的代码。
        }
        noodles--;
        sleep(1);
        printf("i am consumer,i ate noodles:%d\n",noodles);
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond); //已经吃完面了，给厨子发送一个做面信号，唤醒厨子做面
    }
    return NULL;
}
void* thread_b(void* arg)   //厨子
{
    (void)arg;
    while(1)
    {
        pthread_mutex_lock(&lock);  //加锁
        if(noodles==1)
        {
            pthread_cond_wait(&cond,&lock); //如果有面，则入队等待顾客吃面。
        }
        noodles++;
        sleep(1);
        printf("i am cooker,i made noodles:%d\n",noodles);
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond); //已经做好面了，给顾客发送信号，唤醒顾客吃面。
    }
    return NULL;
}

int main()
{
    pthread_t tid[2];   //定义两个线程标识符
    pthread_mutex_init(&lock,NULL); //初始化锁资源
    pthread_cond_init(&cond,NULL);  //初始化条件变量

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
    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&lock);
    while(1)
    {
        printf("this is linux\n");
    }
    return 0;
}
