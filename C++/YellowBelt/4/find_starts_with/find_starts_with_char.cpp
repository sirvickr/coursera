#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

struct Comp {
  bool operator() ( const string& s, char prefix ) const { return s[0] < prefix; }
  bool operator() ( char prefix, const string& s ) const { return prefix < s[0]; }
};

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, char prefix) {
  return equal_range(range_begin, range_end, prefix, Comp{});
}

#if 0
int main() {
  const vector<string> sorted_strings = {"moscow", "murmansk", "vologda"};
  auto m_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), 'm');
  for (auto it = m_result.first; it != m_result.second; ++it) {
    cout << *it << " ";
  }
  cout << endl;
  const auto p_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), 'p');
  cout << (p_result.first - begin(sorted_strings)) << " " <<
      (p_result.second - begin(sorted_strings)) << endl;

  const auto z_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), 'z');
  cout << (z_result.first - begin(sorted_strings)) << " " <<
      (z_result.second - begin(sorted_strings)) << endl;

  return 0;
}
#endif

