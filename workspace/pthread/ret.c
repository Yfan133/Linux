#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
void* thread_start(void* arg)       //自定义的线程入口函数
{
    int* ti=(int*)arg;   //新创建一个mystruct*接收强转后的arg
    while(1)
    {
        printf("i am new thread!%d\n",*ti);    //这里虽然没有直接崩溃，但是使用的还是临时变量，因此创建子进程循环结束之后i被释放，这里访问的是非法地址，没有崩溃是因为这段空间还没有被开辟。
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t tid;                      //线程标识符
    for(int i=0;i<4;i++)                      //创建4个线程
    {
        int ret=pthread_create(&tid,NULL,thread_start,(void*)&i);   //创建线程
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
