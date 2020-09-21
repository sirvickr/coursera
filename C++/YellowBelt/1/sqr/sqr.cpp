#include <iostream>
#include <vector>
#include <map>

using namespace std;

template<typename Seq> Seq SeqSqr(const Seq& seq);
template<typename T> T Sqr(const T& x);
template<typename K, typename V> pair<K, V> Sqr(const pair<K, V>& x);
template<typename T> vector<T> Sqr(const vector<T>& x);
template<typename K, typename V> map<K, V> Sqr(const map<K, V>& x);

template<typename Seq>
Seq SeqSqr(const Seq& seq) {
	Seq result = seq;
	for(auto& item: result) {
		item = Sqr(item);
	}
	return result;
}

template<typename T>
T Sqr(const T& x) {
	return x * x;
}

template<typename K, typename V>
pair<K, V> Sqr(const pair<K, V>& x) {
	return {x.first * x.first, x.second * x.second};
}

template <typename T>
vector<T> Sqr(const vector<T>& x) {
	return SeqSqr(x);
}

template<typename K, typename V>
map<K, V> Sqr(const map<K, V>& x) {
	map<K, V> result = x;
	for(auto& item: result) {
		item.second = Sqr(item.second);
	}
	return result;
}

int main() {
	vector<int> v = {1, 2, 3};
	cout << "vector:";
	for (int x : Sqr(v)) {
	  cout << ' ' << x;
	}
	cout << endl;

	map<int, pair<int, int>> map_of_pairs = {
	  {4, {2, 2}},
	  {7, {4, 3}}
	};
	cout << "map of pairs:" << endl;
	for (const auto& x : Sqr(map_of_pairs)) {
	  cout << x.first << ' ' << x.second.first << ' ' << x.second.second << endl;
	}

  return 0;
}
