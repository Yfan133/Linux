#include "../tcp.hpp"
#include <pthread.h>

#define CHECK_RET(p) if (p == false){return 0;}  //定义一个宏用于检查返回值

void* ThreadStart(void* arg)
{
  //分离自己的属性
  pthread_detach(pthread_self());
  TcpSock *ser_ts = (TcpSock*)arg;
  while(1)
  {
    std::string buf;
    ser_ts->Recv(&buf);  //接受对端发送的消息
    printf("Client say: %s\n",buf.c_str());

    printf("please send msg to client: ");
    fflush(stdout);
    std::cin >> buf;
    ser_ts->Send(buf);
  }
  ser_ts->Close();
  delete ser_ts;
}

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    printf("Start Server: ./svr [ip] [port]\n");
    return 0;
  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  TcpSock ts;    //实例化对象用于创建监听套接字，接收新连接(侦听套接字)
  CHECK_RET(ts.SockCreate()); 
  CHECK_RET(ts.Bind(ip, port)); 
  CHECK_RET(ts.Listen()); 

  while (1)
  {
    TcpSock* ser_ts = new TcpSock(); //实例化对象用于进行1v1服务(new_socket)
    struct sockaddr_in clientaddr;
    //主线程只负责accep
    CHECK_RET(ts.Accept(&clientaddr, ser_ts));
    printf("Have a new connection ip=%s, port=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, ThreadStart, (void*)ser_ts);
    if(ret < 0)
    {
      perror("pthread_create");
      return -1;
    }
  }
  return 0;
}
