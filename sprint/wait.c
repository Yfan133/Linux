#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    pid_t pid=fork();
    if(pid==0){
        //this is child;
        sleep(5);
        exit(0);
    }else if(pid>0){
        //this is parent;
        wait(); //不加wait，子进程在退出之后就会成为僵尸进程
    }else{
        //error
    }
    while(1)
    {
        sleep(1);
        printf("正在打麻将~~\n");
    }
    return 0;
}
