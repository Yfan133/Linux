#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
void fun()
{
    printf("---------------\n");
    exit(0);
    printf("***************\n");
}
int main()
{
    printf("@@@@@@@@@@@@@@@\n");
    printf("000000000000000");
    sleep(2);
    fun();
    printf("###############\n");
    return 0; 
}
