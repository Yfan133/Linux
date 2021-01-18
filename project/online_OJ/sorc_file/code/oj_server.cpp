#include <iostream>
#include <cstdio>

#include "httplib.h"
#include "oj_model.hpp"
int main()
{
  using namespace httplib;
  Server svr;
  OjModel model;

  // 1.注册所有题目的资源路径
  svr.Get("/all_questions", [&model](const Request &req, Response &resp){
      std::vector<Questions> all_ques;
      model.GetAllQuestions(&all_ques);
      for (auto ques : all_ques)
      {
        std::cout << ques.id_ << " " << ques.title_ << " " << ques.comp_ << " " << ques.path_ << std::endl;
      }
      resp.set_content("<html><h1>this is Linux~</h1></html>", 36, "text/html");
      });
  // 2.注册单个题目的资源路径 
  svr.Get("/signle_ques", [](const Request &req, Response &resp){

      });
  // 3.注册提交代码并运行的路径
  svr.Post("/test-no", [](const Request &req, Response &resp){
  
      });
  svr.listen("192.168.23.128", 18888);
  return 0;
}
