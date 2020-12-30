#include "tcp.hpp"

#define CHECK_RET(p) if (p == false){return 0;}  //定义一个宏用于检查返回值
int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    printf("Start Client: ./cli [ip] [port]\n");
    return 0;
  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  TcpSock ts;
  CHECK_RET(ts.SockCreate()); 
  CHECK_RET(ts.Connect(ip, port));  //阻塞接口，如果和服务端完成连接则接口返回

  while (1)
  {
    std::string buf;
    printf("Please send msg to server: ");
    fflush(stdout);
    std::cin >> buf;
    CHECK_RET(ts.Send(buf));
    int ret = ts.Recv(&buf);
    if(ret < 0)
    {
      ts.Close();
      return -1;
    }
    else if(ret == 0)
    {
      ts.Close();
      return 0;
    }
    printf("Client say: %s\n",buf.c_str());
  }
  return 0;
}
