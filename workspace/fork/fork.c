#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    int g_val=100;
    pid_t pid=fork();
    if(pid>0){
        printf("this is parents. %d-%d-g_val=%d\n",getpid(),&pid,g_val);
        getchar();
        return 0;
    }
    else if(pid==0)
    {
        g_val=200;
        sleep(50);
        printf("this is child. %d-%d-g_val=%d\n",getpid(),&pid,g_val);
        //exit(0);//子进程先于父进程退出，造成僵尸进程
    }
    else
        printf("error\n");
    //sleep(100);
    while(1)
    {
        printf("this is progress\n");
        sleep(10);
    }
    return 0;
}
