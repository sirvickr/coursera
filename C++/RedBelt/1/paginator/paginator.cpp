#include "test_runner.h"

#include <numeric>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <typename Iterator>
class Paginator {
public:
  class Page {
  public:
    Page(Paginator& owner, size_t num, Iterator b, Iterator e)
    : owner_(owner)
    , num_(num)
    , begin_(b)
    , end_(e) {
    }

    size_t size() const {
      return distance(begin_, end_);
    }

    Iterator begin() const {
      return begin_;
    }

    Iterator end() const {
      return end_;
    }

  private:
    Paginator& owner_;
    Iterator begin_, end_;
    size_t num_;
  };

  Paginator(Iterator b, Iterator e, size_t page_size) {
    size_t count = distance(b, e);
    if(count) {
      size_t page_count = (count - 1) / page_size + 1;
      //cout << "item count " << count << " page_size " << page_size << " page_count " << page_count << ": ";
      auto it = b;
      size_t remainder = count;
      for(size_t i = 0; i < page_count; ++i) {
        size_t size = min(page_size, remainder);
        pages_.emplace_back(Page{*this, i, it, it + size});
        it += size;
        remainder -= size;
      }
      //cout << " page_size = " << pages_.size() << endl;
    }
    page_ = pages_.begin();
  }

  size_t size() const {
    return pages_.size();
  }

  auto begin() {
    return pages_.begin();
  }

  auto end() {
    return pages_.end();
  }

private:
  vector<Page> pages_;
  typename vector<Page>::iterator page_;
  size_t currPage_ = 0;
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
  return Paginator{ begin(c), end(c), page_size };
}

void TestPageCounts() {
  vector<int> v(15);
  iota(begin(v), end(v), 1);

  ASSERT_EQUAL(Paginate(v, 1).size(), v.size());
  ASSERT_EQUAL(Paginate(v, 3).size(), 5u);
  ASSERT_EQUAL(Paginate(v, 5).size(), 3u);
  ASSERT_EQUAL(Paginate(v, 4).size(), 4u);
  ASSERT_EQUAL(Paginate(v, 15).size(), 1u);
  ASSERT_EQUAL(Paginate(v, 150).size(), 1u);
  ASSERT_EQUAL(Paginate(v, 14).size(), 2u);
}

void TestLooping() {
  vector<int> v(15);
  iota(begin(v), end(v), 1);

  Paginator<vector<int>::iterator> paginate_v(v.begin(), v.end(), 6);
  ostringstream os;
  for (const auto& page : paginate_v) {
    for (int x : page) {
      os << x << ' ';
    }
    os << '\n';
  }

  ASSERT_EQUAL(os.str(), "1 2 3 4 5 6 \n7 8 9 10 11 12 \n13 14 15 \n");
}

void TestModification() {
  vector<string> vs = {"one", "two", "three", "four", "five"};
  for (auto page : Paginate(vs, 2)) {
    for (auto& word : page) {
      word[0] = toupper(word[0]);
    }
  }

  const vector<string> expected = {"One", "Two", "Three", "Four", "Five"};
  ASSERT_EQUAL(vs, expected);
}

void TestPageSizes() {
  string letters(26, ' ');

  Paginator letters_pagination(letters.begin(), letters.end(), 11);
  vector<size_t> page_sizes;
  for (const auto& page : letters_pagination) {
    page_sizes.push_back(page.size());
  }

  const vector<size_t> expected = {11, 11, 4};
  ASSERT_EQUAL(page_sizes, expected);
}

void TestConstContainer() {
  const string letters = "abcdefghijklmnopqrstuvwxyz";

  vector<string> pages;
  for (const auto& page : Paginate(letters, 10)) {
    pages.push_back(string(page.begin(), page.end()));
  }

  const vector<string> expected = {"abcdefghij", "klmnopqrst", "uvwxyz"};
  ASSERT_EQUAL(pages, expected);
}

void TestPagePagination() {
  vector<int> v(22);
  iota(begin(v), end(v), 1);

  vector<vector<int>> lines;
  for (const auto& split_by_9 : Paginate(v, 9)) {
    for (const auto& split_by_4 : Paginate(split_by_9, 4)) {
      lines.push_back({});
      for (int item : split_by_4) {
        lines.back().push_back(item);
      }
    }
  }

  const vector<vector<int>> expected = {
      {1, 2, 3, 4},
      {5, 6, 7, 8},
      {9},
      {10, 11, 12, 13},
      {14, 15, 16, 17},
      {18},
      {19, 20, 21, 22}
  };
  ASSERT_EQUAL(lines, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestPageCounts);
  RUN_TEST(tr, TestLooping);
  RUN_TEST(tr, TestModification);
  RUN_TEST(tr, TestPageSizes);
  RUN_TEST(tr, TestConstContainer);
  RUN_TEST(tr, TestPagePagination);
}

