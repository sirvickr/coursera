#include <iostream>
#include <set>

using namespace std;

set<int>::const_iterator FindNearestElement(const set<int>& numbers, int border) {
  auto p = numbers.equal_range(border);
  // точное совпадение
  if(p.first != p.second) {
    return p.first;
  }
  // если непустое - вернуть 1й элемент, иначе end() = begin()
  if(p.first == begin(numbers)) {
    return p.first;
  }
  // тут уже точно не пустое - если end(), вернуть последний эелемент
  auto left = prev(p.first);
  if(p.first == end(numbers)) {
    return left;
  }
  // находимся посреди непустого множества - вернуть
  // ближайший элемент, либо наименьший из двух ближайших
  if(border - *left <= *p.first - border) {
    return left; // это наименьший из двух ближайших
  }
  return p.first; // это единственный ближайший элемент
}

#if 0
int main() {
  set<int> numbers = {1, 4, 6};
  cout <<
      *FindNearestElement(numbers, 2) << " \n" <<
      *FindNearestElement(numbers, 0) << " " <<
      *FindNearestElement(numbers, 3) << " " <<
      *FindNearestElement(numbers, 5) << " " <<
      *FindNearestElement(numbers, 6) << " " <<
      *FindNearestElement(numbers, 100) << endl;

  set<int> empty_set;

  cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;
  return 0;
}
#endif
