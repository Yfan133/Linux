#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
typedef struct mystruct
{
    int data;
    mystruct()          //构造函数
    {
        data=-1;
    }
}MYSTRUCT;

void* thread_start(void* arg)
{
    MYSTRUCT* ti=(MYSTRUCT*)arg;   //新创建一个mystruct*接收强转后的arg
    while(1)
    {
        printf("i am new thread!%d\n",ti->data);    //会看到抢占式执行的结果
        sleep(1);
    }
    delete ti;
    return NULL;
}

int main()
{
    pthread_t tid;                      //线程标识符
    int i=0;
    for(i;i<4;i++)                      //创建4个线程
    {
        MYSTRUCT* myinfo=new mystruct;  //在堆上开辟一段空间
        myinfo->data=i;
        int ret=pthread_create(&tid,NULL,thread_start,(void*)myinfo);   //创建线程
        if(ret<0)
        {
            perror("pthread_create");
            return -1;
        }
    }
    while(1)
    {
        printf("i am main thread\n");
        sleep(1);
    }
    return 0;
}
