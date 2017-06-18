#include "./include/helpers.hpp"

#include <regex>
#include <string>

template<class BidirIt, class Func>
static void general_regex_search(BidirIt first, BidirIt last, const std::regex& pattern, Func f) {
	std::smatch match;
	size_t offset = 0;
	while (std::regex_search(first + offset, last, match, pattern)) {

		f(match, offset);

		offset += match.position() + match.length();
	}
}
