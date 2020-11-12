#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
void* thread_start(void* arg)
{
    int* i=(int*)arg;   //新创建一个int*接收强转后的arg
    while(1)
    {
        printf("i am new thread!%d\n",*i);
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t tid;
    int i=0;
    for(i;i<4;i++)
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
