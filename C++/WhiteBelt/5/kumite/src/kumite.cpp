#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <tuple>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

class Date {
public:
	Date() {
		year = 1;
		month = 1;
		day = 1;
	}
	Date(int y, int m, int d) {
		year = y;
		month = m;
		day = d;
	}
	int GetYear() const {
		return year;
	}
	int GetMonth() const {
		return month;
	}
	int GetDay() const {
		return day;
	}
private:
	int year;
	int month;
	int day;

	friend bool operator<(const Date& lhs, const Date& rhs) {
		return make_tuple(lhs.year, lhs.month, lhs.day) <
		       make_tuple(rhs.year, rhs.month, rhs.day);
	}
	friend ostream& operator<<(ostream& stream, const Date& date) {
		stream << setfill('0') << setw(4) << date.year << '-' << setw(2) << date.month << '-' << setw(2) << date.day;
		return stream;
	}
	friend istream& operator>>(istream& stream, Date& date) {
#if 1
		string s;
		if(stream >> s) {
			istringstream iss(s);
			if(!(iss >> date.year))
				throw runtime_error("Wrong date format: " + s);

			if(iss.peek() != '-')
				throw runtime_error("Wrong date format: " + s);
			iss.ignore(1);
			if(!(iss >> date.month))
				throw runtime_error("Wrong date format: " + s);

			if(date.month < 1 || date.month > 12)
				throw runtime_error("Month value is invalid: " + to_string(date.month));

			if(iss.peek() != '-')
				throw runtime_error("Wrong date format: " + s);
			iss.ignore(1);
			if(!(iss >> date.day))
				throw runtime_error("Wrong date format: " + s);

			if(date.day < 1 || date.day > 31)
				throw runtime_error("Day value is invalid: " + to_string(date.day));
		} else {
			throw runtime_error("Wrong date format: " + s);
		}
#else
		stream >> date.year;
		stream.ignore(1);
		stream >> date.month;
		stream.ignore(1);
		stream >> date.day;
#endif
		return stream;
	}
};

class Database {
public:
	using Events = set<string>;
	Database() {
		ifstream file(fileName);
		if(file) {
			//cout << "Read DB:" << endl;
			string line;
			Date date;
			string event;
			while (getline(file, line)) {
				istringstream iss(line);
				iss >> date;
				//cout << date << ": ";
				while(iss >> event) {
					//cout << event << " ";
					table[date].insert(event);
				}
				//cout << endl;
			}
		}
	}

	~Database() {
		ofstream file(fileName);
		if(file) {
			//cout << "Write DB" << endl;
			for(const auto& record : table) {
				file << record.first;
				for(const auto& event : record.second) {
					file << " " << event;
				}
				file << endl;
			}
		}
	}

	void AddEvent(const Date& date, const string& event) {
		//cout << "AddEvent: " << date << " " << event << endl;
		table[date].insert(event);
	}

	bool DeleteEvent(const Date& date, const string& event) {
		//cout << "DeleteEvent: " << date << " " << event << endl;
		const auto recordIt = table.find(date);
		if(recordIt == cend(table)) {
			return false;
		}
		const auto eventIt = find(cbegin(recordIt->second), cend(recordIt->second), event);
		if(eventIt == cend(recordIt->second)) {
			return false;
		}
		recordIt->second.erase(eventIt);
		return true;
	}

	int  DeleteDate(const Date& date) {
		//cout << "DeleteDate: " << date << endl;
		auto recordIt = table.find(date);
		int count = recordIt->second.size();
		if(recordIt != cend(table)) {
			recordIt->second.clear();
		}
		return count;
	}

	Events Find(const Date& date) const {
		//cout << "Find: " << date << endl;
		auto recordIt = table.find(date);
		if(recordIt == cend(table)) {
			return {};
		}
		return recordIt->second;
	}

	void Print() const {
		//cout << "Print" << endl;
		for(const auto& record : table) {
			///cout << record.first;
			for(const auto& event : record.second) {
				cout << record.first << " " << event << endl;
			}
			///cout << endl;
		}
	}
private:
	const string fileName = "database.txt";
	map<Date, Events> table;
};

void Task(istream& input) {
	Database db;
	// - ���������� �������:                        Add ���� �������
	// - �������� �������:                          Del ���� �������
	// - �������� ���� ������� �� ���������� ����:  Del ����
	// - ����� ������� �� ���������� ����:          Find ����
	// - ������ ���� ������� �� ��� ����:           Print
	string command;
	while (getline(input, command)) {
		istringstream iss(command);
		string id;
		iss >> id;
		if(id == "Add") {
			string event;
#if 0
			Date date;
			iss >> date >> event;
			//cout << "\"Add " << date << " " << event << "\"" << endl;
			db.AddEvent(date, event);
#else			// �������� ��� ��������� Case 9 (������� �� operatot<<(Date), ������ ��� ����, ����� ��������� �������� �������� ������ ��� �������� "�������� ����")
			int year = 0, month = 0, day = 0;
			string s;
			if(iss >> s) {
				istringstream tmp(s);
				tmp >> year;

				if(tmp.peek() != '-')
					throw runtime_error("Wrong date format: " + s);
				tmp.ignore(1);
				tmp >> month;

				if(month < 1 || month > 12)
					throw runtime_error("Month value is invalid: " + to_string(month));

				if(tmp.peek() != '-')
					throw runtime_error("Wrong date format: " + s);
				tmp.ignore(1);
				tmp >> day;

				if(day < 1 || day > 31)
					throw runtime_error("Day value is invalid: " + to_string(day));
			} else {
				throw runtime_error("Wrong date format: " + s);
			}
			Date date(year, month, day);
			iss >> event;
			if(event.empty()) {
				throw runtime_error("Wrong date format: " + s);
			} else {
				db.AddEvent(date, event);
			}
			db.AddEvent(date, event);
#endif
		} else if(id == "Del") {
			Date date;
			string event;
			iss >> date >> event;
			if(event.empty()) {
				cout << "Deleted " << db.DeleteDate(date) << " events" << endl;
			} else {
				if(db.DeleteEvent(date, event)) {
					cout << "Deleted successfully" << endl;
				} else {
					cout << "Event not found" << endl;
				}
			}
		} else if(id == "Find") {
			Date date;
			iss >> date;
			const auto events = db.Find(date);
			for(const auto& event : events) {
				cout << event << endl;
			}
		} else if(id == "Print") {
			db.Print();
		} else if(!id.empty()) {
			cout << "Unknown command: " << id << endl;
		}
		//cout << "id \"" << id << "\" date \"" << date << "\" event \"" << event << "\"" << endl;
	}
}

int main() {
	try {
#if 0
		ifstream input("input.txt");
		Task(input);
#else
		Task(cin);
#endif
	} catch(runtime_error& e) {
		cout << e.what() << endl;
	}
	return 0;
}
