#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

// * класс Matrix
class Matrix {
public:
  Matrix(int m = 0, int n = 0) {
    Reset(m, n);
  }

  void Reset(int m, int n) {
    CheckDimentions(m, n);
    this->m = m;
    this->n = n;
    data.resize(m * n, 0);
  }

  int At(int i, int j) const {
    CheckIndices(i, j);
    return data[i * n + j];
  }

  int& At(int i, int j) {
    CheckIndices(i, j);
    return data[i * n + j];
  }

  int GetNumRows() const {
    return m;
  }

  int GetNumColumns() const {
    return n;
  }

  friend istream& operator>>(istream& stream, Matrix& matrix) {
    int m, n;
    stream >> m >> n;
    matrix.Reset(m, n);
    for(int& i: matrix.data)
      stream >> i;
    return stream;
  }

  friend ostream& operator<<(ostream& stream, const Matrix& matrix) {
    stream << matrix.m << ' ' << matrix.n;
    for(size_t i = 0; i < matrix.data.size(); ++i) {
      if(0 == (i % matrix.n))
        stream << '\n';
      stream << matrix.data[i] << ' ';
    }
    return stream;
  }

  friend bool operator==(const Matrix& lhs, const Matrix& rhs) {
    if(lhs.m != rhs.m || lhs.n != rhs.n)
      return false;
    return lhs.data == rhs.data;
  }

  friend Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
    if(lhs.m != rhs.m || lhs.n != rhs.n) {
      ostringstream oss;
      oss << "lhs[" << lhs.m << "][" << lhs.n << "] rhs[" << rhs.m << "][" << rhs.n << "]";
      throw invalid_argument(oss.str());
    }
    Matrix result(lhs.m, lhs.n);
    transform(cbegin(lhs.data), cend(lhs.data), cbegin(rhs.data), begin(result.data), std::plus<>{});
    return result;
  }

private:
  void CheckDimentions(int m, int n) const {
    if(m < 0 || n < 0) {
      ostringstream oss;
      oss << "num rows " << m << ", num columns " << n;
      throw out_of_range(oss.str());
    }
  }

  void CheckIndices(int i, int j) const {
    if(i < 0 || j < 0 || i >= m || j >= n) {
      ostringstream oss;
      oss << "row " << i << ", col " << j << "; matrix[" << m << "][" << n << "]";
      throw out_of_range(oss.str());
    }
  }

  int m, n;
  vector<int> data;
};

int main() {
  Matrix one;
  Matrix two;

  cin >> one >> two;
  cout << one + two << endl;
  return 0;
}
