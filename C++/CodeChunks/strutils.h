#pragma once

struct substring {
	string& s;
	size_t first, last;

	char& operator[](size_t index) {
		return s[first + index];
	}

	const char& operator[](size_t index) const {
		return s[first + index];
	}

	size_t size() const {
		return last - first;
	}
};
