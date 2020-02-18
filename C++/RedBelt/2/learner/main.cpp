#include <iostream>
#include <iomanip>
#include "test_runner.h"
#include "learner.cpp"
#include "profile.h"

#include <sstream>
#include <fstream>

int main() {
  #if 0
  ofstream stream("big.txt");
  int count = 10000;
  for(int i = 0; i < count; ++i) {
    stream << (count - i) << " ";
  }
  stream << '\n';
  stream << "10 20 30 101 0";
  #else
  LOG_DURATION("total")
  
  Learner learner;
  string line;
  while (getline(cin, line)) {
    vector<string> words;
    stringstream ss(line);
    string word;
    while (ss >> word) {
      words.push_back(word);
    }
    cout << learner.Learn(words) << "\n";
  }
  #if 1
  //cout << "=== known words ===\n";
  for (auto word : learner.KnownWords()) {
    cout << word << "\n";
  }
  #endif
  #endif
  return 0;
}
