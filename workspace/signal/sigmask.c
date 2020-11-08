#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void fun(int signo)
{
    printf("i am signal:%d\n",signo);
}
int main()
{
    signal(SIGINT,fun);
    signal(40,fun);

    sigset_t set;
    sigfillset(&set);

    sigset_t oldset;
    sigprocmask(SIG_SETMASK,&set,&oldset);
    getchar();
    sigprocmask(SIG_SETMASK,&oldset,NULL);
    while(1)
    {
        printf("this is linux\n");
        sleep(3);
    }
    return 0;
}
