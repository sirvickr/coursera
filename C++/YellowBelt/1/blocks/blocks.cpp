#include <iostream>

using namespace std;

int main() {
  int n, r;
  uint64_t sum = 0;
  cin >> n >> r;
  for (int i = 0; i < n; ++i) {
    int w, h, d;
    cin >> w >> h >> d;
    uint64_t v = static_cast<uint64_t>(w) * h * d;
    uint64_t m = static_cast<uint64_t>(v) * r;
    sum += m;
  }

  cout << sum << endl;

  return 0;
}
