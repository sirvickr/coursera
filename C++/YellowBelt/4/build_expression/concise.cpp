#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <limits>

using namespace std;

#if 1
int main() {
  const map<string, int> operations = { {"+", 1}, {"-", 1}, {"*", 2}, {"/", 2} };
  int n;
  string base, expr, operand, operation;//, curr_operation;
  int prio_prev = numeric_limits<int>::max();
  cin >> base >> n;
  expr = base;
  for (int i = 0; i < n; ++i) {
    ostringstream oss;
    cin >> operation >> operand;
    int prio_curr = operations.at(operation);
    if(prio_curr > prio_prev) {
      oss << "(" << expr << ")";
    } else {
      oss << expr;
    }
    oss << ' ' << operation << ' ' << operand;
    expr = oss.str();

    prio_prev = prio_curr;
  }
  cout << expr;
  return 0;
}
#endif

