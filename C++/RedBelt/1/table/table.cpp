#include "test_runner.h"

#include <vector>
#include <iostream>

using namespace std;

template<typename T>
class Table {

  template<typename InputIt>
  class RowView {
  public:
    RowView(InputIt start, InputIt stop)
    : start(start), stop(stop) {
    }
    const typename iterator_traits<InputIt>::value_type& operator[](size_t j) const {
      return *next(start, j);
    }
    typename iterator_traits<InputIt>::value_type& operator[](size_t j) {
      return *next(start, j);
    }
  private:
    InputIt start, stop;
  };

public:
  Table(size_t m = 0, size_t n = 0) {
    Resize(m, n);
  }

  void Resize(size_t m, size_t n) {
    if (m == 0 || n == 0)
      m = n = 0;
    data.resize(m);
    for(auto& v: data) {
      v.resize(n, T());
    }
    this->m = m;
    this->n = n;
  }

  pair<size_t, size_t> Size() const {
    return {m, n};
  }

  RowView<typename vector<T>::iterator> operator[](size_t i) {
    return RowView<typename vector<T>::iterator>(begin(data[i]), end(data[i]));
  }

  const RowView<typename vector<T>::const_iterator> operator[](size_t i) const {
    return RowView<typename vector<T>::iterator>(begin(data[i]), end(data[i]));
  }

private:
  size_t m, n;
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
