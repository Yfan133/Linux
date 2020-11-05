#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
    int fd=open("./tube",O_RDWR);
    int flag = fcntl(fd,F_GETFL);
    fcntl(fd,F_SETFL,flag | O_NONBLOCK);
    char buf[1024]={0};
    while(1)
    {
        int ret=read(fd,buf,sizeof(buf)-1);
        printf("ret=%d,buf:%s\n",ret,buf);
        memset(buf,0,sizeof(buf));
        sleep(1);
    }
    close(fd);
    return 0;
}
