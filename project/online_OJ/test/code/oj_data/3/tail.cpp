#ifndef CompileOnline
//这是为了编写用例的时候有语法提示. 实际线上编译的过程中这个操作是不生效的.
#include "header.cpp"
#endif

///////////////////////////////////////////////////////
// 此处约定:
// 1. 每个用例是一个函数
// 2. 每个用例从标准输出输出一行日志
// 3. 如果用例通过, 统一打印 [TestName] ok!
// 4. 如果用例不通过, 统一打印 [TestName] failed! 并且给出合适的提示.
///////////////////////////////////////////////////////

void Test1() {
  std::string s = "We are happy.";
  std::string ret = Solution().replaceSpace(s);
  if (ret == "We%20are%20happy.") {
    std::cout << "Test1 ok!" << std::endl;
  } else {
    std::cout << "Test1 failed! input: \"We are happy.\", output：\"" << ret << "\"" << std::endl;
  }
}

void Test2() {
  std::string s = "  ";
  std::string ret = Solution().replaceSpace(s);
  if (ret == "%20%20") {
    std::cout << "Test2 ok!" << std::endl;
  } else {
    std::cout << "Test2 failed! input: \"  \", output：\"" << ret << "\"" << std::endl;
  }
}

int main() {
  Test1();
  Test2();
  return 0;
}
