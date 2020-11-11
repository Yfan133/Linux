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
    signal(SIGCHLD,func);
    pid_t pid=fork();
    if(pid<0)
    {
        perror("fork");
        return -1;
    }
    else if(pid==0)
    {
        sleep(3);
        printf("this is child!666666666666666666\n");
        _exit(0);
    }
    else{
        while(1)
        {
            printf("this is linux!\n");
            sleep(1);
        }
    }
    return 0;
}
