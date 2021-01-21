#pragma once
#include <iostream>
#include "/usr/include/jsoncpp/json/json.h"

#include "oj_model.hpp"
#include "ctemplate/template.h"

class Oj_View
{
  public:
    // 填充http模板，并且返回填充后的string类型
    static void FillInAllQuestions(std::vector<Questions>& questions, std::string* html)
    {
      // 创建一个模板字典
      ctemplate::TemplateDictionary dic("all_questions");
      // 给字典添加试题列表,用vector里的内容填充
      for (const auto& ques : questions)
      {
        // 创建一个子字典
        ctemplate::TemplateDictionary* sub_dic = dic.AddSectionDictionary("question");

        // 设置子字典的内容
        sub_dic->SetValue("id", ques.id_);
        sub_dic->SetValue("id", ques.id_);
        sub_dic->SetValue("title", ques.title_);
        sub_dic->SetValue("star", ques.comp_);
      }

      // 从文件中获取http模板
      ctemplate::Template* tl = ctemplate::Template::GetTemplate("./template/all_questions.html", ctemplate::DO_NOT_STRIP);
      // 对模板填充数据
      tl->Expand(html, &dic);
    }
    static void FillInOneQuestion(const Questions& question, std::string* html)
    {
      ctemplate::TemplateDictionary dic("question");
      dic.SetValue("id", question.id_);
      dic.SetValue("title", question.title_);
      dic.SetValue("star", question.comp_);
      dic.SetValue("desc", question.desc_);
      dic.SetValue("id", question.id_);
      dic.SetValue("code", question.header_);
      
      // 从文件中获取http模板
      ctemplate::Template* tl = ctemplate::Template::GetTemplate("./template/question.html", ctemplate::DO_NOT_STRIP);
      // 填充数据
      tl->Expand(html, &dic);
    }
    static void FillInResponse(const Json::Value& Resp, std::string* html)
    {
      ctemplate::TemplateDictionary dic("resp");
      dic.SetValue("errorno", Resp["errorno"].asString());
      dic.SetValue("reason", Resp["reason"].asString());
      ctemplate::Template* tl = ctemplate::Template::GetTemplate("./template/response.html", ctemplate::DO_NOT_STRIP);
      tl->Expand(html, &dic);
    }
};
