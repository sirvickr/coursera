#include "database.h"
#include "date.h"
#include "condition_parser.h"
#include "node.h"
#include "test_runner.h"

#include <iostream>
#include <stdexcept>

using namespace std;

string ParseEvent(istream& is) {
  string str;
  getline(is >> ws, str);
  return str;
}

void TestAll();

void Task(istream& is, ostream& os) {
  Database db;

  for (string line; getline(is, line); ) {
    istringstream is(line);

    string command;
    is >> command;
    if (command == "Add") {
      const auto date = ParseDate(is);
      const auto event = ParseEvent(is);
      db.Add(date, event);
    } else if (command == "Print") {
      db.Print(os);
    } else if (command == "Del") {
      auto condition = ParseCondition(is);
      auto predicate = [condition](const Date& date, const string& event) {
        return condition->Evaluate(date, event);
      };
      int count = db.RemoveIf(predicate);
      os << "Removed " << count << " entries" << endl;
    } else if (command == "Find") {
      auto condition = ParseCondition(is);
      auto predicate = [condition](const Date& date, const string& event) {
        return condition->Evaluate(date, event);
      };

      const auto entries = db.FindIf(predicate);
      for (const auto& entry : entries) {
        os << entry << endl;
      }
      os << "Found " << entries.size() << " entries" << endl;
    } else if (command == "Last") {
      try {
        os << db.Last(ParseDate(is)) << endl;
      } catch (invalid_argument&) {
        os << "No entries" << endl;
      }
    } else if (command.empty()) {
      continue;
    } else {
      throw logic_error("Unknown command: " + command);
    }
  }
}

int main() {
  TestAll();
  Task(cin, cout);
  return 0;
}

void TestParseEvent() {
  {
    istringstream is("event");
    AssertEqual(ParseEvent(is), "event", "Parse event without leading spaces");
  }
  {
    istringstream is("   sport event ");
    AssertEqual(ParseEvent(is), "sport event ", "Parse event with leading spaces");
  }
  {
    istringstream is("  first event  \n  second event");
    vector<string> events;
    events.push_back(ParseEvent(is));
    events.push_back(ParseEvent(is));
    AssertEqual(events, vector<string>{"first event  ", "second event"}, "Parse multiple events");
  }
}

void TestParseDate() {
  {
    istringstream is("2017-01-01");
    AssertEqual(ParseDate(is), Date{2017, 1, 1}, "Parse date: 2017-01-01");
  }
  {
    istringstream is("0-2-3");
    AssertEqual(ParseDate(is), Date{0, 2, 3}, "Parse date: 0-2-3");
  }
}

void TestTask() {
  {
    istringstream is("Add 2017-06-01 1st of June\n"
                     "Add 2017-07-08 8th of July\n"
                     "Add 2017-07-08 Someone's birthday\n"
                     "Del date == 2017-07-08\n");
    ostringstream result;
    Task(is, result);
    ostringstream expected("Removed 2 entries\n");
//    cout << "result:   '" << result.str() << "'" << endl;
//    cout << "expected: '" << expected.str() << "'" << endl;
    AssertEqual(result.str(), expected.str(), "Case 1");
  }
  {
    istringstream is(
        "Add 2017-01-01 Holiday\n"
        "Add 2017-03-08 Holiday\n"
        "Add 2017-1-1 New Year\n"
        "Add 2017-1-1 New Year\n"
        "Print\n");
    ostringstream result;
    Task(is, result);
    ostringstream expected(
        "2017-01-01 Holiday\n"
        "2017-01-01 New Year\n"
        "2017-03-08 Holiday\n");
    AssertEqual(result.str(), expected.str(), "Case 2");
  }
  {
    istringstream is(
        "Add 2017-01-01 Holiday\n"
        "Add 2017-03-08 Holiday\n"
        "Add 2017-01-01 New Year\n"
        "Find event != \"working day\"\n"
        "Add 2017-05-09 Holiday\n");
    ostringstream result;
    Task(is, result);
    ostringstream expected(
        "2017-01-01 Holiday\n"
        "2017-01-01 New Year\n"
        "2017-03-08 Holiday\n"
        "Found 3 entries\n");
    AssertEqual(result.str(), expected.str(), "Case 3");
  }
  {
    istringstream is(
        "Add 2017-01-01 New Year\n"
        "Add 2017-03-08 Holiday\n"
        "Add 2017-01-01 Holiday\n"
        "Last 2016-12-31\n"
        "Last 2017-01-01\n"
        "Last 2017-06-01\n"
        "Add 2017-05-09 Holiday\n");
    ostringstream result;
    Task(is, result);
    ostringstream expected(
        "No entries\n"
        "2017-01-01 Holiday\n"
        "2017-03-08 Holiday\n");
    AssertEqual(result.str(), expected.str(), "Case 4");
  }
  {
    istringstream is(
        "Add 2017-11-21 Tuesday\n"
        "Add 2017-11-20 Monday\n"
        "Add 2017-11-21 Weekly meeting\n"
        "Print\n"
        "Find event != \"Weekly meeting\"\n"
        "Last 2017-11-30\n"
        "Del date > 2017-11-20\n"
        "Last 2017-11-30\n"
        "Last 2017-11-01\n");
    ostringstream result;
    Task(is, result);
    ostringstream expected(
        "2017-11-20 Monday\n"
        "2017-11-21 Tuesday\n"
        "2017-11-21 Weekly meeting\n"
        "2017-11-20 Monday\n"
        "2017-11-21 Tuesday\n"
        "Found 2 entries\n"
        "2017-11-21 Weekly meeting\n"
        "Removed 2 entries\n"
        "2017-11-20 Monday\n"
        "No entries\n");
    AssertEqual(result.str(), expected.str(), "Case 5");
  }
}

void TestAll() {
  TestRunner tr;
  tr.RunTest(TestParseDate, "TestParseDate");
  tr.RunTest(TestParseEvent, "TestParseEvent");
  tr.RunTest(TestParseCondition, "TestParseCondition");
  tr.RunTest(TestTask, "TestTask");
}
