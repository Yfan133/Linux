#include<stdio.h>
#include<iostream>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

class Udp
{
  private:
    int sockfd_;
  public:
    Udp()
    {
      sockfd_=-1;
    }
    ~Udp()
    {}
    bool CreateSock()   //创建套接字
    {
      sockfd_=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
      if(sockfd_<0)
      {
        perror("socket");
        return false;
      }
      return true;
    }
    bool Bind(const std::string& ip,uint16_t port)  //传进来一个字符串类型的ip，和无符号16位端
    {
      struct sockaddr_in addr;        //创建一个ipv4版本的结构体
      addr.sin_family=AF_INET;        //ipv4版本
      addr.sin_port=htons(port);      //把端口转换成网络字节序
      addr.sin_addr.s_addr=inet_addr(ip.c_str()); //1.转换成uint32位整数 2.把ip转换称网络字节序
      int ret=bind(sockfd_,(struct sockaddr*)&addr,sizeof(addr));
      if(ret<0)
      {
        perror("bind");
        return false;
      }
      return true;
    }
    bool Send(std::string& data,struct sockaddr_in* dest_addr)  //这里data是入参因此引用
    {
      int send_size=sendto(sockfd_,data.c_str(),data.size(),0,(struct sockaddr*)dest_addr,sizeof(struct sockaddr_in));
      if(send_size<0)
      {
        perror("sendto");
        return false;
      }
      return true;
    }
    bool Recv(std::string* data,struct sockaddr_in* src_addr=NULL)   //这里data出参，因此*
    {
      char buf[1024]={0}; //必须初始化，char buf[1034]="0";也可以
      ssize_t recv_size=-1;
      if(!src_addr)   //如果传进来的地址为空
      {
        recv_size=recvfrom(sockfd_,buf,sizeof(buf)-1,0,NULL,NULL);
      }
      else
      {
        socklen_t addrlen=sizeof(struct sockaddr_in);   //结构体的大小
        recv_size=recvfrom(sockfd_,buf,sizeof(buf)-1,0,(struct sockaddr*)src_addr,&addrlen);
      }
      if(recv_size<0)
      {
        perror("recvfrom");
        return false;
      }
      data->assign(buf,recv_size);   //这里只接受buf里长度为recv_size的内容
      return true; 
    }
    void Close()  //关闭套接字，和关闭文件描述符一样,函数名头一个字母尽量大写，避免重复定义
    {
      close(sockfd_);
      sockfd_=-1;
    }
};
