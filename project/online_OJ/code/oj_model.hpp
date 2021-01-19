#pragma once
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
    std::unordered_map <std::string, Questions> ques_map;
  public:
    // 运行加载函数Load
    OjModel()
    {
      Load("./oj_data/oj_config_cfg");
    }
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
        StringSpit::Split(buf, " ", &vec);  // yikonggeqiefen
        // 保存切分出来的内容
        Questions ques;
        ques.id_ = vec[0];
        ques.title_ = vec[1];
        ques.comp_ = vec[2];
        ques.path_ = vec[3];
        
        // 打开路径下的文件，并且保存试题的详细信息
        std::string dir = vec[3];
        FileOpen::file_open(dir + "/desc.txt", &ques.desc_);
        FileOpen::file_open(dir + "/header.cpp", &ques.header_);
        FileOpen::file_open(dir + "/tail.cpp", &ques.tail_);

        // 把信息放入无序map中
        ques_map[ques.id_] = ques;
      }
      file.close();
      return true;
    }

    // 返回给调用者所有试题目录
    bool GetAllQuestions(std::vector<Questions>* all_ques)
    {
      // 把无序map中的所有内容给allques
      for (const auto it : ques_map)
      {
        (*all_ques).push_back(it.second);
      }
      // 对allques排序
      std::sort((*all_ques).begin(), (*all_ques).end(), [](Questions& ques1, Questions& ques2){
          return std::stoi(ques1.id_) < std::stoi(ques2.id_);
          });
      return true;
    }

    // 返回给调用者单个试题内容
    bool GetOneQues(const std::string& id, Questions* question)
    {
      // 从map中通过id查找
      auto ques = ques_map.find(id);
      if (ques == ques_map.end())
      {
        return false;
      }
      *question = ques->second;
      return true;
    }
};
