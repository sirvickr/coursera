#include "database.h"
#include "condition_parser.h"

#include <sstream>

ostream& operator<<(ostream& os, const pair<Date, string>& p) {
  os << p.first << " " << p.second;
  return os;
}

void Database::Add(const Date& date, const string& event) {
  auto& db_entry = storage[date];
  if(db_entry.event_set.find(event) == db_entry.event_set.end()) {
    db_entry.event_set.insert(event);
    db_entry.events.push_back(event);
  }
}

void Database::Print(ostream& stream) const {
  for(const auto& item : storage) {
    for(const auto& event : item.second.events) {
      stream << item.first << " " << event << endl;
    }
  }
}

pair<Date, string> Database::Last(const Date& date) const {
  auto it = storage.upper_bound(date);
  if(it == storage.begin()) {
    throw invalid_argument("No entries");
  }
  --it;
  return { it->first, it->second.events.back() };
}
