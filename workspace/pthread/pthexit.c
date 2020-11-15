#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
void* thread_start(void* arg)
{
    //pthread_exit(NULL);
    pthread_cancel(pthread_self());    //为什么没有直接退出工作线程而是还打印了一次,因为pthread_cancel这个接口不会立即退出，而是会有延迟，然后退出,这个延迟不会超过1s
    while(1)
    {
        printf("i am new thread!\n");
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t tid;
    int ret=pthread_create(&tid,NULL,thread_start,NULL);    //第一个参数是线程标识符一般都要传,第二个参数一般为NULL，第三个参数线程执行什么函数，最后一个参数如果没有也可以穿NULL。
    if(ret<0)
    {
        perror("pthread_create");
        return -1;
    }
    //sleep(5);
    //pthread_exit(NULL);     //一般都是NULL，出参可以保存退出信息,这条语句会导致主线程变成僵尸线程
    //pthread_cancel(tid);      //通过线程标识符结束指定的线程,这里结束的是所创建的工作线程
    while(1)
    {
        printf("i am main thread\n");
        sleep(1);
    }
    return 0;
}
