#pragma once

#include "date.h"

#include <set>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <iostream> // TODO
#include <algorithm>

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
    //cout << "RemoveIf" << endl;
    for(auto db_it = begin(storage); db_it != end(storage); ) {
      //cout << " " << db_it->first << ":";
      for(auto it = begin(db_it->second.events); it != end(db_it->second.events); ) {
        //cout << " {" << *it;
        if(predicate(db_it->first, *it)) {
          //cout << " +";
          db_it->second.event_set.erase(*it);
          it = db_it->second.events.erase(it);
          ++count;
        } else {
          //cout << " -";
         ++it;
        }
        //cout << " }: ";
      }
      //cout << db_it->second.event_set.size() << " entries overall";
      if(db_it->second.event_set.empty()) {
        //cout << " (entry erased)";
        db_it = storage.erase(db_it);
      } else {
        ++db_it;
      }
      //cout << endl;
    }
    return count;
  }

  template<class Predicate>
  vector<pair<Date, string>> FindIf(Predicate predicate) const {
    vector<pair<Date, string>> result;
    //cout << "FindIf" << endl;
    for(auto db_it = begin(storage); db_it != end(storage); ++db_it) {
      //cout << " " << db_it->first << ":";
      for(auto it = begin(db_it->second.events); it != end(db_it->second.events); ++it) {
        //cout << " {" << *it;
        if(predicate(db_it->first, *it)) {
          //cout << " +";
          result.push_back({db_it->first, *it});
        }
        //cout << " }: ";
      }
      //cout << endl;
    }
    return result;
  }

private:
  map<Date, Events> storage;
};

ostream& operator<<(ostream& os, const pair<Date, string>& p);
