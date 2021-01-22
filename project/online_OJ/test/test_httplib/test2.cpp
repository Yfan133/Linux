#include <iostream>
#include <string>
#include "httplib.h"

int main()
{
  // 1.创建一个Server类对象
  httplib::Server http_svr;

  // 2.注册Get方法
  http_svr.Get("/index", [](const httplib::Request& req, httplib::Response& resp){
      std::cout << req.method << " " << req.path << " " << req.version << std::endl;
      std::string html = "<html>this is Linux~~<html>";
      resp.set_content(html, "text/html");
      });
  http_svr.listen("0.0.0.0", 17878);
  return 0;
}
