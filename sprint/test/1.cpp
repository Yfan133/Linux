#include <iostream>

using namespace std;
int main()
{
  int x = 10;
  int y = 10;
  x = y = y++;
  cout << x << endl << y << endl;
  return 0;
}
