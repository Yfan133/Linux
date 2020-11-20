#include<iostream>
#include<unistd.h>
#include<semaphore.h>
#include<vector>
#include<cstdio>

#define SIZE 1
#define THREADCOUNT 2

class RingQueue //循环队列
{
    public:
        RingQueue()
            :Vec_(SIZE)
        {
            PosRead_=0;
            PosWrite_=0;
            Capacity_=SIZE;
            sem_init(&ConSem_,0,0); //消费者资源计数器初始化为0，代表当前无资源可用，等生产者用post接口发信号时，这里计数器会++
            sem_init(&ProSem_,0,Capacity_); //初始化资源和容量大小一致
            sem_init(&lock_,0,1);           //初始化资源为1，代表最多只能一个访问锁资源
        }
        ~RingQueue()
        {
            sem_destroy(&ConSem_);
            sem_destroy(&ProSem_);
            sem_destroy(&lock_);
        }
        void Push(int& Data)
        {
            sem_wait(&ProSem_); //生产者请求生产，如果资源计数器>0则表示资源可用
            sem_wait(&lock_);    //请求加锁，锁资源只能被一个执行流拿着

            Vec_[PosWrite_]=Data;   //这里不用进行while判断，因为wait的资源计数器已经帮我们计算好了
            PosWrite_=(PosWrite_+1)%Capacity_;  //循环队列

            sem_post(&lock_);    //"解锁"，资源计数器++，并且唤醒等待队列
            sem_post(&ConSem_); //计数器++，唤醒消费者等待队列
        }
        void Pop(int* Data)
        {
            sem_wait(&ConSem_); 
            sem_wait(&lock_);

            *Data = Vec_[PosRead_];
            PosRead_=(PosRead_+1)%Capacity_;  //循环队列

            sem_post(&lock_);
            sem_post(&ProSem_);
        }
    private:
        std::vector<int> Vec_;
        int PosRead_;
        int PosWrite_;
        size_t Capacity_;
        sem_t ConSem_;
        sem_t ProSem_;
        sem_t lock_;
};
void* thread_con(void* arg)
{
    RingQueue* rq=(RingQueue*)arg;
    int data;
    while(1)
    {
        rq->Pop(&data);         //这里bq是指针，要用->
        printf("Consumed：[%p][%d]\n",pthread_self(),data);
    }
    return NULL;
}
void* thread_pro(void* arg)
{
    RingQueue* rq=(RingQueue*)arg;
    int i=0;
    while(1)
    {
        rq->Push(i);         //这里bq是指针，要用->
        printf("Producted：[%p][%d]\n",pthread_self(),i);
        ++i;
    }
    return NULL;
}
int main()
{
    RingQueue* rq=new RingQueue;
    pthread_t con_tid[THREADCOUNT];
    pthread_t pro_tid[THREADCOUNT];
    int i=0;
    for(;i<THREADCOUNT;i++)
    {
        if(0>pthread_create(&pro_tid[i],NULL,thread_pro,(void*)rq))
        {
            perror("pthread_create");
            return -1;
        }
        if(0>pthread_create(&con_tid[i],NULL,thread_con,(void*)rq))
        {
            perror("pthread_create");
            return -1;
        }
    }
    for(i=0;i<THREADCOUNT;++i)
    {
        pthread_join(con_tid[i],NULL);
        pthread_join(pro_tid[i],NULL);
    }
    delete rq;
    rq=NULL;
    return 0;
}
