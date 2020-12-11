#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <stdlib.h>

class UdpSocket
{
private:
  int sockfd_;
public:
  UdpSocket()
  {
    sockfd_ = -1;
  }
  ~UdpSocket()
  {
    close(sockfd_);
  }
  int Socket()
  {
    sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);     //设置网络层使用ipv4版本的协议
    if(sockfd_ < 0)
    {
      perror("socket");
      return -1;
    }
    return 0;
  }
  int Bind(std::string ip, uint16_t port)         //入参
  {
    struct sockaddr_in addr;        
    addr.sin_family = AF_INET;        //设置地址域，使用ipv4
    addr.sin_port = htons(port);      //设置端口号
    addr.sin_addr.s_addr = inet_addr(ip.c_str()); //设置ip地址
    int ret = bind(sockfd_, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
      perror("bind");
      return -1;
    }
    return 0;
  }
  int Recv(std::string* data, struct sockaddr_in* peer_addr)   //出参
  {
    char buf[1024] = { 0 };
    socklen_t peer_addr_len = sizeof(peer_addr);    //这里一定要初始化，不然内核不知道该如何解析peer_addr，会默认给port值为65536
    ssize_t size = recvfrom(sockfd_, buf, sizeof(buf) - 1, 0, (struct sockaddr*)peer_addr, &peer_addr_len);
    data->append(buf, size);          //把buf内容给data
    if(size < 0)
    {
      perror("recvfrom");
      return -1;
    }
    return size;
  }
  bool Sendto(std::string data, struct sockaddr_in* dest_addr)
  {
    ssize_t size = sendto(sockfd_, data.c_str(), data.size(), 0, (struct sockaddr*)dest_addr, sizeof(sockaddr_in));
    if(size < 0)
    {
      perror("sendto");
      return -1;
    }
    return size;
  }
};

