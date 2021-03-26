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
	// 3. Разбить вектор на 3 равные части (в этой задаче гарантируется, что длина передаваемого диапазона является степенью тройки, так что вектор всегда можно разбить на три равные части).
	size_t segment_size = size / 3;
	auto delim1 = next(range_begin, segment_size);
	auto delim2 = next(delim1, segment_size);
	vector<typename RandomIt::value_type> a(range_begin, delim1);
	vector<typename RandomIt::value_type> b(delim1, delim2);
	vector<typename RandomIt::value_type> c(delim2, range_end);
	// 4. Вызвать функцию MergeSort от каждой части вектора.
	MergeSort(begin(a), end(a));
	MergeSort(begin(b), end(b));
	MergeSort(begin(c), end(c));
	// 5. Слить первые две трети вектора с помощью алгоритма merge, сохранив результат во временный вектор с помощью back_inserter.
	vector<typename RandomIt::value_type> temp;
	temp.reserve(segment_size * 2);
	merge(begin(a), end(a), begin(b), end(b), back_inserter(temp));
	// 6. Слить временный вектор из предыдущего пункта с последней третью вектора из п. 2, записав полученный отсортированный диапазон вместо исходного.
	merge(begin(temp), end(temp), begin(c), end(c), range_begin);
}

#if 0
int main() {
	vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1, 5};
	MergeSort(begin(v), end(v));
	for (int x : v) {
		cout << x << " ";
	}
	cout << endl;
	return 0;
}
#endif
