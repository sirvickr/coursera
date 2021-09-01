#include "test_runner.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <numeric>

using namespace std;

struct Person {
  string name;
  int age, income;
  bool is_male;
};

ostream& operator<<(ostream& stream, const Person& person) {
  stream << "\n" << left << setw(10) << person.name << " " << setw(2) << person.age << " " << setw(6) << person.income << " " << person.is_male; 
  return stream;
}

using People = vector<Person>;
using View = vector<const Person*>;

class PopulationStats {
public:
  explicit PopulationStats(istream& input): input(input) {
  }

  int operator()() {
    ReadPeople();
    ProcessCommands();
    return 0;
  }

private:
  void ProcessAdultAge(int adult_age) {
    auto adult_begin = lower_bound(
      begin(people), end(people), adult_age, [](const Person& lhs, int age) {
        return lhs.age < age;
      }
    );
    cout << "There are " << std::distance(adult_begin, end(people))
        << " adult people for maturity age " << adult_age << '\n';
  }

  void ProcessIncome(int count) {
    cout << "Top-" << count << " people have total income " << cumulative_wealth[count - 1] << '\n';
  }

  void ProcessPopularName(char gender) {
      const auto& most_popular_name = gender == 'M' ? most_popular_male_name
                                                    : most_popular_female_name;
      if (most_popular_name) {
        cout << "Most popular name among people of gender " << gender << " is "
             << *most_popular_name << '\n';
      } else {
        cout << "No people of gender " << gender << '\n';
      }
  }

  void ProcessCommands() {
    size_t people_count = people.size();
    sort(begin(people), end(people), [](const Person& lhs, const Person& rhs) {
      return lhs.age < rhs.age;
    });

    auto copy_addr = [](const Person& person) { return &person; };

    View by_income(people_count, nullptr);
    transform(begin(people), end(people), begin(by_income), copy_addr);
    sort(begin(by_income), end(by_income), [](const Person* lhs, const Person* rhs) {
      return lhs->income > rhs->income;
    });
    cumulative_wealth.resize(people_count);
    int64_t total = 0;
    for(size_t i = 0; i < people_count; ++i) {
      total += by_income[i]->income;
      cumulative_wealth[i] = total;
    }

    View by_gender(people_count, nullptr);
    transform(begin(people), end(people), begin(by_gender), copy_addr);
    auto gender_bound = partition(begin(by_gender), end(by_gender), [](const Person* p) {
      return p->is_male;
    });
    most_popular_male_name = FindMostPopularName(begin(by_gender), gender_bound);
    most_popular_female_name = FindMostPopularName(gender_bound, end(by_gender));

    for (string command; input >> command; ) {
      if (command == "AGE") {
        int adult_age;
        input >> adult_age;
        ProcessAdultAge(adult_age);
      } else if (command == "WEALTHY") {
        int count;
        input >> count;
        ProcessIncome(count);
      } else if (command == "POPULAR_NAME") {
        char gender;
        input >> gender;
        ProcessPopularName(gender);
      }
    }
  }

  void ReadPeople() {
    int count;
    input >> count;
    people.resize(count);
    for (Person& p : people) {
      char gender;
      input >> p.name >> p.age >> p.income >> gender;
      p.is_male = gender == 'M';
    }
  }

private:
  template <typename Iter>
  optional<string> FindMostPopularName(Iter from, Iter to) {
    sort(from, to, [](const Person* lhs, const Person* rhs) {
      return lhs->name < rhs->name;
    });

    if (from == to)
      return nullopt;

    const string* most_popular_name = &(*from)->name;
    int count = 1;
    for (auto i = from; i != to; ) {
      auto same_name_end = find_if_not(i, to, [i](const Person* p) {
        return p->name == (*i)->name;
      });
      auto cur_name_count = std::distance(i, same_name_end);
      if (cur_name_count > count) {
        count = cur_name_count;
        most_popular_name = &(*i)->name;
      }
      i = same_name_end;
    }
    return *most_popular_name;
  }

private:
  istream& input;
  People people;
  vector<int64_t> cumulative_wealth;
  optional<string> most_popular_male_name, most_popular_female_name;
};

int main() {
  return PopulationStats{cin}();
}
