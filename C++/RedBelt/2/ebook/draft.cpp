#include <iomanip>
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>

#include "test_runner.h"
#include "profile.h"

using namespace std;
#if 0
class ReadingManager {
public:
  ReadingManager()
      : user_page_counts_(MAX_USER_COUNT_ + 1, 0),
        sorted_users_(),
        user_positions_(MAX_USER_COUNT_ + 1, -1) {}

  void Read(int user_id, int page_count) {
    if (user_page_counts_[user_id] == 0) {
      AddUser(user_id);
    }
    user_page_counts_[user_id] = page_count;
    int& position = user_positions_[user_id];
    while (position > 0 && page_count > user_page_counts_[sorted_users_[position - 1]]) {
      SwapUsers(position, position - 1);
    }
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
    int position = user_positions_[user_id];
    while (position < user_count &&
      user_page_counts_[sorted_users_[position]] == page_count) {
      ++position;
    }
    if (position == user_count) {
        return 0;
    }
    // По умолчанию деление целочисленное, поэтому
    // нужно привести числитель к типу double.
    // Простой способ сделать это — умножить его на 1.0.
    return (user_count - position) * 1.0 / (user_count - 1);
  }

private:
  // Статическое поле не принадлежит какому-то конкретному
  // объекту класса. По сути это глобальная переменная,
  // в данном случае константная.
  // Будь она публичной, к ней можно было бы обратиться снаружи
  // следующим образом: ReadingManager::MAX_USER_COUNT.
  static const int MAX_USER_COUNT_ = 100'000;

  vector<int> user_page_counts_;
  vector<int> sorted_users_;   // отсортированы по убыванию количества страниц
  vector<int> user_positions_; // позиции в векторе sorted_users_

  int GetUserCount() const {
    return sorted_users_.size();
  }
  void AddUser(int user_id) {
    sorted_users_.push_back(user_id);
    user_positions_[user_id] = sorted_users_.size() - 1;
  }
  void SwapUsers(int lhs_position, int rhs_position) {
    const int lhs_id = sorted_users_[lhs_position];
    const int rhs_id = sorted_users_[rhs_position];
    swap(sorted_users_[lhs_position], sorted_users_[rhs_position]);
    swap(user_positions_[lhs_id], user_positions_[rhs_id]);
  }
};
#else
class ReadingManager {
public:
  ReadingManager()
      : user_page_counts_(MAX_USER_COUNT_ + 1, 0),
        users_on_page_(MAX_PAGE_COUNT_ + 1, 0),
        user_count_(0) {}

  void Read(int user_id, int page_count) {
    cerr << "Read[" << user_id << "] -> " << page_count << ": user page counts " << user_page_counts_[user_id] << " prev_page_count " << user_page_counts_[user_id];
    if (user_page_counts_[user_id] == 0) {
      ++user_count_; // новый читатель
    } else {
      --users_on_page_[user_page_counts_[user_id]]; // пользователь ушёл с этой страницы
    }
    ++users_on_page_[page_count];        // и остановился на этой
    user_page_counts_[user_id] = page_count;
    cerr << " >>>>  user page counts " << user_page_counts_[user_id] << " prev_page_count " << user_page_counts_[user_id] << "\tpages: {";
    for( int i = 1; i <= 12; ++i)
      cerr << " " << users_on_page_[i];
    cerr << " }" << endl;
  }

  double Cheer(int user_id) const {
    cerr << "Cheer[" << user_id << "]";
    if (user_page_counts_[user_id] == 0) {
      cerr << " not found" << endl;
      return 0;
    }
    const int user_count = GetUserCount();
    if (user_count == 1) {
      cerr << " sole reader" << endl;
      return 1;
    }
    const int page_count = user_page_counts_[user_id];
    cerr << " page_count " << page_count << " user_count " << user_count;
    int looser_count = 0; // счётчик числа пользователей, остановившихся на предыдущих страницах
    for(int i = 1; i < page_count; ++i) {
      looser_count += users_on_page_[i];
    }
    cerr << " looser_count " << looser_count;

    // По умолчанию деление целочисленное, поэтому
    // нужно привести числитель к типу double.
    // Простой способ сделать это — умножить его на 1.0.
    cerr << " ratio " << (looser_count * 1.0 / (user_count - 1)) << endl;
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
#endif
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
  //cerr << "query_count " << query_count << endl;

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
#if 1
  task(cin, cout);
#else
  //srand (time(NULL));
  const int MAX_USER_COUNT = 100'000;
  const int MAX_PAGE_COUNT = 1000;
  const int count = 25000;
  struct query_t { int user_id, page_count; };
  vector<query_t> queries;
  vector<int> user_page_counts_(MAX_USER_COUNT + 1, 0);
  for(int i = 0; i < count; ++i) {
    int user_id = rand() % MAX_USER_COUNT + 1;
    if(user_page_counts_[user_id] < MAX_PAGE_COUNT) {
      int a = user_page_counts_[user_id] + 1;
      int b = MAX_PAGE_COUNT - user_page_counts_[user_id];
      #if 1
      int page_count = user_page_counts_[user_id] + rand() % (b) + 1;
      #else
      int page_count = user_page_counts_[user_id] + 1;
      #endif
      //cerr << "READ[" << user_id << "] " << page_count << " [" << a << ", " << b << "]" << endl;
      user_page_counts_[user_id] = page_count;
      queries.push_back({user_id, page_count});
    }
  }

  {
    stringstream stream;
    stream << (queries.size()) << "\n";
    for(auto it = queries.begin(); it != queries.end(); ++it) {
      stream << "READ " << it->user_id << " " << it->page_count << '\n';
    }
    task(stream, cout);
  }
  {
    stringstream stream;
    stream << (queries.size() * 2) << "\n";
    for(auto it = queries.begin(); it != queries.end(); ++it) {
      stream << "READ " << it->user_id << " " << it->page_count << '\n';
    }
    for(auto it = queries.begin(); it != queries.end(); ++it) {
      stream << "CHEER " << it->user_id << '\n';
      //cerr << "CHEER[" << it->user_id << "] " << endl;
    }
    task(stream, cout);
  }
#endif
  return 0;
}
