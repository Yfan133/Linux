#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
    int ret=mkfifo("./tube",0664);
    if(ret<0)
    {
        perror("mkfifo");
        return -1;
    }
    int fd=open("./tube",O_RDWR);
    write(fd,"Linux",5);
    char buf[1024]={0};
    read(fd,buf,sizeof(buf)-1);
    printf("buf:%s\n",buf);
    while(1)
    {
        sleep(1);
    }
    return 0;
}
