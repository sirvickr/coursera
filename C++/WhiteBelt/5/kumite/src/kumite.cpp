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
  void SetYear(int value) {
    year = value;
  }

  int GetMonth() const {
    return month;
  }
  void SetMonth(int value) {
    month = value;
  }

  int GetDay() const {
    return day;
  }
  void SetDay(int value) {
    day = value;
  }

private:
  int year;
  int month;
  int day;
};

bool operator<(const Date& lhs, const Date& rhs) {
  return make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) <
         make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

ostream& operator<<(ostream& stream, const Date& date) {
  stream << setfill('0') << setw(4) << date.GetYear() << '-' << setw(2) << date.GetMonth() << '-' << setw(2) << date.GetDay();
  return stream;
}

istream& operator>>(istream& stream, Date& date) {
  string s;
  stream >> s;
  const string wrong_date_format = "Wrong date format: " + s;
  istringstream date_stream(s);

  int year;
  if(!(date_stream >> year))
    throw logic_error(wrong_date_format);
  if(date_stream.peek() != '-')
    throw logic_error(wrong_date_format);
  date_stream.ignore(1);

  int month;
  if(!(date_stream >> month))
    throw logic_error(wrong_date_format);
  if(date_stream.peek() != '-')
    throw logic_error(wrong_date_format);
  date_stream.ignore(1);

  int day;
  if(!(date_stream >> day))
    throw logic_error(wrong_date_format);
  if(!date_stream.eof())
      throw logic_error(wrong_date_format);

  if(month < 1 || month > 12)
    throw logic_error("Month value is invalid: " + to_string(month));
  if(day < 1 || day > 31)
    throw logic_error("Day value is invalid: " + to_string(day));

  date.SetYear(year);
  date.SetMonth(month);
  date.SetDay(day);

  return stream;
}

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

      Date date;
      string event;
      ss >> date >> event;
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
