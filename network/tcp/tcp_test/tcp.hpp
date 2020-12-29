#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <string>

class TcpSock
{
  private:
    int sockfd_;
  public:
    TcpSock()
    {
      sockfd_ = -1;
    }
    void Close()
    {
      close(sockfd_);
      sockfd_ = -1;
    }
    //创建侦听套接字
    bool SockCreate() 
    {
      sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
      if(sockfd_ < 0)
      {
        perror("socket");
        return false;
      }
      return true;
    }
    //绑定地址信息
    bool Bind(std::string IP, uint16_t port)
    {
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(IP.c_str()); 
      int ret = bind(sockfd_, (struct sockaddr*)&addr, sizeof(addr));
      if(ret < 0)
      {
        perror("bind");
        return false;
      }
      return true;
    }
    //侦听，告诉操作系统内核可以建立连接了，三次握手
    bool Listen(int backlog = 1)
    {
      int ret = listen(sockfd_, backlog); //backlog是已完成队列大小，当队列被打满时可能会丢失也可能不会丢失
      if(ret < 0)
      {
        perror("listen");
        return false;
      }
      return true;
    }
    //获取连接
    //这里的peeraddr 客户端地址是出参
    //用TcpSock实例化指针里的fd，接受accpet返回的new_socket
    bool Accept(struct sockaddr_in* peeraddr, TcpSock* ts)
    {
      socklen_t addrlen = sizeof(struct sockaddr_in);
      int new_socket = accept(sockfd_, (struct sockaddr*)peeraddr, &addrlen);
      if(new_socket < 0)
      {
        perror("accept");
        return false;
      }
      ts->sockfd_ = new_socket;
      return true;
    } 
    //客户端发起连接
    bool Connect(std::string& ip, uint16_t port)
    {
      struct sockaddr_in destaddr;
      destaddr.sin_family = AF_INET;
      destaddr.sin_port = htons(port);
      destaddr.sin_addr.s_addr = inet_addr(ip.c_str());
      int ret = connect(sockfd_, (struct sockaddr*)& destaddr, sizeof(struct sockaddr_in));
      if(ret < 0)
      {
        perror("connect");
        return false;
      }
      return true;
    }
    //发送数据
    bool Send(std::string& data)  //入参
    {
      int sendsize = send(sockfd_, data.c_str(), data.size(), 0);
      if(sendsize < 0)
      {
        perror("send");
        return false;
      }
      return true;
    }
    //接受数据
    bool Recv(std::string* data)  //出参
    {
      char buf[1024] = { 0 };
      int recvsize = recv(sockfd_, buf, sizeof(buf) - 1, 0);
      if(recvsize < 0)
      {
        perror("recv");
        return false;
      }
     (*data).assign(buf, recvsize);
      return true;
    }
};
