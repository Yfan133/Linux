#include <iostream>
#include <cstdio>

#include "httplib.h"
#include "oj_model.hpp"
#include "oj_view.hpp"
using namespace httplib;

int main()
{
  Server svr;
  OjModel model;

  // 1.注册所有题目的资源路径
  svr.Get("/all_questions", [&model](const Request &req, Response &resp){
      std::vector<Questions> all_ques;
      model.GetAllQuestions(&all_ques);       // 获取所有题目信息

      std::string html;
      Oj_View::FillInAllQuestions(all_ques, &html);   // 获取html页面
      resp.set_content(html, "text/html");    // 设置响应体
      });
  // 2.注册单个题目的资源路径 
  svr.Get(R"(/question/(\d+))", [&model](const Request &req, Response &resp){
      // 获取单个试题的信息
      Questions question;
      model.GetOneQues(req.matches[1], &question);  
      // 获取html页面
      std::string html;
      Oj_View::FillInOneQuestion(question, &html);  
      // 设置响应体
      resp.set_content(html, "text/html");
      });
  // 3.注册提交代码并运行的路径
  svr.Post(R"(/compile/(\d+))", [&model](const Request &req, Response &resp){
      // 1.获取试题编号和内容
      Questions question;
      model.GetOneQues(req.matches[1], &question);
      // 2.对req进行url解码
      std::cout << Urlcode::UrlDecode(req.body) << std::endl;
      // 3.把code和预定义的头结合并运行
      // 4.根据不同的运行结果，返回响应
      resp.set_content("<html>this is Linux~</html>", 27, "text/html");
      });
  svr.listen("192.168.23.128", 18888);
  return 0;
}
