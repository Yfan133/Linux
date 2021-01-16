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
  CHECK_RET(ts.Bind("192.168.43.29", 18888));
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
    printf("chrome said :\n%s\n", buf.c_str());

    // C风格
    //std::string body = "<html><h1>this is linux</h1></html>";
    //// 响应首行，响应头部
    //char buffer[10240] = { 0 };
    //snprintf(buffer, sizeof(buffer) - 1, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %lu\r\n\r\n", "text/html", body.size());
    //std::string header;
    //header.assign(buffer, strlen(buffer));
    //printf("header : %s\n", header.c_str());
    //new_sock.Send(header);
    //new_sock.Send(body);   
    
    // C++风格
    std::string body = "<html><h1>this is linux~</h1></html>";
    // 响应首行和响应头部
    std::stringstream ss;
    ss << "HTTP/1.1 200 OK\r\n";                      // 协议版本和状态(200 OK: 正常处理)
    ss << "Content-Type:text/html\r\n";               // 正文类型
    ss << "Content-Length:" << body.size() << "\r\n"; // 长度
    ss << "\r\n";                                     // 空行
    std::string header = ss.str();
    new_sock.Send(header);
    new_sock.Send(body);
    
    // 302重定向状态，搭配Location: http://www.baidu.com\r\n 使用
    //std::string body = "<html><h1>this is linux~</h1></html>";
    //// 响应首行和响应头部
    //std::stringstream ss;
    //ss << "HTTP/1.1 302 OK\r\n";                        // 协议版本和状态(302 OK：重定向)
    //ss << "Content-Type: text/html\r\n";                // 正文类型
    //ss << "Content-Length: " << body.size() << "\r\n";  // 长度
    //ss << "Location: http://www.baidu.com\r\n";         // 重定向至百度
    //ss << "\r\n";                                       // 空行
    //std::string header = ss.str();
    //new_sock.Send(header);
    //new_sock.Send(body);

    //std::string body = "<html><h1>this is linux~</h1></html>";
    //std::stringstream ss;
    //// 前提
    //ss << "HTTP/1.1 404 Page Not Found\r\n";
    //// 响应首行和响应头部
    //ss << "Content-Type: text/html\r\n";
    //ss << "Content-:Length: " << body.size() << "\r\n";
    //ss << "\r\n";
    //std::string header(ss.str());
    //new_sock.Send(header);
    //new_sock.Send(body);
  }
  new_sock.Close();
  ts.Close();
  return 0;
}
