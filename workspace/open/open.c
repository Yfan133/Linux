#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
int main()
{
    char buf[]="Linux-57";
    int fd =open("./Linux.txt",O_RDWR|O_CREAT,0664);
    if(fd<0){
        printf("open file fail!\n");
        _exit(0);
    }
    printf("fd:%d\n",fd);
    write(fd,buf,sizeof(buf));
    memset(buf,0,sizeof(buf));
    lseek(fd,0,SEEK_SET);
    size_t size=read(fd,buf,sizeof(buf));
    printf("%d,%s\n",size,buf);
    close(fd);
    return 0;
}
