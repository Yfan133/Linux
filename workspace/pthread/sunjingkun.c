#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
void* thread_start(void* arg)
{
    printf("work:%d\n",getpid());
    while(1)
    {
        sleep(3);
    }
    return NULL;
}

int main()
{
    pthread_t tid;
    pthread_detach(tid);
    int ret=pthread_create(&tid,NULL,thread_start,NULL);    //第一个参数是线程标识符一般都要传,第二个参数一般为NULL，第三个参数线程执行什么函数，最后一个参数如果没有也可以穿NULL。
    if(ret<0)
    {
        perror("pthread_create");
        return -1;
    }
    printf("main:%d\n",getpid());
    while(1)
    {
        sleep(3);
    }
    sleep(2);
    return 0;
}
