// 用于获取文件信息

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

#include "tools.hpp"

// 题目的基本信息
struct Questions
{
  std::string id_;
  std::string title_;
  std::string comp_;
  std::string path_;

  std::string desc_;
  std::string header_;
  std::string tail_;
};
// 从文件中读取
class OjModel
{
  private:
    // 成员变量：map用于保存所有试题信息
    unordered_map <std::string, Questions> ques_map;
  public:
    // 运行加载函数Load
    OjModel(){}
    ~OjModel(){}
    // 读取信息
    bool Load(const std::string& file_path)
    {
      std::ifstream file(file_path.c_str());
      if (!file.is_open())
      {
        perror("open_file");
        return false;
      }
      // 保存读取的每一行内容
      std::string buf;
      // 读取文件内容
      while (std::getline(file, buf))
      {
        // 切分读取的这一行内容
        std::vector<std::string> vec;
        
      }
    }
};
