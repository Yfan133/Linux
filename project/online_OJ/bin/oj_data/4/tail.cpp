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
  int ret = Solution().cuttingRope(2);
  if (ret == 1) {
    std::cout << "Test1 ok!" << std::endl;
  } else {
    std::cout << "Test1 failed! input: 2, output expected 1" << std::endl;
  }
}

void Test2() {
  int ret = Solution().cuttingRope(10);
  if (ret == 36) {
    std::cout << "Test2 ok!" << std::endl;
  } else {
    std::cout << "Test2 failed! input: 10, output expected 36" << std::endl;
  }
}

int main() {
  Test1();
  Test2();
  return 0;
}