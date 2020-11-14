#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void fun(int signo)
{
    printf("i am signal:%d\n",signo);
}
int main()
{
    signal(2,fun);         //重新定义2号信号的处理方式
    signal(40,fun);

    sigset_t set;          //用于设置阻塞位图
    sigfillset(&set);      //这里阻塞所有信号

    sigset_t oldset;       //用于保存原始阻塞位图
    sigprocmask(SIG_SETMASK,&set,&oldset);  //调用阻塞接口
    getchar();                              //阻塞进程等待我们发送信号，并输入回车之后继续运行
    sigprocmask(SIG_SETMASK,&oldset,NULL);  //恢复原始阻塞位图
    while(1)
    {
        printf("this is linux\n");
        sleep(3);
    }
    return 0;
}
