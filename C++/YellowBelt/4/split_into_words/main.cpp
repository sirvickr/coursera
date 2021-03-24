#include "test_runner.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/*
Гарантируется, что:
строка непуста;
строка состоит лишь из латинских букв и пробелов;
первый и последний символы строки не являются пробелами;
строка не содержит двух пробелов подряд.
*/

vector<string> SplitIntoWords(const string& s) {
  vector<string> result;
  auto from = begin(s);
  const auto to = end(s);
  while (true) {
    auto it = find(from, to, ' ');
    result.push_back(string(from, it));
    if (it == to) {
      break;
    } else {
      from = it + 1;
    }

  }
  return result;
}

void TestAll() {
  AssertEqual(SplitIntoWords("C"), vector<string>{"C"}, "2");
  AssertEqual(SplitIntoWords("C"), vector<string>{"C"}, "3");
  AssertEqual(SplitIntoWords("C Cpp Java Python"), vector<string>{"C", "Cpp", "Java", "Python"}, "5");
}

int main() {
  TestRunner tr;
  tr.RunTest(TestAll, "TestAll");

  string s = "C Cpp Java Python";

  vector<string> words = SplitIntoWords(s);
  cout << words.size() << " ";
  for (auto it = begin(words); it != end(words); ++it) {
    if (it != begin(words)) {
      cout << "/";
    }
    cout << *it;
  }
  cout << endl;

  return 0;
}
