#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void fun(int signo)
{
    printf("i am signal:%d\n",signo);
}
int main()
{
    struct sigaction act;           //入参act结构体
    sigemptyset(&act.sa_mask);      //把act结构体里表示收到哪种信号的位图，sa_mask清空
    act.sa_flags=0;                 //把act结构体里表示收到信号时调用什么函数，调用sa_handler所指的函数
    act.sa_handler=fun;             //把sa_handler函数指针，指向fun函数

    struct sigaction oldact;
    sigaction(2,&act,&oldact);

    getchar();            //getchar()是让程序阻塞，等我们生成一个信号，程序运行下面的代码

    sigaction(2,&oldact,NULL);

    while(1)
    {
        printf("this is linux\n");
        sleep(3);
    }
    return 0;
}
