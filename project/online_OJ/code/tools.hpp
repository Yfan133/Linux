#pragma once

#include <iostream>
#include <sys/time.h>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#include <boost/algorithm/string.hpp>

class FileUtil
{
  public:
    static bool file_read(const std::string file_path, std::string* content)
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
    static bool file_write(const std::string& file_path, const std::string& data)
    {
      std::ofstream file(file_path.c_str());
      if (!file.is_open())
      {
        return false;
      }
      file.write(data.c_str(), data.size());
      file.close();
      return true;
    }
};

class StringSplit
{
  public:
    static void Split(const std::string& input, const std::string& split_char, std::vector<std::string>* output)
    {
      boost::split(*output, input, boost::is_any_of(split_char), boost::token_compress_off);
    }
};
class Urlcode
{
  public:
    static void PraseBody(const std::string& body, std::unordered_map<std::string, std::string>* body_kv)
    {
      // 1.先按照&进行切分，切分成不同key=value
      std::vector<std::string> kv_vec;
      StringSplit::Split(body, "&", &kv_vec);
      // 2.再按照=进行切分，把key value分开
      for (const auto it : kv_vec)
      {
        std::vector<std::string> kv_sig;
        StringSplit::Split(it, "=", &kv_sig);
        if (kv_sig.size() != 2)
        {
          continue;
        }
        // 3.把切分后的key:value保存到map中返回给上层调用者
        (*body_kv)[kv_sig[0]] = UrlDecode(kv_sig[1]);
      }
    }
    static unsigned char ToHex(unsigned char x)   
    {   
      return  x > 9 ? x + 55 : x + 48;   
    }  
    static unsigned char FromHex(unsigned char x)   
    {   
      unsigned char y;  
      if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;  
      else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;  
      else if (x >= '0' && x <= '9') y = x - '0';  
      else assert(0);  
      return y;  
    }  

    static std::string UrlEncode(const std::string& str)  
    {  
      std::string strTemp = "";  
      size_t length = str.length();  
      for (size_t i = 0; i < length; i++)  
      {  
        if (isalnum((unsigned char)str[i]) ||   
            (str[i] == '-') ||  
            (str[i] == '_') ||   
            (str[i] == '.') ||   
            (str[i] == '~'))  
          strTemp += str[i];  
        else if (str[i] == ' ')  
          strTemp += "+";  
        else  
        {  
          strTemp += '%';  
          strTemp += ToHex((unsigned char)str[i] >> 4);  
          strTemp += ToHex((unsigned char)str[i] % 16);  
        }  
      }  
      return strTemp;  
    }  

    static std::string UrlDecode(const std::string& str)  
    {  
      std::string strTemp = "";  
      size_t length = str.length();  
      for (size_t i = 0; i < length; i++)  
      {  
        if (str[i] == '+') strTemp += ' ';  
        else if (str[i] == '%')  
        {  
          assert(i + 2 < length);  
          unsigned char high = FromHex((unsigned char)str[++i]);  
          unsigned char low = FromHex((unsigned char)str[++i]);  
          strTemp += high*16 + low;  
        }  
        else strTemp += str[i];  
      }  
      return strTemp;  
    } 
};
// 获取时间戳
class TimeUtil
{
  public:
    // 返回给上层调用者
    static int64_t GetTimeStampMs()
    {
      struct timeval tv;
      gettimeofday(&tv, NULL);
      return tv.tv_sec + tv.tv_usec / 1000;
    }
};
