#include "test_runner.h"

#include <vector>
#include <iostream>

using namespace std;

template<typename T>
class Table {
public:
  Table(size_t m = 0, size_t n = 0) {
    Resize(m, n);
  }

  void Resize(size_t m, size_t n) {
    data.resize(m);
    for(auto& v: data) {
      v.resize(n);
    }
  }

  pair<size_t, size_t> Size() const {
    if (!data.empty() && !data[0].empty()) {
      return {data.size(), data[0].size()};
    }

    return  {0, 0};
  }

  vector<T>& operator[](size_t i) {
    return data[i];
  }

  const vector<T>& operator[](size_t i) const {
    return data[i];
  }

private:
  vector<vector<T>> data;
};

void TestTable() {
  Table<int> t(1, 1);
  ASSERT_EQUAL(t.Size().first, 1u);
  ASSERT_EQUAL(t.Size().second, 1u);
  t[0][0] = 42;
  ASSERT_EQUAL(t[0][0], 42);
  t.Resize(3, 4);
  ASSERT_EQUAL(t.Size().first, 3u);
  ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestTable);
  return 0;
}
