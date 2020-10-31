#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    pid_t pid=fork();
    if(pid<0){
        perror("pid");
        exit(0);
    }else if(pid==0){
        printf("this is child!\n");
        sleep(3);
        execl("./test",NULL,NULL);
        printf("666\n");
        exit(0);

    }else if(pid>0)
    {
        wait(NULL);
        while(1){
            printf("正在打麻将~~\n");
            sleep(1);
        }

    }
    return 0;
}

