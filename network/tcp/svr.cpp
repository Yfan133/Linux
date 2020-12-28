#include "tcp.hpp"

#define CHECK_RET(p) if (p == false){return 0;}  //定义一个宏用于检查返回值
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

  TcpSock ser_ts; //实例化对象用于进行1v1服务(new_socket)
  struct sockaddr_in clientaddr;
  //accept在while(1)外则只能和一个客户端聊
  while (1)
  {
    CHECK_RET(ts.Accept(&clientaddr, &ser_ts));
    std::string buf;
    ser_ts.Recv(&buf);  //接受对端发送的消息
    printf("Client say: %s\n",buf.c_str());

    printf("please send msg to client: ");
    fflush(stdout);
    std::cin >> buf;
    ser_ts.Send(buf);
  }
  return 0;
}
