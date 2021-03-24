#include "test_runner.h"

#include <iostream>
#include <set>
#include <vector>

using namespace std;

// Возвращает вектор из всех элементов множества, бо́льших border, в возрастающем порядке.
template <typename T>
vector<T> FindGreaterElements(const set<T>& elements, const T& border) {
  auto it = begin(elements);
  while (it != end(elements) && *it <= border) {
    ++it;
  }
  return {it, end(elements)};
}

void TestAll() {
  AssertEqual(FindGreaterElements(set<int>{1, 5, 7, 8}, 5), vector<int>{7, 8}, "test-1");
  AssertEqual(FindGreaterElements(set<string>{"C", "C++"}, string("Python")).size(), 0u, "test-2");
  AssertEqual(FindGreaterElements(set<int>{1, 5, 7, 8}, -1), vector<int>{1, 5, 7, 8}, "test-3");
}

int main() {
  TestRunner tr;
  tr.RunTest(TestAll, "TestAll");

  for (int x : FindGreaterElements(set<int>{1, 5, 7, 8}, 5)) {
    cout << x << " ";
  }
  cout << endl;

  cout << FindGreaterElements(set<string>{"C", "C++"}, string("Python")).size() << endl;

  return 0;
}
