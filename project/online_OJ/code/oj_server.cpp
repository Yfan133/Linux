#include <iostream>
#include <cstdio>
#include <json/json.h>

#include "httplib.h"

#include "oj_model.hpp"
#include "oj_view.hpp"
#include "oj_compile.hpp"
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
      resp.set_content(html, "text/html");
      });
  // 3.注册提交代码并运行的路径
  svr.Post(R"(/compile/(\d+))", [&model](const Request &req, Response &resp){
      // 1.获取试题编号和内容
      Questions question;
      model.GetOneQues(req.matches[1], &question);
      // 2.对浏览器的请求体进行切分，切分成key:value形式，再进行url解码
      std::unordered_map<std::string, std::string> kv_map;
      Urlcode::PraseBody(req.body, &kv_map);
      for (const auto it : kv_map)
      {
        std::cout << it.first << std::endl << it.second << std::endl;
      }
      // 3.把code和预定义的头结合，创建出一个新的文件
      std::string code = kv_map["code"];
      std::string stdin = kv_map["stdin"];
      Json::Value Req_Js;
      Json::Value Resp_Js;
      Req_Js["code"] = code + question.tail_;
      Req_Js["stdin"] = stdin;
      // httplib库会创建线程去处理注册的方法，因此高并发时写文件不是线程安全的
      Compiled::CompileAndRun(Req_Js, &Resp_Js);
      // 4.根据不同的运行结果，返回响应
      std::string html;
      Oj_View::FillInResponse(Resp_Js, &html);
      resp.set_content(html, "text/html");
      });
  svr.listen("0.0.0.0", 18888);
  return 0;
}
