#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <tuple>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

class Date {
public:
  Date() {
    year = 1;
    month = 1;
    day = 1;
  }
  Date(int y, int m, int d) {
    year = y;
    month = m;
    day = d;
  }
  int GetYear() const {
    return year;
  }
  int GetMonth() const {
    return month;
  }
  int GetDay() const {
    return day;
  }
private:
  int year;
  int month;
  int day;

  friend bool operator<(const Date& lhs, const Date& rhs) {
    return make_tuple(lhs.year, lhs.month, lhs.day) <
           make_tuple(rhs.year, rhs.month, rhs.day);
  }
  friend ostream& operator<<(ostream& stream, const Date& date) {
    stream << setfill('0') << setw(4) << date.year << '-' << setw(2) << date.month << '-' << setw(2) << date.day;
    return stream;
  }
  friend istream& operator>>(istream& stream, Date& date) {
    string s;
    if(stream >> s) {
      istringstream iss(s);
      if(!(iss >> date.year))
        throw logic_error("Wrong date format: " + s);

      if(iss.peek() != '-')
        throw logic_error("Wrong date format: " + s);
      iss.ignore(1);
      if(!(iss >> date.month))
        throw logic_error("Wrong date format: " + s);

      if(date.month < 1 || date.month > 12)
        throw logic_error("Month value is invalid: " + to_string(date.month));

      if(iss.peek() != '-')
        throw logic_error("Wrong date format: " + s);
      iss.ignore(1);
      if(!(iss >> date.day))
        throw logic_error("Wrong date format: " + s);

      if(date.day < 1 || date.day > 31)
        throw logic_error("Day value is invalid: " + to_string(date.day));
    } else {
      throw logic_error("Wrong date format: " + s);
    }
    return stream;
  }
};

class Database {
public:
  using Events = set<string>;

  void AddEvent(const Date& date, const string& event) {
    storage[date].insert(event);
  }

  bool DeleteEvent(const Date& date, const string& event) {
    const auto recordIt = storage.find(date);
    if(recordIt == cend(storage)) {
      return false;
    }
    const auto eventIt = find(cbegin(recordIt->second), cend(recordIt->second), event);
    if(eventIt == cend(recordIt->second)) {
      return false;
    }
    recordIt->second.erase(eventIt);
    return true;
  }

  int DeleteDate(const Date& date) {
    int count = 0;
    auto recordIt = storage.find(date);
    if(recordIt != cend(storage)) {
      count = recordIt->second.size();
      storage.erase(recordIt);
    }
    return count;
  }

  Events Find(const Date& date) const {
    auto recordIt = storage.find(date);
    if(recordIt != cend(storage)) {
      return recordIt->second;
    }
    return {};
  }

  void Print() const {
    for(const auto& item : storage) {
      for(const auto& event : item.second) {
        cout << item.first << " " << event << endl;
      }
    }
  }

private:
  map<Date, Events> storage;
};

void Task(istream& input) {
  Database db;
  // - ���������� �������:                        Add ���� �������
  // - �������� �������:                          Del ���� �������
  // - �������� ���� ������� �� ���������� ����:  Del ����
  // - ����� ������� �� ���������� ����:          Find ����
  // - ������ ���� ������� �� ��� ����:           Print
  string command_line;
  while (getline(input, command_line)) {
    istringstream ss(command_line);

    string command;
    ss >> command;

    if(command == "Add") {

      string event;
      int year = 0, month = 0, day = 0;
      string s;
      if(ss >> s) {
        istringstream tmp(s);
        tmp >> year;

        if(tmp.peek() != '-')
          throw logic_error("Wrong date format: " + s);
        tmp.ignore(1);
        tmp >> month;

        if(month < 1 || month > 12)
          throw logic_error("Month value is invalid: " + to_string(month));

        if(tmp.peek() != '-')
          throw logic_error("Wrong date format: " + s);
        tmp.ignore(1);
        tmp >> day;

        if(day < 1 || day > 31)
          throw logic_error("Day value is invalid: " + to_string(day));
      } else {
        throw logic_error("Wrong date format: " + s);
      }
      Date date(year, month, day);
      ss >> event;
      if(event.empty()) {
        throw logic_error("Wrong date format: " + s);
      } else {
        db.AddEvent(date, event);
      }
      db.AddEvent(date, event);

    } else if(command == "Del") {

      Date date;
      string event;
      ss >> date >> event;
      if(event.empty()) {
        cout << "Deleted " << db.DeleteDate(date) << " events" << endl;
      } else {
        if(db.DeleteEvent(date, event)) {
          cout << "Deleted successfully" << endl;
        } else {
          cout << "Event not found" << endl;
        }
      }

    } else if(command == "Find") {

      Date date;
      ss >> date;
      const auto events = db.Find(date);
      for(const auto& event : events) {
        cout << event << endl;
      }

    } else if(command == "Print") {

      db.Print();

    } else if(!command.empty()) {

      throw logic_error("Unknown command: " + command);

    }
    //cout << "id \"" << command << "\" date \"" << date << "\" event \"" << event << "\"" << endl;
  }
}

int main() {
  try {
    Task(cin);
  } catch(logic_error& e) {
    cout << e.what() << endl;
  }
  return 0;
}
