#include<stdio.h>
#include<unistd.h>
#include<signal.h>
int g_val=1;
void func(int signo)
{
    g_val=0;
    //g_val--;
    printf("i am signal:%d,g_val=%d\n",signo,g_val);
}
int main()
{
    signal(2,func);
    while(g_val)
    {
        //printf("this is linux\n");    //这里刷新缓冲或sleep()CPU
        //sleep(1);
    }
    return 0;
}
