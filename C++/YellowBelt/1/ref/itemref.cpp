#include <iostream>
#include <map>

using namespace std;



template<typename K, typename V>
V& GetRefStrict(map<K, V>& m, K key) {
    if (m.count(key) == 0) {
        throw runtime_error("no such key in dictionary");
    }
    return m[key];
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
