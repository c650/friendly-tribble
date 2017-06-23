/*
	helpers.hpp
	Copyright (C) c650, 2017
*/
#ifndef HELPERS_H
#define HELPERS_H

#include <regex>

template<class BidirIt, class Func>
static void general_regex_search(BidirIt first, BidirIt last, const std::regex& pattern, Func f) {
	std::smatch match;
	size_t offset = 0;
	while (std::regex_search(first + offset, last, match, pattern)) {

		f(match, offset);

		offset += match.position() + match.length();
	}
}

#endif
