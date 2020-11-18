#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#define SCALS 4         //四个黄牛
int g_val=100;          //100张票
pthread_mutex_t lock;   //互斥锁

void* thread_start(void* arg)       //抢票程序
{
    (void)arg;                      //不使用这个arg时把他void一下就行，防止报警告
    while(1)                        
    {
        pthread_mutex_lock(&lock);  //加锁
        if(g_val>0)
        {
            printf("i am scaplers:%p,i get tickes:%d\n",pthread_self(),g_val);
            g_val--;                //抢到票则g_val--
            
        }
        else
        {
            //pthread_mutex_unlock(&lock);    //这个位置和return NULL位置解锁的效果是一样的,这里不解锁的话，最后一次拿到锁的线程直接退出，会造成其它3个线程一直卡在pthread_mutex_lock处，无法加锁。
            break;                          //票没有了就退出循环
        }
        pthread_mutex_unlock(&lock);      //这里不解锁的话，拿到第100张票之后会继续循环，自己卡住了自己，死锁！
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main()
{
    pthread_mutex_init(&lock,NULL);
    pthread_t tid[SCALS];
    for(int i=0;i<4;i++)
    {
        int ret=pthread_create(&tid[i],NULL,thread_start,NULL);
        if(ret<0)
        {
            perror("pthread_create");
            return -1;
        }
    }
    for(int i=0;i<4;i++)
    {
        pthread_join(tid[i],NULL);  //回收线程并清理线程退出资源
    }
    pthread_mutex_destroy(&lock);   //如果使用完毕不销毁锁，会造成内存泄露，锁的本质是什么？
    return 0;
}
