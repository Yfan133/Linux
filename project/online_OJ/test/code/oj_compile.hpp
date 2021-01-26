#pragma once
// 对用户提交的代码进行处理

#include <unistd.h>
#include <atomic>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <json/json.h>

#include "tools.hpp"

enum ErrorNo
{
  OK = 0,
  PRAM_ERROR,       // 参数错误
  INTERNAL_ERROR,   // 内部错误
  COMPILE_ERROR,    // 编译错误
  RUN_ERROR         // 运行错误
};
class Compiled
{
  public:
    static void CompileAndRun(Json::Value Req, Json::Value* Resp)
    {
      // 1.对code进行检查
      if (Req["code"].empty())
      {
        // 参数错误
        (*Resp)["errorno"] = PRAM_ERROR;
        (*Resp)["reason"] = "Pram Error";
        return;
      }
      // 2.将code写到文件中去
      std::string code = Req["code"].asString();
      std::string filename = WriteTmpFile(code);
      //std::cout << filename << std::endl;
      if (filename == "")
      {
        // 写文件失败
        (*Resp)["errorno"] = INTERNAL_ERROR;
        (*Resp)["reason"] = "Internal Error";
        return;
      }
      // 3.对code进行编译
      if (!Compile(filename))
      {
        // 编译失败
        (*Resp)["errorno"] = COMPILE_ERROR;
        // 设置编译错误的响应体
        std::string reason;
        FileUtil::file_read(ErrorPath(filename), &reason);
        (*Resp)["reason"] = reason;
        Clean(filename);
        return;
      }
      // 4.运行code
      int ret = Run(filename);
      if (ret != 0)
      {
        // 运行失败，-1：创建子进程错误，-2：打开文件失败
        (*Resp)["errorno"] = RUN_ERROR;
        (*Resp)["reason"] = "pragma exit by sig : " + std::to_string(ret);
        Clean(filename);
        return;
      }
      // 5.设置响应
      (*Resp)["errorno"] = OK;
      (*Resp)["reason"] = "Compile and Run OK";

      std::string stdout_str;
      FileUtil::file_read(StdoutPath(filename), &stdout_str);
      std::string stderr_str;
      FileUtil::file_read(StderrorPath(filename), &stderr_str);
      
      (*Resp)["stdout"] = stdout_str;
      (*Resp)["stderr"] = stderr_str;
      // 6.删除临时文件
      Clean(filename);
      return;
    }
  private:
    static void Clean(const std::string& filename)
    {
      unlink(SrcPath(filename).c_str());
      unlink(ExePath(filename).c_str());
      unlink(ErrorPath(filename).c_str());
      unlink(StdoutPath(filename).c_str());
      unlink(StderrorPath(filename).c_str());
    }
    static int Run(const std::string& filename)
    {
      int pid = fork();
      if (pid > 0)
      {
        int status = 0;
        waitpid(pid, &status, 0);
        // 返回退出信号
        return status & 0x7f;
      }
      else if (pid == 0)
      {
        // 1.设置内存限制
        struct rlimit rlim;
        rlim.rlim_cur = 30000 * 1024;
        rlim.rlim_max = RLIM_INFINITY;
        setrlimit(RLIMIT_AS, &rlim);
        // 2.将标准输出和标准错误重定向到不同文件
        int fd_stdout = open(StdoutPath(filename).c_str(), O_RDWR | O_CREAT, 0666);
        if (fd_stdout < 0)
        {
          perror("open fail");
          return -2;
        }
        dup2(fd_stdout, 1);
        int fd_stderr = open(StderrorPath(filename).c_str(), O_RDWR | O_CREAT, 0666);
        if (fd_stderr < 0)
        {
          perror("open fail");
          return -2;
        }
        dup2(fd_stderr, 2);
        // 3.设置运行时间限制
        alarm(1);
        // 4.子进程程序替换去运行code
        execl(ExePath(filename).c_str(), ExePath(filename).c_str(), NULL);
        close(fd_stdout);
        close(fd_stderr);
        exit(0);
      }
      else 
      {
        return -1;
      }
      return 0;
    }
    static bool Compile(const std::string& filename)
    {
      int pid = fork();
      if (pid > 0)
      {
        // 父进程进程等待
        waitpid(pid, NULL, 0);
      }
      else if (pid == 0)
      {
        // 子进程
        // 1.将标准错误重定向到创建出的文件中
        int fd_err = open(ErrorPath(filename).c_str(), O_RDWR | O_CREAT, 0666);
        if (fd_err < 0)
        {
          return false;
        }
        dup2(fd_err, 2);
        // 2.进程替换
        execlp("g++", "g++", SrcPath(filename).c_str(), "-o", ExePath(filename).c_str(), "-std=c++11", "-D", "CompileOnline", NULL);
        // 子进程替换失败则直接退出，这里也算编译失败
        close(fd_err);
        exit(0);
      }
      else
      {
        // 进程创建失败
        return false;
      }
      // 判断是否生成了可执行文件，如果没有生成则说明编译失败
      struct stat st;
      int ret = stat(ExePath(filename).c_str(), &st);
      if (ret < 0)
      {
        return false;
      }
      return true;
    }

private:
    static std::string StdoutPath(const std::string& filename)
    {
      // 运行信息
      // 返回的是 ./tmp_file/tmp_1611242441.0.stdout
      return "./tmp_file/" + filename + ".stdout";
    }
    static std::string StderrorPath(const std::string& filename)
    {
      // 运行信息
      // 返回的是 ./tmp_file/tmp_1611242441.0.stderror
      return "./tmp_file/" + filename + ".stderror";
    }
    static std::string ErrorPath(const std::string& filename)
    {
      // 错误信息
      // 返回的是 ./tmp_file/tmp_1611242441.0.compile_err
      return "./tmp_file/" + filename + ".compile_err";
    }
    static std::string ExePath(const std::string& filename)
    {
      // 可执行文件
      // 返回的是 ./tmp_file/tmp_1611242441.0.exe
      return "./tmp_file/" + filename + ".exe";
    }
    static std::string SrcPath(const std::string& filename)
    {
      // 源文件
      // 返回的是 ./tmp_file/tmp_1611242441.0.cpp
      return "./tmp_file/" + filename + ".cpp";
    }
    static std::string WriteTmpFile(const std::string& code)
    {
      // 1.为了区分不同客户端发来的代码，我们应按时间生成不同的文件名
      // 2.光按时间进行区分，不能保证线程安全，高并发情况下会出现问题
      static std::atomic_uint id(0);
      std::string filename = "tmp_" + std::to_string(TimeUtil::GetTimeStampMs()) + "." + std::to_string(id);
      id++;
      // 3.创建一个文件，并且把code写入文件
      FileUtil::file_write(Compiled::SrcPath(filename), code);
      return filename;
    }
};
