#include "airline_ticket.h"
#include "test_runner.h"

using namespace std;

#define UPDATE_FIELD(ticket, field, values) \
{ \
  auto it = values.find(#field); \
    if (it != values.end()) { \
      istringstream is(it->second); \
      is >> ticket.field; \
    } \
}

bool operator ==(const Date& a, const Date& b) {
  return tie(a.year, a.month, a.day) == tie(b.year, b.month, b.day);
}

istream& operator >>(istream& stream, Date& date) {
  stream >> date.year;
  stream.ignore(1);
  stream >> date.month; 
  stream.ignore(1);
  stream >> date.day; 
  return stream;
}

ostream& operator<<(ostream& stream, const Date& date) {
  stream << date.year << "-" << date.month << "-" << date.day;
  return stream;
}

bool operator ==(const Time& a, const Time& b) {
  return tie(a.hours, a.minutes) == tie(b.hours, b.minutes);
}

istream& operator >>(istream& stream, Time& time) {
  stream >> time.hours; 
  stream.ignore(1);
  stream >> time.minutes; 
  return stream;
}

ostream& operator<<(ostream& stream, const Time& time) {
  stream << time.hours << ":" << time.minutes;
  return stream;
}

void TestUpdate() {
  AirlineTicket t;
  t.price = 0;

  const map<string, string> updates1 = {
    {"departure_date", "2018-2-28"},
    {"departure_time", "17:40"},
  };
  UPDATE_FIELD(t, departure_date, updates1);
  UPDATE_FIELD(t, departure_time, updates1);
  UPDATE_FIELD(t, price, updates1);

  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 0);

  const map<string, string> updates2 = {
    {"price", "12550"},
    {"arrival_time", "20:33"},
  };
  UPDATE_FIELD(t, departure_date, updates2);
  UPDATE_FIELD(t, departure_time, updates2);
  UPDATE_FIELD(t, arrival_time, updates2);
  UPDATE_FIELD(t, price, updates2);

  // updates2 не содержит ключей "departure_date" и "departure_time", поэтому
  // значения этих полей не должны измениться
  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 12550);
  ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestUpdate);
}
