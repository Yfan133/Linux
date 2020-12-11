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
  
  struct sockaddr_in dest_addr;
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(port);
  dest_addr.sin_addr.s_addr = inet_addr(ip.c_str());

  std::string data;
  while(1)
  {
    std::cout << "please enter msg to Server : ";
    fflush(stdout);
    std::cin >> data;
    us.Sendto(data, &dest_addr);
    data.clear();
    us.Recv(&data, &dest_addr);
    std::cout << "Server say : " << data << std::endl;
  }
  return 0;
}
