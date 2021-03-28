#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#if 1
int main() {
  int n;
  string base, expr, operand, operation;
  cin >> base >> n;
  expr = base;
  for (int i = 0; i < n; ++i) {
    ostringstream oss;
    cin >> operation >> operand;
    oss << "(" << expr << ") " << operation << ' ' << operand;
    expr = oss.str();
  }
  cout << expr;
  return 0;
}
#endif
