#include<stdio.h>
#include<iostream>
#include<vector>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>

#define THREADCOUNT 4

class RingQueue
{
private:
  std::vector<int> vec_;
  size_t Capacity_;
  sem_t lock_;
  sem_t pro_;
  sem_t con_;
  int WritePos;
  int ReadPos;
public:
  RingQueue(size_t SIZE)
    :vec_(SIZE)
    ,Capacity_(SIZE)
  {
    sem_init(&lock_,0,1);  //第二个参数属性一般为0，第三个参数初始化数量
    sem_init(&pro_,0,SIZE);
    sem_init(&con_,0,0);
    WritePos=0;
    WritePos=0;
  }
  ~RingQueue()
  {
    sem_destroy(&lock_);
    sem_destroy(&pro_);
    sem_destroy(&con_);
  }
  void Push(int& data)
  {
    sem_wait(&pro_);
    sem_wait(&lock_);
    vec_[WritePos]=data;
    WritePos=(WritePos+1)%Capacity_;
    sem_post(&lock_);
    sem_post(&con_);
  }
  void Pop(int* data)
  {
    sem_wait(&con_);
    sem_wait(&lock_);
    *data=vec_[ReadPos];
    ReadPos=(ReadPos+1)%Capacity_;
    sem_post(&lock_);
    sem_post(&pro_);
  }
};
void* ProStart(void*arg)
{
  RingQueue* rq=(RingQueue*)arg;
  int data=0;
  while(1)
  {
    rq->Push(data);
    printf("pro:[%d]~~~\n",data);
    data++;
    sleep(1);
  }
  return NULL;
}
void* ConStart(void*arg)
{
  RingQueue* rq=(RingQueue*)arg;
  int data;
  while(1)
  {
    rq->Pop(&data);
    printf("---con:[%d]---\n",data);
    sleep(1);
  }
  return NULL;
}
int main()
{
  RingQueue* rq=new RingQueue(4);
  pthread_t pro_tid[THREADCOUNT];
  pthread_t con_tid[THREADCOUNT];
  for(int i=0;i<THREADCOUNT;i++)
  {
    pthread_create(&pro_tid[i],NULL,ProStart,(void*)rq);
    pthread_create(&con_tid[i],NULL,ConStart,(void*)rq);
  }
  for(int i=0;i<THREADCOUNT;i++)
  {
    pthread_join(pro_tid[i],NULL);
    pthread_join(con_tid[i],NULL);
  }
  delete rq;
  rq=NULL;
  return 0;
}
