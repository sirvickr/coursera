#include <iostream>
#include <map>

using namespace std;

template<typename K, typename V> V& GetRefStrict(map<K, V>& m, const K& key);

template<typename K, typename V>
V& GetRefStrict(map<K, V>& m, const K& key) {
	auto it = m.find(key);
	if(it == m.end()) {
		throw runtime_error("not found");
	}
	return it->second;
}

#if 1 // 0 to send
int main() {
	map<int, string> m = {{0, "value"}};
	string& item = GetRefStrict(m, 0);
	item = "newvalue";
	cout << m[0] << endl; // выведет newvalue
  return 0;
}
#endif
