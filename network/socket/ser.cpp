#include"udp.hpp"

#define CHECK_RET(p) if(p==false){return 0;}

int main()
{
  Udp u;
  CHECK_RET(u.CreateSock());
  CHECK_RET(u.Bind("192.168.0.38",19999));

  while(1)
  {
    std::string buf;
    struct sockaddr_in dest_addr;
    u.Recv(&buf,&dest_addr);  //这里两个都是入参

    printf("Cli say: %s , from [%s]:[%d]\n",buf.c_str(),inet_ntoa(dest_addr.sin_addr),ntohs(dest_addr.sin_port));

    buf.clear();
    printf("reply response: ");
    fflush(stdout);
    std::cin>>buf;    //之前这里没有输入，导致每次用户传过去，立马返回空

    u.Send(buf,&dest_addr);

  }
  u.Close();
  return 0;
}
