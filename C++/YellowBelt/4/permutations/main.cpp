#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

int main() {
  size_t n;
  cin >> n;
  vector<size_t> v(n);
  iota(rbegin(v), rend(v), 1);
  do {
	  for(int i : v)
	    cout << i << ' ';
	  cout << endl;
  } while(prev_permutation(begin(v), end(v)));

  return 0;
}
