#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
	// 1. Если диапазон содержит меньше 2 элементов, выйти из функции.
	size_t size = distance(range_begin, range_end);
	if(size < 2)
		return;
	// 2. Создать вектор, содержащий все элементы текущего диапазона.
	vector<typename RandomIt::value_type> elements(range_begin, range_end);
	// 3. Разбить вектор на две равные части (в этой задаче гарантируется, что длина передаваемого диапазона является степенью двойки, так что вектор всегда можно разбить на две равные части).
	auto middle = next(range_begin, size / 2);
	vector<typename RandomIt::value_type> a(range_begin, middle);
	vector<typename RandomIt::value_type> b(middle, range_end);
	// 4. Вызвать функцию MergeSort от каждой половины вектора.
	MergeSort(begin(a), end(a));
	MergeSort(begin(b), end(b));
	// 5. С помощью алгоритма std::merge слить отсортированные половины, записав полученный отсортированный диапазон вместо исходного.
	merge(begin(a), end(a), begin(b), end(b), range_begin);
}

#if 0
int main() {
	vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1};
	MergeSort(begin(v), end(v));
	for (int x : v) {
		cout << x << " ";
	}
	cout << endl;
	return 0;
}
#endif
