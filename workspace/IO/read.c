#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
int main()
{
    char buf[1024]={0};
    int s=read(0,buf,sizeof(buf));
    if(s > 0){
        buf[s] = 0;
        write(1, buf, strlen(buf));
        write(2, buf, strlen(buf));

    }
    return 0;

}

