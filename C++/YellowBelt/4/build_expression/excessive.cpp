#include <iostream>
#include <string>
#include <deque>

using namespace std;

#if 1
int main() {
  int n;
  string base, operand, operation;
  deque<string> expr;
  cin >> base >> n;

  expr.push_back(base);

  for (int i = 0; i < n; ++i) {
    cin >> operation >> operand;
    expr.push_front("(");
    expr.push_back(")");
    expr.push_back(" ");
    expr.push_back(operation);
    expr.push_back(" ");
    expr.push_back(operand);
  }
  for(const auto& s: expr) {
    cout << s;
  }
  return 0;
}
#endif
