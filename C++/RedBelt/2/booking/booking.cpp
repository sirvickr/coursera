#include <iomanip>
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>

#include "test_runner.h"
#include "profile.h"

using namespace std;

class BookingManager {
public:
  BookingManager()
  : current_time_(0)
  {}

  void Book(const string& hotel_name, int book_time, int client_id, int room_count) {
    current_time_ = book_time;
    cerr << "Book: hotel_name " << hotel_name << " book_time " << book_time << " client_id " << client_id << " room_count " << room_count << endl;
  }

  int Clients(const string& hotel_name) {
    cerr << "Clients: hotel_name " << hotel_name << endl;
    return 0;
  }

  int Rooms(const string& hotel_name) {
    cerr << "Rooms: hotel_name " << hotel_name << endl;
    return 0;
  }

private:
  int current_time_;
};

void TestSample() {
  BookingManager manager;
  ASSERT_EQUAL(manager.Clients("Marriott"), 0);
  ASSERT_EQUAL(manager.Rooms("Marriott"), 0);
  manager.Book("FourSeasons", 10, 1, 2);
  manager.Book("Marriott", 10, 1, 1);
  manager.Book("FourSeasons", 86409, 2, 1);
  ASSERT_EQUAL(manager.Clients("FourSeasons"), 2);
  ASSERT_EQUAL(manager.Rooms("FourSeasons"), 3);
  ASSERT_EQUAL(manager.Clients("Marriott"), 1);
  manager.Book("Marriott", 86410, 2, 10);
  ASSERT_EQUAL(manager.Rooms("FourSeasons"), 1);
  ASSERT_EQUAL(manager.Rooms("Marriott"), 10);
}

void task(istream& is, ostream& os) {
  BookingManager manager;

  int query_count;
  is >> query_count;

  LOG_DURATION("total")
  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    is >> query_type;

    if (query_type == "BOOK") {
      int book_time, client_id, room_count;
      string hotel_name;
      is >> book_time >> hotel_name >> client_id >> room_count;
      manager.Book(hotel_name, book_time, client_id, room_count);
    } else if (query_type == "CLIENTS") {
      string hotel_name;
      is >> hotel_name;
      os << manager.Clients(hotel_name) << "\n";
    } else if (query_type == "ROOMS") {
      string hotel_name;
      is >> hotel_name;
      os << manager.Rooms(hotel_name) << "\n";
    }
  }
}

int main() {
  //TestRunner tr;
  //RUN_TEST(tr, TestSample);
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  task(cin, cout);

  return 0;
}
