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
    while(1)
    {
        printf("this is linux!\n");
        sleep(3);
    }
    return 0;
}
