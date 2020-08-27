#pragma once

#include <map>
#include <vector>
#include <sstream>
#include <ostream>

template <typename First, typename Second>
std::ostream& operator <<(std::ostream& out, const std::pair<First, Second>& p);

template <typename T>
std::ostream& operator <<(std::ostream& out, const std::vector<T>& vi);

template <typename Key, typename Value>
std::ostream& operator <<(std::ostream& out, const std::map<Key, Value>& m);

template <typename Collection>
std::string StringifyCollection(const Collection& c, char d) {
	std::stringstream ss;
	bool first = true;
	for(const auto& i : c) {
		if(!first) {
			ss << d;
		}
		first = false;
		ss << i;
	}
	return ss.str();
}

template <typename First, typename Second>
std::ostream& operator <<(std::ostream& out, const std::pair<First, Second>& p) {
	return out << '(' << p.first << ',' << p.second << ')';
}

template <typename T>
std::ostream& operator <<(std::ostream& out, const std::vector<T>& vi) {
	return out << '[' << StringifyCollection(vi, ',') << ']';
}

template <typename Key, typename Value>
std::ostream& operator <<(std::ostream& out, const std::map<Key, Value>& m) {
	return out << '{' << StringifyCollection(m, ',') << '}';
}
