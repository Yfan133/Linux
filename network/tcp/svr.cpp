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
  //accept在whilccept(&ce(1)外则只能和一个客户端聊
  CHECK_RET(ts.Accept(&clientaddr, &ser_ts));
  while (1)
  {
    std::string buf;
    int ret = ser_ts.Recv(&buf);  //接受对端发送的消息
    if(ret < 0)
    {
      ts.Close();
      return -1;
    }
    else if(ret == 0) //若返回值为0说明对端关闭了连接
    {
      ts.Close();
      ser_ts.Close();
      return 0;
    }
    printf("Client say: %s\n",buf.c_str());

    printf("please send msg to client: ");
    fflush(stdout);
    getline(std::cin, buf);
    //buf.append("\r\n");
    ser_ts.Send(buf);
  }
  return 0;
}
