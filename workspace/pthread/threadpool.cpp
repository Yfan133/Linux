#include<stdio.h>
#include<unistd.h>
#include<queue>
#include<pthread.h>
#include<iostream>

#define THREADCOUNT 4

typedef void* (*Handler_t)(int);  //函数指针类型

class ThreadTask      //
{
private:
  Handler_t handler_; //函数指针用于保存函数入口地址
  int data_;          //元素
public:
  ThreadTask(Handler_t handler,int data)
    :handler_(handler)
     ,data_(data)
  {}
  ~ThreadTask()
  {}
  void Run()
  {
    handler_(data_);
  }
};
class ThreadPool
{
  private:
    std::queue<ThreadTask*> que_; //定义一个队列用于保存数据和入口函数，队列元素类型是类指针
    size_t capacity_; //初始化队列容量
    size_t thread_capacity_;//初始化时线程数量
    size_t cur_thread_count_;//当前还有多少线程
    pthread_mutex_t lock_;
    pthread_cond_t con_;//消费者队列，不需要生产者队列，因为客户端的请求行为是不确定时间的
    pthread_t tid_[THREADCOUNT];
    bool IsExit;  //标志是否可以退出
  public:
    ThreadPool()
    {
      capacity_=10; //规定队列容量为10
      thread_capacity_=THREADCOUNT;
      pthread_mutex_init(&lock_,NULL);
      pthread_cond_init(&con_,NULL);
      if(!is_create)
      {
        printf("ThreadPool create fail\n");
        exit(1);
      }
      IsExit=false; //标志当前不能退出
      cur_thread_count_=THREADCOUNT;
    }
    ~ThreadPool()
    {}
    bool Push(ThreadTask* tt)
    {
      pthread_mutex_lock(&lock_);
      if(IsExit)
      {
        pthread_mutex_unlock(&lock_);
        return false;
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
    void ThreadExit() //如果直接退出可能还有线程
    {
        for(int i=0;i<THREADCOUNT;i++)
        {
          pthread_cancel(tid_[i]);  //调用这个接口可以结束任意线程
        }
    }
    void ThreadPoolClear()  //清理队列并设置退出位
    {
      pthread_mutex_lock(&lock_);
      IsExit=true;
      pthread_mutex_unlock(&lock_);
      if(cur_thread_count_>0)
      {
        pthread_cond_broadcast(&con_);
      }
    }
  private:
    static void* ThreadStart(void* arg)
    {
      ThreadPool* tp=(ThreadPool*)arg;
      while(1)
      {
        pthread_mutex_lock(&tp->lock_);
        ThreadTask* tt;
        while(tp->que_.empty())
        {
          if(tp->IsExit)
          {
            tp->cur_thread_count_--;
            pthread_mutex_unlock(&tp->lock_);
            pthread_exit(NULL);
          }
          pthread_cond_wait(&tp->con_,&tp->lock_);
        }
        tp->Pop(&tt); //这里传入了tt接收删除的队列节点
        pthread_mutex_unlock(&tp->lock_);
        tt->Run();
        delete tt;  //释放tt内存，防止泄露
      }
    }
    bool ThreadCreate
};
