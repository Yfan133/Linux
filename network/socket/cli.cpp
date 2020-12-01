#include"udp.hpp"

#define CHECK_RET(p) if(p==false){return 0;}  //定义一个宏用于检查返回值是否为空

int main()
{
  Udp u;  //创建一个类对象
  CHECK_RET(u.CreateSock());

  while(1)
  {
    printf("send data to server: ");
    fflush(stdout);
    std::string buf;  
    std::cin>>buf;    //用于用户输入

    struct sockaddr_in dest_addr;   //目标addr
    dest_addr.sin_family=AF_INET;   //设置ipv4协议
    dest_addr.sin_port=htons(19999);  //设置port端口
    dest_addr.sin_addr.s_addr=inet_addr("192.168.0.38");
    u.Send(buf,&dest_addr); //给服务端发送数据

    buf.clear();
    u.Recv(&buf,NULL);  //接收数据，传NULL
    printf("svrver tell me [%s]\n",buf.c_str());
  }
  u.Close();
  return 0;
}
