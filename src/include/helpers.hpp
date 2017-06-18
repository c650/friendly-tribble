#ifndef HELPERS_H
#define HELPERS_H

#include <regex>

template<class BidirIt, class Func>
void general_regex_search(BidirIt first, BidirIt last, const std::regex& pattern, Func f);

#endif
