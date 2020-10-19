#include<stdio.h>
#include<unistd.h>
int main()
{
    pid_t pid=fork();
    if(pid>0)
        printf("this is parents%d\n",getpid());
    else if(pid==0)
    {
        printf("this is child%d\n",getpid());
       //exit(0);//子进程先于父进程退出，造成僵尸进程
    }
    else
        printf("error\n");
    sleep(100);
    printf("this is progress\n");
    return 0;
}
