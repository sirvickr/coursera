#include "phone_number.h"

#include <sstream>

PhoneNumber::PhoneNumber(const string &international_number) {
  if(international_number.empty())
    throw invalid_argument("empty");
  if(international_number.front() != '+')
    throw invalid_argument("no leading '+'");
  istringstream iss(international_number);
  string s;
  iss.ignore(1);
  if(!getline(iss, s, '-') || s.empty()) {
    throw invalid_argument("invalid format (country code)");
  }
  country_code_ = s;
  if(!getline(iss, s, '-') || s.empty()) {
    throw invalid_argument("invalid format (city code)");
  }
  city_code_ = s;
  if(!getline(iss, s) || s.empty()) {
    throw invalid_argument("invalid format (local number)");
  }
  local_number_ = s;
}

string PhoneNumber::GetCountryCode() const {
  return country_code_;
}

string PhoneNumber::GetCityCode() const {
  return city_code_;
}

string PhoneNumber::GetLocalNumber() const {
  return local_number_;
}

string PhoneNumber::GetInternationalNumber() const {
  return '+' + country_code_ + '-' + city_code_ + '-' + local_number_;
}
