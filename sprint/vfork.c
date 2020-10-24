#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    int val=8;
    pid_t pid=vfork();
    if(pid<0){
        printf("error\n");
    }else if(pid==0){
        val++;
        printf("this is child!\n");
        exit(0);
    }else{
        printf("this is parents!\n");
        printf("val=%d",val);
    }
    printf("----------\n");
    return 0; 
}
