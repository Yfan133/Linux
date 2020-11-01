#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>

#define KEY 0x88888888

int main()
{
    int shmid = shmget(KEY, 1024, IPC_CREAT | 0664);
    if(shmid < 0)
    {
        perror("shmget");
        return -1;

    }
    void* lp = shmat(shmid, NULL, 0);
    printf("%s\n", (char*)lp);
    printf("%s\n", (char*)lp);
   // printf("%s\n", (char*)lp);
   // printf("%s\n", (char*)lp);
   // printf("%s\n", (char*)lp);
   // printf("%s\n", (char*)lp);
   // printf("%s\n", (char*)lp);
   // printf("%s\n", (char*)lp);

    struct shmid_ds buf;
    shmctl(shmid, IPC_STAT, &buf);
    
    printf("shm size : %d\n", buf.shm_segsz);

    shmdt(lp);
    shmctl(shmid,IPC_RMID,NULL);
    while(1)
    {
        sleep(1);
        printf("正在打麻将~~\n");
    }
    return 0;
}
