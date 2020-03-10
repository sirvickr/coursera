#include <iomanip>
#include <sstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <utility>

#include "test_runner.h"
#include "profile.h"

using namespace std;

struct BookingItem {
  //int book_time = 0;
  int client_id = 0;
  int room_count = 0;
};

ostream& operator <<(ostream& stream, const BookingItem& item) {
  stream << "(" << /*item.book_time << ", " <<*/ item.client_id << ", " << item.room_count << ")";
  return stream;
}

/*bool operator <(const BookingItem& lhs, const BookingItem& rhs) {
  return lhs.book_time < rhs.book_time;
}*/

struct Hotel {
  int clients = 0;
  int booked_rooms = 0;
  map<int, BookingItem> booking;
};

class BookingManager {
public:
  BookingManager()
  : current_time_(0)
  {}

  void Book(const string& hotel_name, int book_time, int client_id, int room_count) {
    //cerr << "Book: hotel_name " << hotel_name << " book_time " << book_time << " client_id " << client_id << " room_count " << room_count << endl;
    current_time_ = book_time;
    auto hotel = hotels_.find(hotel_name);
    if(hotel == hotels_.end()) {
      hotel = hotels_.insert({hotel_name, {}}).first;
      cerr << "Book: inserted hotel " << hotel->first << endl;
    } else {
      cerr << "Book: found hotel " << hotel->first << ": clients " << hotel->second.clients << " rooms " << hotel->second.booked_rooms << " bookings " << hotel->second.booking << endl;
    }
    // erase: log(c.size()) + std::distance(first, last)
    //hotel->second.booking.insert({book_time, client_id, room_count});
    hotel->second.booking.insert({book_time, {client_id, room_count}});
  }

  int Clients(const string& hotel_name) {
    //cerr << "Clients: hotel_name " << hotel_name << endl;
    auto hotel = hotels_.find(hotel_name);
    if(hotel == hotels_.end()) {
      return 0;
    }
    cerr << "Clients: found hotel " << hotel->first << ": clients " << hotel->second.clients << " rooms " << hotel->second.booked_rooms << " bookings " << hotel->second.booking << endl;
    return 0;
  }

  int Rooms(const string& hotel_name) {
    //cerr << "Rooms: hotel_name " << hotel_name << endl;
    auto hotel = hotels_.find(hotel_name);
    if(hotel == hotels_.end()) {
      return 0;
    }
    cerr << "Rooms: found hotel " << hotel->first << ": clients " << hotel->second.clients << " rooms " << hotel->second.booked_rooms << " bookings " << hotel->second.booking << endl;
    return 0;
  }

private:
  static const int MAX_TIME_SPAN = 86400;

  int current_time_;
  map<string, Hotel> hotels_;
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
