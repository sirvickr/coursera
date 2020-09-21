#include <iostream>

using namespace std;

int main() {
  int n, r;
  int64_t sum = 0;
  cin >> n >> r;
  for (int i = 0; i < n; ++i) {
    int w, h, d;
    cin >> w >> h >> d;
    int64_t v = static_cast<int64_t>(w) * h * d;
    int64_t m = v * r;
    sum += m;
  }

  cout << sum << endl;

  return 0;
}
