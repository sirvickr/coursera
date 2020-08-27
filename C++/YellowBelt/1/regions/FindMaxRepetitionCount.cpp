#include <algorithm>

tuple<string, string, map<Lang, string>, int64_t> regionRank(const Region& region) {
  return {region.std_name, region.parent_std_name, region.names, region.population};
}

bool operator<(const Region& a, const Region& b) {
  return regionRank(a) < regionRank(b);
}

int FindMaxRepetitionCount(const vector<Region>& regions) {
  if(regions.empty())
    return 0;
  map<Region, int> repetitions;
  for(const auto& region: regions) {
    repetitions[region]++;
  }
  return max_element(begin(repetitions), end(repetitions), [](const pair<Region, int> a, const pair<Region, int> b){ return a.second < b.second; })->second;
}
