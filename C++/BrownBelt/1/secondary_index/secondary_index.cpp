#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;
};

class Database {
public:
  bool Put(const Record& record) {
    auto [it, inserted] = storage.insert({record.id, {record, {}, {}, {}}});

    if(!inserted) {
      return false;
    }

    auto& data = it->second;
    Record* ptr = &data.record;
    data.timestamp_iter = timestamp_index.insert({record.timestamp, ptr});
    data.karma_iter = karma_index.insert({record.karma, ptr});
    data.user_iter = user_index.insert({record.user, ptr});
    return true;
  }

  const Record* GetById(const string& id) const {
    if(auto it = storage.find(id); it != storage.end()) {
      return &it->second.record;
    }
    return nullptr;
  }

  bool Erase(const string& id) {
    if(auto it = storage.find(id); it != storage.end()) {
      timestamp_index.erase(it->second.timestamp_iter);
      karma_index.erase(it->second.karma_iter);
      user_index.erase(it->second.user_iter);
      storage.erase(it);
      return true;
    }
    return false;
  }

  template <typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const {
    Range(timestamp_index, low, high, callback);
  }

  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const {
    Range(karma_index, low, high, callback);
  }

  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const {
    Range(user_index, user, user, callback);
  }

  template <typename Map, typename Callback>
  void Range(const Map& m, typename Map::key_type low, typename Map::key_type high, Callback callback) const {
    auto last = m.upper_bound(high);
    for(auto it = m.lower_bound(low); it != last; ++it) {
      if(!callback(*it->second)) {
        break;
      }
    }
  }

private:
  template<typename Type>
  using Index = multimap<Type, Record*>;

  struct Data {
    Record record;
    Index<int>::iterator timestamp_iter;
    Index<int>::iterator karma_iter;
    Index<string>::iterator user_iter;
  };

private:
  unordered_map<string, Data> storage;
  Index<int> timestamp_index;
  Index<int> karma_index;
  Index<string> user_index;
};

void TestRangeBoundaries() {
  Database db;

  // karma
  const int good_karma = 1000;
  const int bad_karma = -10;

  db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
  db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);

  // timestamps
  count = 0;

  db.Put({"id3", "Rec#3", "anon", 1536107263, good_karma});
  db.Put({"id4", "Rec#4", "anon", 1536107266, good_karma});

  count = 0;
  db.RangeByTimestamp(1536107260, 1536107264, [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(3, count);
}

void TestSameUser() {
  Database db;
  db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
  db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

  int count = 0;
  db.AllByUser("master", [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestReplacement() {
  const string final_body = "Feeling sad";

  Database db;
  bool result = db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
  ASSERT(result);
  result = db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
  ASSERT(!result);
  result = db.Erase("id");
  ASSERT(result);
  result = db.Erase("id");
  ASSERT(!result);
  result = db.Put({"id", final_body, "not-master", 1536107260, -10});
  ASSERT(result);

  auto record = db.GetById("id");
  ASSERT(record != nullptr);
  ASSERT_EQUAL(final_body, record->title);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  return 0;
}
