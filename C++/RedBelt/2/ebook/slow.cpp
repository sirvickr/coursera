#include <iomanip>
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>

#include "test_runner.h"
#include "profile.h"

using namespace std;

class ReadingManager {
public:
  ReadingManager()
      : user_page_counts_(MAX_USER_COUNT_ + 1, 0),
        users_on_page_(MAX_PAGE_COUNT_ + 1, 0),
        user_count_(0) {}

  void Read(int user_id, int page_count) {
    if (user_page_counts_[user_id] == 0) {
      ++user_count_; // новый читатель
    } else {
      --users_on_page_[user_page_counts_[user_id]]; // пользователь ушёл с этой страницы
    }
    ++users_on_page_[page_count];        // и остановился на этой
    user_page_counts_[user_id] = page_count;
  }

  double Cheer(int user_id) const {
    if (user_page_counts_[user_id] == 0) {
      return 0;
    }
    const int user_count = GetUserCount();
    if (user_count == 1) {
      return 1;
    }
    const int page_count = user_page_counts_[user_id];
    int looser_count = 0; // счётчик числа пользователей, остановившихся на предыдущих страницах
    for(int i = 1; i < page_count; ++i) {
      looser_count += users_on_page_[i];
    }

    // По умолчанию деление целочисленное, поэтому
    // нужно привести числитель к типу double.
    // Простой способ сделать это — умножить его на 1.0.
    return looser_count * 1.0 / (user_count - 1);
  }

private:
  // Статическое поле не принадлежит какому-то конкретному
  // объекту класса. По сути это глобальная переменная,
  // в данном случае константная.
  // Будь она публичной, к ней можно было бы обратиться снаружи
  // следующим образом: ReadingManager::MAX_USER_COUNT.
  static const int MAX_USER_COUNT_ = 100'000;
  static const int MAX_PAGE_COUNT_ =   1'000;

  vector<int> user_page_counts_;
  vector<int> users_on_page_; // количество пользователей в конце каждой страницы
  int user_count_;

  int GetUserCount() const {
    return user_count_;
  }
};

void TestSample() {
  ReadingManager manager;
  ASSERT_EQUAL(manager.Cheer(5), 0.0);
  manager.Read(1, 10);
  ASSERT_EQUAL(manager.Cheer(1), 1.0);
  manager.Read(2, 5);
  manager.Read(3, 7);
  ASSERT_EQUAL(manager.Cheer(2), 0.0);
  ASSERT_EQUAL(manager.Cheer(3), 0.5);
  manager.Read(3, 10);
  ASSERT_EQUAL(manager.Cheer(3), 0.5);
  manager.Read(3, 11);
  ASSERT_EQUAL(manager.Cheer(3), 1.0);
  ASSERT_EQUAL(manager.Cheer(1), 0.5);
}

void task(istream& is, ostream& os) {
  ReadingManager manager;

  int query_count;
  is >> query_count;

  LOG_DURATION("total")
  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    is >> query_type;
    int user_id;
    is >> user_id;

    if (query_type == "READ") {
      int page_count;
      is >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      os << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSample);
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  task(cin, cout);

  return 0;
}
