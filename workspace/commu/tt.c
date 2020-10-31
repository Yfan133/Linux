#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
int main()
{
    int fd[2];
    int ret=pipe(fd);
    if(ret<0)
    {
        perror("pipe");
        return -1;
    }
    printf("fd[0]=%d,fd[1]=%d\n",fd[0],fd[1]);
    int flag = fcntl(fd[1], F_GETFL);           //获取文件描述符
    printf("flag-fd[1] : %d\n", flag);
    fcntl(fd[0], F_SETFL, flag | O_NONBLOCK);   //将写端文件描述符设置成非阻塞属性
    pid_t pid =fork();
    if(pid<0)
    {
        perror("fork");
        return -1;
    }
    else if(pid==0)
    {
        while(1)
        {
            printf("this is child\n");
            char buf[1024]={0};
            int p=read(fd[0],buf,sizeof(buf)-1);    //注意read和write为阻塞接口，因此读端要设置成非阻塞，才能一直读
            printf("p=%d,buf:%s\n",p,buf);
            sleep(3);
        }
    }
    else
    {
        printf("this is parents\n");
        write(fd[1],"linux",5);
    }
    while(1)
    {
        sleep(1);
    }
    return 0;
}
