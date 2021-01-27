#include <iostream>
#include <unistd.h>

int main()
{
  int i = 0;
  fork();
  fork();
  fork();
  printf("hello\n");
  return 0;
}
