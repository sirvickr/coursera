#include <iostream>
#include <set>
#include <vector>

using namespace std;

// Возвращает вектор из всех элементов множества, бо́льших border, в возрастающем порядке.
template <typename T>
vector<T> FindGreaterElements(const set<T>& elements, const T& border) {
}

int main() {
  for (int x : FindGreaterElements(set<int>{1, 5, 7, 8}, 5)) {
    cout << x << " ";
  }
  cout << endl;

  cout << FindGreaterElements(set<string>{"C", "C++"}, string("Python")).size() << endl;

  return 0;
}
