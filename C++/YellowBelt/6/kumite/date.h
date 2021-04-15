#pragma once

#include <istream>
#include <ostream>
#include <string>
#include <tuple>

using namespace std;

class Date {
public:
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

  auto rank() const {
    return std::tie(year, month, day);
  }

private:
  int year;
  int month;
  int day;
};

bool operator<(const Date& lhs, const Date& rhs);
bool operator<=(const Date& lhs, const Date& rhs);
bool operator>(const Date& lhs, const Date& rhs);
bool operator>=(const Date& lhs, const Date& rhs);
bool operator==(const Date& lhs, const Date& rhs);
bool operator!=(const Date& lhs, const Date& rhs);
ostream& operator<<(ostream& stream, const Date& date);

Date ParseDate(istream& is);
