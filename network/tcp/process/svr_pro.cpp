#include "../tcp.hpp"
#include <signal.h>
#include <sys/wait.h>

#define CHECK_RET(p) if (p == false){return 0;}  //定义一个宏用于检查返回值

void sig(int signo)
{
  (void)signo;
  wait(NULL);
}
int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    printf("Start Server: ./svr [ip] [port]\n");
    return 0;
  }
  signal(SIGCHLD, sig);

  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  TcpSock ts;    //实例化对象用于创建监听套接字，接收新连接(侦听套接字)
  CHECK_RET(ts.SockCreate()); 
  CHECK_RET(ts.Bind(ip, port)); 
  CHECK_RET(ts.Listen()); 

  while (1)
  {
    TcpSock ser_ts; //实例化对象用于进行1v1服务(new_socket)
    struct sockaddr_in clientaddr;
    //父进程就一直去accept，接收新的连接，返回创建出的new_socket
    CHECK_RET(ts.Accept(&clientaddr, &ser_ts));
    printf("Have a new connection ip=%s, port=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    int pid = fork();
    if (pid == 0)  //子进程
    {
      while (1)
      {
        std::string buf;
        ser_ts.Recv(&buf);  //接受对端发送的消息
        printf("Client:%d say: %s\n",ntohs(clientaddr.sin_port), buf.c_str());

        printf("please send msg to client: ");
        fflush(stdout);
        std::cin >> buf;
        ser_ts.Send(buf);
      }
      ser_ts.Close();
    }
    else if (pid < 0) //错误
    {
      perror("fork");
      return -1;
    }
    else
    {
      //父进程关闭新创建的哪个socket，接收新socket
      ser_ts.Close();
    }
  }
  return 0;
}
