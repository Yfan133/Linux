#include<stdio.h>
#include<unistd.h>
#include<signal.h>
int g_val=20;
void func(int signo)
{
    g_val--;
    printf("i am signal:%d,g_val=%d\n",signo,g_val);
}
int main()
{
    signal(2,func);
    int i=1;
    while(g_val--)
    {
        printf("%d\n",i);
        i++;
        sleep(1);
    }
    return 0;
}
