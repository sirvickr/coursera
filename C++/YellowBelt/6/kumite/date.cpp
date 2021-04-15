#include "date.h"

#include <iomanip>

bool operator<(const Date& lhs, const Date& rhs) {
  return lhs.rank() < rhs.rank();
}

bool operator<=(const Date& lhs, const Date& rhs) {
  return lhs.rank() <= rhs.rank();
}

bool operator>(const Date& lhs, const Date& rhs) {
  return lhs.rank() > rhs.rank();
}

bool operator>=(const Date& lhs, const Date& rhs) {
  return lhs.rank() >= rhs.rank();
}

bool operator==(const Date& lhs, const Date& rhs) {
  return lhs.rank() == rhs.rank();
}

bool operator!=(const Date& lhs, const Date& rhs) {
  return lhs.rank() != rhs.rank();
}

ostream& operator<<(ostream& stream, const Date& date) {
  stream << setfill('0') << setw(4) << date.GetYear() << '-' << setw(2) << date.GetMonth() << '-' << setw(2) << date.GetDay();
  return stream;
}
#if 1
Date ParseDate(istream& is) {
  string date;
  is >> date;
  const string wrong_date_format = "Wrong date format: " + date;
  istringstream date_stream(date);

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

  return Date(year, month, day);
}
#else
Date ParseDate(const string& date) {
  const string wrong_date_format = "Wrong date format: " + date;
  istringstream date_stream(date);

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

  return Date(year, month, day);
}
#endif
