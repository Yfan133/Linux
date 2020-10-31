#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
int main()
{
    int fd[2];
    int ret=pipe(fd);
    if(ret<0){
        perror("pipe");
        return -1;
    }
    printf("fd[0]=%d,fd[1]=%d\n",fd[0],fd[1]);
    




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
