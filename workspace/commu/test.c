#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    pid_t pid = fork();
    if(pid<0)
    {
        perror("fork");
        return -1;
    }
    else if(pid==0)
    {
        while(1)
        {
            printf("this is child!\n");
            sleep(1);        
        }
    }

    else{
        while(1)
        {
            printf("this is father!\n");
            sleep(1);
        }
    }
    return 0;
}
