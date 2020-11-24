#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<queue>
#include<cstdio>

#define CAPACITY 1
#define THREADCOUNT 1

class BlockQueue
{
    public:
        BlockQueue(size_t Capacity=CAPACITY)
        {
            pthread_mutex_init(&lock_,NULL);
            pthread_cond_init(&procond_,NULL);
            pthread_cond_init(&comcond_,NULL);
            Capacity_=Capacity;
        }
        ~BlockQueue()
        {
            pthread_mutex_destroy(&lock_);
            pthread_cond_destroy(&procond_);
            pthread_cond_destroy(&comcond_);
        }
        void Push(int& Data)
        {
            pthread_mutex_lock(&lock_);
            while(IsFull())
            {
                pthread_cond_wait(&procond_,&lock_);
            }
            Queue_.push(Data);
            pthread_mutex_unlock(&lock_);
            pthread_cond_signal(&comcond_);
        }
        void Pop(int* Data)
        {
            pthread_mutex_lock(&lock_);
            while(Queue_.empty())
            {
                pthread_cond_wait(&comcond_,&lock_);
            }
            *Data=Queue_.front();
            Queue_.pop();
            pthread_mutex_unlock(&lock_);
            pthread_cond_signal(&procond_);
        }
    private:
        bool IsFull()
        {
            return Queue_.size()==Capacity_;
        }
    private:
        std::queue<int> Queue_;
        size_t Capacity_;
        pthread_mutex_t lock_;
        pthread_cond_t procond_;
        pthread_cond_t comcond_;
};
void* thread_com(void* arg)
{
    BlockQueue* bq=(BlockQueue*)arg;
    int data;
    while(1)
    {
        bq->Pop(&data);         //这里bq是指针，要用->
        printf("Consumed：[%p] [%d]\n",pthread_self(),data);
    }
    return NULL;
}
void* thread_pro(void* arg)
{
    BlockQueue* bq=(BlockQueue*)arg;
    int i=0;
    while(1)
    {
        bq->Push(i);         //这里bq是指针，要用->
        printf("Producted：[%p] [%d]\n",pthread_self(),i);
        ++i;
    }
    return NULL;
}
int main()
{
    BlockQueue* bq =new BlockQueue;
    pthread_t pro_tid[THREADCOUNT];
    pthread_t com_tid[THREADCOUNT];
    int i=0;
    for(;i<THREADCOUNT;i++)
    {
        if(0>pthread_create(&pro_tid[i],NULL,thread_pro,(void*)bq))
        {
            perror("pthread_create");
            return -1;
        }
        if(0>pthread_create(&com_tid[i],NULL,thread_com,(void*)bq))
        {
            perror("pthread_create");
            return -1;
        }
    }
    for(i=0;i<THREADCOUNT;++i)
    {
        pthread_join(com_tid[i],NULL);
        pthread_join(pro_tid[i],NULL);
    }
    delete bq;
    bq=NULL;
    return 0;
}
