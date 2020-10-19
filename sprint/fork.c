#include<stdio.h>
#include<unistd.h>
int main()
{
    pid_t pid=fork();   //creat a process
    if(pid>0){
        //parents process
        printf("this is parents!%d\n",getpid());
    }
    else if(pid==0){
        //child process
        printf("this is child!%d\n",getpid());
    }
    else{
        //creat error
        printf("error!\n");
    }
    printf("this is a process!\n");
    sleep(100);
    return 0;
}
