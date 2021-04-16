#pragma once

#include "date.h"

#include <set>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <ostream>

using namespace std;

class Database {
public:
  struct Events {
    set<string> event_set;
    list<string> events;
  };

  void Add(const Date& date, const string& event);

  void Print(ostream& stream) const;

  pair<Date, string> Last(const Date& date) const;

  template<class Predicate>
  int RemoveIf(Predicate predicate) {
    int count = 0;
    for(auto db_it = begin(storage); db_it != end(storage); ) {
      for(auto it = begin(db_it->second.events); it != end(db_it->second.events); ) {
        if(predicate(db_it->first, *it)) {
          db_it->second.event_set.erase(*it);
          it = db_it->second.events.erase(it);
          ++count;
        } else {
         ++it;
        }
      }
      if(db_it->second.event_set.empty()) {
        db_it = storage.erase(db_it);
      } else {
        ++db_it;
      }
    }
    return count;
  }

  template<class Predicate>
  vector<pair<Date, string>> FindIf(Predicate predicate) const {
    vector<pair<Date, string>> result;
    for(auto db_it = begin(storage); db_it != end(storage); ++db_it) {
      for(auto it = begin(db_it->second.events); it != end(db_it->second.events); ++it) {
        if(predicate(db_it->first, *it)) {
          result.push_back({db_it->first, *it});
        }
      }
    }
    return result;
  }

private:
  map<Date, Events> storage;
};

ostream& operator<<(ostream& os, const pair<Date, string>& p);
