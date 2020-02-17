#include "test_runner.h"
#include "dequeue2.cpp"

using namespace std;

void TestDeque() {
  Deque<int> d;
  d.PushBack(10);
  d.PushBack(20);
  d.PushFront(30);
  d.PushFront(40);
  cout << "deque: " << d[0] << " " << d[1] << " " << d[2] << " " << d[3] << endl;
  ASSERT_EQUAL(d.Front(), 40);
  ASSERT_EQUAL(d.Back(), 20);
  ASSERT_EQUAL(d.Size(), 4);
  ASSERT_EQUAL(d.At(0), d.Front());
  ASSERT_EQUAL(d.At(d.Size() - 1), d.Back());
  ASSERT_EQUAL(d.At(1), d[1]);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestDeque);
}
