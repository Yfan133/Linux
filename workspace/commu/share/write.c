#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>
#define KEY 0x88888888
int main()
{
    int shmid = shmget(KEY,1024,IPC_CREAT|0664);//创建
    if(shmid<0)
    {
        perror("shmget");
        return -1;
    }
    void* lp=shmat(shmid,NULL,0);//附加
    strcpy((char*)lp,"linux");
    //shmdt(lp);                  //分离
    //shmctl(shmid,IPC_RMID,NULL);//删除
    while(1)
    {
        printf("正在打麻将~\n");
        sleep(3);
    }
    return 0;
}
