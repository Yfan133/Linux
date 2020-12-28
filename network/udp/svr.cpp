#include "udp.hpp"

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    printf("using ./svr [ip] [port]\n");
    return -1;

  }
  UdpSocket us;
  us.Socket();
  std::string ip(argv[1]);
  uint16_t port = atoi(argv[2]);
  us.Bind(ip, port);
  struct sockaddr_in addr;
  std::string data;
  while(1)
  {
    us.Recv(&data, &addr);
    std::cout << "client say : " << data << std::endl;
    data.clear();
    std::cout << "please enter msg to client : ";
    fflush(stdout);
    std::cin >> data;
    us.Sendto(data, &addr);
    data.clear();
  }
  return 0;
}
