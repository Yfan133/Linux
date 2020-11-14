#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
//typedef struct mystruct
//{
//    int data;
//    mystruct()          //构造函数
//    {
//        data=-1;
//    }
//}MYSTRUCT;

void* thread_start(void* arg)
{
    int* ti=(int*)arg;
    while(1)
    {
        printf("i am new thread!%d\n",*ti);
        //(*ti)++;
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t tid;
    int i=0;
    for(;i<4;i++)
    {
        int ret=pthread_create(&tid,NULL,thread_start,(void*)&i);
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
