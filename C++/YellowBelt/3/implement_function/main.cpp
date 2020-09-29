#include "sum_reverse_sort.h"

#include "test_runner.h"

void TestSum() {
  AssertEqual(Sum(-2, 5), 3, "");
}

void TestReverse() {
  AssertEqual(Reverse("ab c"), "c ba", "");
}

void TestSort() {
  vector<int> nums = {5, 2, 8};
  Sort(nums);
  AssertEqual(nums, vector<int>{2, 5, 8}, "");
}

int main() {
  TestRunner tr;
  tr.RunTest(TestSum, "TestSum");
  tr.RunTest(TestReverse, "TestReverse");
  tr.RunTest(TestSort, "TestSort");
  return 0;
}
