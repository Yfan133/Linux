#pragma once
// 对用户提交的代码进行处理

#include <iostream>
#include <string>
#include <json/json.h>

#include "tools.hpp"

enum ErrorNo
{
  OK = 0,
  PRAM_ERROR,
  KERNEL_ERROR,
  COMPILE_ERROR,
  RUN_ERROR
};
class Compiled
{
  public:
    static void CompileAndRun(Json::Value Req, Json::Value* Resp)
    {
      // 1.先进行code检查
      if (Req["code"].empty())
      {
        (*Resp)["errorno"] = PRAM_ERROR;
        (*Resp)["reason"] = "Pram Error";
        return;
      }
      // 2.把code写到文件中去
      std::string code = Req["code"].asString();
      std::string filename = WriteTmpFile(code);
      std::cout << filename << std::endl;
      // 3.编译
      // 4.运行
      // 5.设置响应
    }
    static std::string SrcPath(const std::string& filename)
    {
      return filename + ".cpp";
    }
    static std::string WriteTmpFile(const std::string& code)
    {
      // 1.为了区分不同客户端发来的代码，我们应按时间生成不同的文件名
      std::string filename = "tmp_" + std::to_string(TimeUtil::GetTimeStampMs());
      FileUtil::file_write(Compiled::SrcPath(filename), code);
      // 2.返给上层调用者文件名称
      return filename;
    }
};
