#include <stdio.h>
#include "httplib.h"

void func(const httplib::Request& req, httplib::Response& resp)
{
  resp.set_content("<html><h1>this is Linux~</h1></html>", 36, "text/html");
  printf("Recv\n");
}

int main()
{
  // 创建Server类对象 
  httplib::Server svr;

  // 注册Get方法,第一个参数路径名，第二个参数回调函数
  svr.Get("/abc", func);

  // 侦听,第一个参数ip，第二个参数端口
  svr.listen("192.168.23.128", 18888);
  
  return 0;
}
