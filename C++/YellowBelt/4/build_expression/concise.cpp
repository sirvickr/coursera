#include <limits>
#include <iostream>
#include <string>
#include <map>
#include <deque>

using namespace std;

#if 1
int main() {
  const map<string, int> operations = { {"+", 1}, {"-", 1}, {"*", 2}, {"/", 2} };
  int n;
  string base, operand, operation;//, curr_operation;
  deque<string> expr;
  int prio_prev = numeric_limits<int>::max();
  cin >> base >> n;

  expr.push_back(base);

  for (int i = 0; i < n; ++i) {
    cin >> operation >> operand;
    int prio_curr = operations.at(operation);
    if(prio_curr > prio_prev) {
      expr.push_front("(");
      expr.push_back(")");
    }
    expr.push_back(" ");
    expr.push_back(operation);
    expr.push_back(" ");
    expr.push_back(operand);

    prio_prev = prio_curr;
  }
  for(const auto& s: expr) {
    cout << s;
  }
  return 0;
}
#endif

