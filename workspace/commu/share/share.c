#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>
#define KEY 0x88888888
int main()
{
    int shmid = shmget(KEY,1024,IPC_CREAT|0664);
    if(shmid<0)
    {
        perror("shmget");
        return -1;
    }
    void* lp=shmat(shmid,NULL,0);
    strcpy((char*)lp,"linux");
    shmdt(lp);
    //shmctl(shmid,IPC_RMID,NULL);
    return 0;
}
