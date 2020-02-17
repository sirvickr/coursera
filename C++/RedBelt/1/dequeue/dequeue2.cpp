#include <vector>
#include <sstream>
#include <stdexcept>

using namespace std;

template<typename T>
class Deque {
public:
	explicit Deque() {
	}

	bool Empty() const {
		return vFront.empty() && vBack.empty();
	}
	
	size_t Size() const {
		return vFront.size() + vBack.size();
	}

	const T& At(size_t index) const {
		CheckIndex(index);
		if(index < vFront.size())
			return vFront[vFront.size() - index - 1];
		return vBack[index - vFront.size()];
	}

	T& At(size_t index) {
		CheckIndex(index);
		if(index < vFront.size())
			return vFront[vFront.size() - index - 1];
		return vBack[index - vFront.size()];
	}

	const T& operator[](size_t index) const {
		return At(index);
	}

	T& operator[](size_t index) {
		return At(index);
	}

	T& Front() {
		return At(0);
	}

	const T& Front() const {
		return At(0);
	}

	T& Back() {
		return At(Size() - 1);
	}

	const T& Back() const {
		return At(Size() - 1);
	}

	void PushFront(const T& item) {
		vFront.push_back(item);
	}

	void PushBack(const T& item) {
		vBack.push_back(item);
	}

private:
	vector<T> vFront, vBack;
	void CheckIndex(size_t index) const {
		if(index >= Size()) {
			ostringstream os;
			os << "index = " << index << " is out of range";
			throw out_of_range(os.str());
		}
	}
};
