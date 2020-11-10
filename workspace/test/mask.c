#include<stdio.h>
#include<unistd.h>
#include<signal.h>
void func(int signo)
{
    printf("i am signal:%d\n",signo);
}
int main()
{
    signal(SIGINT,func);
    signal(40,func);

    sigset_t newset;            //创建一个新的位图,用于设置所有信号的阻塞，阻塞不代表不会执行，只是保存在未决位图和sigqueue队列中，暂时不会执行
    sigfillset(&newset);         //把位图所有比特位置1，表示全阻塞

    sigset_t oldset;            //创建一个位图，用于保存原始位图

    sigprocmask(SIG_SETMASK,&newset,&oldset);   //设置所有信号阻塞，并且把原始位图保存在oldset中
    getchar();                  //用于阻塞进程，让我们给进程发送信号
    sigprocmask(SIG_SETMASK,&oldset,NULL);      //恢复原始设置
    while(1)
    {
        printf("this is linux!\n");
        sleep(3);
    }
    return 0;
}
