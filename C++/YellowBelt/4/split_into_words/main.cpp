#include "test_runner.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

vector<string> SplitIntoWords(const string& s) {
  vector<string> result;
  auto from = begin(s);
  auto to = find(from, end(s), ' ');
  while(to != end(s)) {
    result.push_back(string(from, to));
    from = next(to);
    to = find(from, end(s), ' ');
  }
  if(from != to) {
    result.push_back(string(from, to));
  }
  return result;
}

void TestAll() {
  AssertEqual(SplitIntoWords(""), vector<string>{}, "1");
  AssertEqual(SplitIntoWords("C"), vector<string>{"C"}, "2");
  AssertEqual(SplitIntoWords("C"), vector<string>{"C"}, "3");
  AssertEqual(SplitIntoWords("C "), vector<string>{"C"}, "4");
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
