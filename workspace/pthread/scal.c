#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#define SCALS 4
int g_val=100;
void* thread_start(void* arg)
{
    (void)arg;       //不使用这个arg时把他void一下就行，防止报警告
    while(g_val>0)  //只要有票黄牛就一直抢票
    {
        printf("i am scaplers:%p,i get tickes:%d\n",pthread_self(),g_val);
        g_val--;    //抢到票则g_val--
    }
    return NULL;
}

int main()
{
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
    while(1)                        //抢票结束就会执行
    {
        printf("i am main thread\n");
        sleep(1);
    }
    return 0;
}
