#include<stdio.h>
#include<unistd.h>
#include<queue>
#include<pthread.h>
#include<iostream>

#define THREADCOUNT 4

typedef void (*Handler_t)(int);  //函数指针类型

class ThreadTask      //队列元素类型
{
  private:
    Handler_t handler_; //保存处理数据的函数
    int data_;          //待处理的数据
  public:
    ThreadTask(Handler_t handler,int data)
      :handler_(handler)
       ,data_(data)
  {}
    ~ThreadTask()
    {}
    void Run()      //直接跑函数，处理数据。
    {
      handler_(data_);
    }
};
class ThreadPool
{
  private:
    std::queue<ThreadTask*> que_; //定义一个队列用于保存数据和处理函数，队列元素类型是类指针

    size_t capacity_;           //初始化队列容量,STL队列容量理想可以无限扩容，我们规定一下容量。
    size_t thread_capacity_;    //初始化时线程数量
    //size_t cur_thread_count_;   //当前还有多少线程

    pthread_mutex_t lock_;      //互斥锁
    pthread_cond_t con_;        //消费者队列，不需要生产者队列，因为客户端的请求行为是不确定时间的
    pthread_t tid_[THREADCOUNT];//线程标识符，工业上用vector动态创建。

    bool IsExit;                //退出标志位  
  public:
    ThreadPool(size_t capacity,size_t thread_capacity)
      :capacity_(capacity)
       ,thread_capacity_(thread_capacity)
  {
    pthread_mutex_init(&lock_,NULL);
    pthread_cond_init(&con_,NULL);

    IsExit=false;                   //标志当前不能退出
  }

    ~ThreadPool()
    {
      pthread_mutex_destroy(&lock_);
      pthread_cond_destroy(&con_);
    }

    bool Push(ThreadTask* tt) //这里tt是入参，push接口是临界区
    {
      pthread_mutex_lock(&lock_);   //这里没有生产者队列，因为来自网络的数据是无法预测的
      if(IsExit)    //如果线程已经退出则无法再进行插入
      {
        pthread_mutex_unlock(&lock_);
        return -1;
      }
      que_.push(tt);
      pthread_mutex_unlock(&lock_);
      pthread_cond_signal(&con_);
      return true;
    }

    bool Pop(ThreadTask** tt)
    {
      *tt=que_.front();
      que_.pop();
      return true;
    }

    void ThreadJoin()
    {
      for(int i=0;i<THREADCOUNT;i++)
        pthread_join(tid_[i],NULL);
    }

    void ThreadExit()
    {
      pthread_mutex_lock(&lock_);
      IsExit=true;
      pthread_mutex_unlock(&lock_);

      pthread_cond_broadcast(&con_);  
    }

    bool ThreadCreate()
    {
        for(int i=0;i<THREADCOUNT;i++)
        {
          if(0>pthread_create(&tid_[i],NULL,ThreadStart,(void*)this))
          {
            perror("pthread_create");
            return false;
          }
        }
        return true;
    }
  private:
    static void* ThreadStart(void* arg)
    {
      ThreadPool* tp=(ThreadPool*)arg;
      while(1)
      {
        pthread_mutex_lock(&tp->lock_);
        while(tp->que_.empty())
        {
          if(tp->IsExit)
          {
            tp->thread_capacity_--;
            pthread_mutex_unlock(&tp->lock_);
            pthread_exit(NULL);
          }
          pthread_cond_wait(&tp->con_,&tp->lock_);
        }
        ThreadTask* tt;
        tp->Pop(&tt);
        pthread_mutex_unlock(&tp->lock_);   //这里应该先解锁在进行处理数据，提高处理效率
        tt->Run();
        delete tt;
      }
    }
};
void DealDate(int data)
{
  printf("data:%d\n",data);
}
int main()
{
  ThreadPool* tp=new ThreadPool(4,THREADCOUNT);
  
  tp->ThreadCreate();
  
  for(int i=1;i<=100;i++)
  {
    ThreadTask* tt=new ThreadTask(DealDate,i);
    tp->Push(tt);
  }
  sleep(10);
  tp->ThreadExit();
  tp->ThreadJoin();
  delete tp;
  return 0;
}
