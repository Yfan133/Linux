#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<queue>
#include<stdio.h>

#define THREADCOUNT 4

typedef void* (*Handler_t)(int);
class ThreadTask
{
    public:
        ThreadTask(Handler_t handler,int data)
            :handler_(handler)
             ,data_(data)
        {}
        ~ThreadTask()
        {

        }
        void Run()
        {
            handler_(data_);
        }
    private:
        Handler_t handler_; //函数指针，返回值为void*，参数为int
        int data_;
};
class ThreadPool
{
    public:

    private:
        static void* ThreadStart(void* arg) //这里是静态成员函数，原因是线程的入口函数参数必须是void*，因此不能有隐藏的this指针，而static修饰的成员函数没有。
        {
            ThreadPool* tp=(ThreadTask*)arg;
            while(1)
            {
                pthread_mutex_lock(&tp->lock_);
                ThreadTask* tt;
                while(tp->que_.empty())
                {
                    if(tp->IsExit())
                    {
                        tp->cur_thread_count_--;
                        pthread_mutex_unlock(&tp->lock_);
                        pthread_exit(NULL);
                    }
                    pthread_cond_wait(&tp->con_,&tp->lock_);
                }
                tp->Pop(&tt);   //tt是出参
                pthread_mutex_unlock(&tp->lock_);
                tt->Run();
                delete tt;
            }
        }
        bool ThreadCreate()     //隐藏this指针
        {
            for(int i=0;i<THREADCOUNT;i++)
            {
                if(0>pthread_create(&tid[i],NULL,ThreadStart,(void*)this))  //传入隐藏的this指针，this里有队列
                {
                    perror("pthread_create");
                    return false;
                }
            }
            return true;
        }
    private:
        std::queue<ThreadTask*> que_;   //队列，队内元素类型是ThreadTask类
        size_t capacity_;    //容量
        
        pthread_mutex_t lock_;  //互斥锁
        pthread_cond_t con_;    //消费者条件变量
        size_t thread_capacity_;    //线程池初始化的时候线程数量
        size_t cur_thread_count_;   //当前还有多少线程在运行
        pthread_t tid[THREADCOUNT]; //线程标识符
        bool IsExit;        //当前是否可以退出
};
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
