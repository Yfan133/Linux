#include<stdio.h>
#include<unistd.h>
#include<signal.h>
void func(int signo)
{
    printf("i am signal:%d\n",signo);
    wait(NULL);
}
int main()
{
    signal(SIGCHLD,func);   //自定义SIGCHLD信号处理方式
    pid_t pid=fork();
    if(pid<0)
    {
        perror("fork");
        return -1;
    }
    else if(pid==0)
    {
        sleep(10);
        printf("this is child!886\n");
        //_exit(0);
        return 0;
    }
    else{
        while(1)
        {
            printf("this is father....!\n");
            sleep(1);
        }
    }
    return 0;
}
