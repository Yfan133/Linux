#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<queue>

#define THREADCOUNT 4

class BlockQueue
{
  private:
    std::queue<int> que_;
    size_t capacity_;
    pthread_mutex_t lock_;
    pthread_cond_t pro_;
    pthread_cond_t con_;
  public:
    BlockQueue(size_t SIZE)
      :capacity_(SIZE)
    {
      pthread_mutex_init(&lock_,NULL);
      pthread_cond_init(&pro_,NULL);
      pthread_cond_init(&con_,NULL);
    }
    ~BlockQueue()
    {
      pthread_mutex_destroy(&lock_);
      pthread_cond_destroy(&pro_);
      pthread_cond_destroy(&con_);
    }
    void Push(int& data)
    {
      pthread_mutex_lock(&lock_);
      while(IsFull())                   //这里一定是while！！！
      {
        pthread_cond_wait(&pro_,&lock_);
      }
      que_.push(data);
      pthread_mutex_unlock(&lock_);
      pthread_cond_signal(&pro_);
    }
    void Pop(int* data)
    {
      pthread_mutex_lock(&lock_);
      while(que_.empty())               //这里一定是while！！！
      {
        pthread_cond_wait(&con_,&lock_);
      }
      *data=que_.front();
      que_.pop();
      pthread_mutex_unlock(&lock_);
      pthread_cond_signal(&con_);
    }
private:
    bool IsFull()
    {
      return que_.size()==capacity_;
    }
};
void* ProStart(void* arg)
{
  BlockQueue* bq=(BlockQueue*)arg;
  int i=0;
  while(1)
  {
    bq->Push(i);
    printf("i am pro [%p], i producted [%d]\n",pthread_self(),i);
    i++;
  }
  return NULL;
}
void* ConStart(void* arg)
{
  BlockQueue* bq=(BlockQueue*)arg;
  int data;
  while(1)
  {
    bq->Pop(&data);
    printf("i am con [%p], i consume [%d]\n",pthread_self(),data);
  }
  return NULL;
}
int main()
{
  BlockQueue* bq=new BlockQueue(4);
  pthread_t pro_tid[THREADCOUNT];
  pthread_t con_tid[THREADCOUNT];
  for(int i=0;i<THREADCOUNT;i++)
  {
    if(0>pthread_create(&pro_tid[i],NULL,ProStart,(void*)bq))
    {
      perror("pthread_create");
      return -1;
    }
    if(0>pthread_create(&con_tid[i],NULL,ConStart,NULL))
    {
      perror("pthread_create");
      return -1;
    }
  }
  for(int i=0;i<THREADCOUNT;i++)
  {
    pthread_join(pro_tid[i],NULL);
    pthread_join(con_tid[i],NULL);
  }
  delete bq;
  bq=NULL;
  return 0;
}
