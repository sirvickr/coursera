#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

int main() {
  size_t n;
  cin >> n;
  vector<size_t> v(n);
  // http://ru.cppreference.com/w/cpp/algorithm/iota
  // Заполняет диапазон последовательно возрастающими значениями
  iota(rbegin(v), rend(v), 1);
  do {
	  for(int i : v)
	    cout << i << ' ';
	  cout << endl;
	  // http://ru.cppreference.com/w/cpp/algorithm/prev_permutation
	  // Преобразует диапазон в предыдущую (лексикографически) перестановку, если она существует, и возвращает true,
	  // иначе (если не существует) - в последнюю (наибольшую) и возвращает false
  } while(prev_permutation(begin(v), end(v)));

  return 0;
}
