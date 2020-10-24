#include<stdio.h>
#include<unistd.h>
int main()
{
    pid_t pid=vfork();
    if(pid<0){
        printf("error\n");
    }else if(pid==0){
        printf("this is child!\n");
        sleep(3);
    }else{
        printf("this is parents!\n");
    }
    printf("----------\n");
    return 0; 
}
