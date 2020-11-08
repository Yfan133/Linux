#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void fun(int signo)
{
    printf("i am signal:%d\n",signo);
}
int main()
{
    signal(SIGINT,fun);     //修改2号信号处理方式
    signal(SIGSEGV,fun);    //修改11号信号处理方式
    while(1)
    {
        printf("this is linux\n");
        sleep(3);
    }
    return 0;
}
