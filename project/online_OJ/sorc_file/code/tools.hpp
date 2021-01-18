#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

class FileOpen
{
  public:
    static bool file_open(const std::string file_path, std::string* content)
    {
      (*content).clear();
      std::ifstream file(file_path.c_str());
      if (!file.is_open())
      {
        perror("open_file");
        return false;
      }
      std::string buf;
      while (std::getline(file, buf))
      {
        (*content) += buf + "\n";
      }
      file.close();
      return true;
    }
};

class StringSpit
{
  public:
    static void Split(const std::string& input, const std::string& split_char, std::vector<std::string>* output)
    {
      boost::split(*output, input, boost::is_any_of(split_char), boost::token_compress_off);
    }
};
