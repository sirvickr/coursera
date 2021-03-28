#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

struct Prefix {
  std::string data;
};

struct Comp {
  bool operator() ( const string& s, const Prefix& prefix ) const { return s.substr(0, prefix.data.size()) < prefix.data; }
  bool operator() ( const Prefix& prefix, const string& s ) const { return prefix.data < s.substr(0, prefix.data.size()); }
};

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, const string& prefix) {
  return equal_range(range_begin, range_end, Prefix{prefix}, Comp{});
}

#if 0
int main() {
  const vector<string> sorted_strings = {"moscow", "motovilikha", "murmansk"};

  const auto mo_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo");
  for (auto it = mo_result.first; it != mo_result.second; ++it) {
    cout << *it << " ";
  }
  cout << endl;

  const auto mt_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), "mt");
  cout << (mt_result.first - begin(sorted_strings)) << " " <<
      (mt_result.second - begin(sorted_strings)) << endl;

  const auto na_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), "na");
  cout << (na_result.first - begin(sorted_strings)) << " " <<
      (na_result.second - begin(sorted_strings)) << endl;

  return 0;
}
#endif
