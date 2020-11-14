#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void fun(int signo)
{
    printf("i am signal:%d\n",signo);
}

int main()
{
    struct sigaction act;           //入参,act结构体
    sigemptyset(&act.sa_mask);      //把act结构体的位图sa_mask清空,表示没有收到任何信号
    act.sa_flags=0;                 //sa_flag=0表示调用sa_handler所指的函数
    act.sa_handler=fun;             //把sa_handler函数指针，指向fun函数

    struct sigaction oldact;        //这个结构体用于保存系统原始处理信号的逻辑

    sigaction(2,&act,&oldact);

    getchar();                      //getchar()是让程序阻塞，等我们生成一个信号，程序运行下面的代码

    sigaction(2,&oldact,NULL);      //恢复原始处理逻辑

    while(1)
    {
        printf("this is linux\n");
        sleep(3);
    }
    return 0;
}
