#include<stdio.h>
#include <fcntl.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    int fd=open("./Linux",O_RDWR | O_CREAT,0664);
    dup2(fd,1);     //也就是把当前进程标准输出修改成了fd的路径(fd的路径：指向当前目录下的Linux文件)
    printf("fd:%d\n",fd);
    printf("正在学习~~\n");
   while(1)
   {
       sleep(1);
   } 
    close(fd);
    return 0;
}
