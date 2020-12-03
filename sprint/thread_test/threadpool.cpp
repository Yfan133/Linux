#include<iostream>
#include<stdio.h>
#include<pthread.h>
#include<queue>
#include<unistd.h>

#define  THREADCOUNT 4
typedef void (*Handler)(int);

class ThreadTask
{
  private:
    Handler handler_;
    int data_;
  public:
    ThreadTask(Handler handler,int data)
      :handler_(handler)
       ,data_(data)
    {}
    ~ThreadTask()
    {}
    void ThreadRun()
    {
      handler_(data_);
    }
};
class ThreadPool
{
  private:
    std::queue<ThreadTask*> que_;
    size_t Capacity_;
    pthread_t tid[THREADCOUNT];
    pthread_mutex_t lock_;
    pthread_cond_t con_;        //这里只需要消费者队列，因为从网络来的数据是无法预测的
    bool flag;
  public:
    ThreadPool(size_t SIZE)
      :Capacity_(SIZE)
    {
      pthread_mutex_init(&lock_,NULL);
      pthread_cond_init(&con_,NULL);
      flag=false;
    }
    ~ThreadPool()
    {
      pthread_mutex_destroy(&lock_);
      pthread_cond_destroy(&con_);
      for(int i=0;i<THREADCOUNT;i++)
        pthread_join(tid[i],NULL);
    }
    bool Push(ThreadTask* tt)
    {
      pthread_mutex_lock(&lock_);
      que_.push(tt);
      pthread_mutex_unlock(&lock_);
      pthread_cond_signal(&con_);
      return true;
    }
    bool Pop(ThreadTask** tt)
    {
      pthread_mutex_lock(&lock_);     //一会处理退出的问题
      while(que_.empty())
      {
        if(flag)
        {
          Capacity_--;
          pthread_mutex_unlock(&lock_);
          pthread_exit(NULL);
        }
        pthread_cond_wait(&con_,&lock_);
      }
      *tt=que_.front();
      que_.pop();
      pthread_mutex_unlock(&lock_);
      return true;
    }
    bool ThreadCreate()
    {
      for(int i=0;i<THREADCOUNT;i++)
      {
        if(0>pthread_create(&tid[i],NULL,ThreadRun,(void*)this))
        {
          perror("pthread_create");
          return false;
        }
      }
      return true;
    }
    static void* ThreadRun(void* arg)
    {
      ThreadPool* tp=(ThreadPool*)arg;
      while(1)
      {
        ThreadTask* tt=NULL;
        tp->Pop(&tt);
        tt->ThreadRun();
        delete tt;
      }
    }
    void ThreadExit()
    {
      pthread_mutex_lock(&lock_);
      flag=true;
      pthread_mutex_unlock(&lock_);
      pthread_cond_broadcast(&con_);  //这里一定得通知所有线程出队
    }
};
void Run(int data)
{
  printf("~~~[%d]\n",data);
}
int main()
{
  ThreadPool* tp=new ThreadPool(4);
  tp->ThreadCreate();
  for(int i=1;i<=100;++i)
  {
    ThreadTask* tt=new ThreadTask(Run,i);
    tp->Push(tt);
  }
  sleep(10);
  tp->ThreadExit();
  delete tp;
  tp=NULL;
  return 0;
}
