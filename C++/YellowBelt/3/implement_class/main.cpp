#include "phone_number.h"

#include <iostream>

int main() {
  PhoneNumber pn("+7-921-1234567");
  cout << "country " << pn.GetCountryCode() << endl;
  cout << "city " << pn.GetCityCode() << endl;
  cout << "local " << pn.GetLocalNumber() << endl;
  cout << "international " << pn.GetInternationalNumber() << endl;
  return 0;
}
