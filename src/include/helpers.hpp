/*
	helpers.hpp
	Copyright (C) c650, 2017
*/
#ifndef HELPERS_H
#define HELPERS_H

#include <regex>

#include <vector>
#include <string>

template<class BidirIt, class Func>
void general_regex_search(BidirIt first, BidirIt last, const std::regex& pattern, Func f) {
	std::smatch match;
	size_t offset = 0;
	while (std::regex_search(first + offset, last, match, pattern)) {

		f(match, offset);

		offset += match.position() + match.length();
	}
}

template <class BidirIt>
std::vector<std::string> split_on_regex(BidirIt first, BidirIt, last, const std::regex& pattern) {
	std::sregex_token_iterator start(first, last, pattern, -1);
	std::sregex_token_iterator end;

	std::vector<std::string> arr;
	for (; start != end; ++start)
		arr.push_back(*start);

	return arr;
}

#endif
