#include "tcp.hpp"
#include <string.h>
#include <sstream>

#define CHECK_RET(p) if(p != true){return 0;}

int main()
{
  TcpSock ts;
  // 创建侦听套接字
  CHECK_RET(ts.SockCreate());
  // 绑定地址信息
  CHECK_RET(ts.Bind("192.168.0.88", 18889));
  // 侦听
  CHECK_RET(ts.Listen());
  // 创建新套接字，并接收连接
  TcpSock new_sock;
  struct sockaddr_in peer_addr;
  CHECK_RET(ts.Accept(&peer_addr, &new_sock));
  // 发送接收数据
  while(1)
  {
    std::string buf;
    new_sock.Recv(&buf);
    printf("chrome said : %s\n", buf.c_str());

    // 向浏览器发送数据
    
    // C风格
    // 发送的数据
    std::string body = "<html><h1>this is linux</h1></html>";
    // 响应首行，响应头部
    char buffer[10240] = { 0 };
    snprintf(buffer, sizeof(buffer) - 1, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %lu\r\n\r\n", "text/html", body.size());
    std::string header;
    header.assign(buffer, strlen(buffer));
    printf("header : %s\n", header.c_str());
    new_sock.Send(header);
    new_sock.Send(body);   
  }
  new_sock.Close();
  ts.Close();
  return 0;
}
