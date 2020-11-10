#include<stdio.h>
#include<unistd.h>
#include<signal.h>
void func(int signo)
{
    printf("i am signal:%d\n",signo);
}
int main()
{
    struct sigaction act;
    act.sa_flags=0;
    sigemptyset(&act.sa_mask);

    act.sa_handler=func;
    struct sigaction oldact;
    sigaction(2,&act,&oldact);
    
    getchar();
    
    sigaction(2,&oldact,NULL);

    while(1)
    {
        printf("this is linux!\n");
        sleep(3);
    }
    return 0;
}
