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
        return 0;
    }else{
        while(1)
        {
            printf("this is parents!\n");
            sleep(1);
        }
    }
    while(1)
    {
        printf("----------\n");
        sleep(1);
    }
    return 0; 
}
