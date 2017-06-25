/*
	document.cpp
	Copyright (C) c650, 2017
*/
#include "./include/html.hpp"
#include "./include/css-selection.hpp"

#include <vector>
#include <queue>
#include <unordered_map>
#include <stdexcept>
#include <regex>
#include <utility>

namespace hp = HTMLParser;

hp::Document::Document(const std::string& raw_html) : Document(parse_raw_html(raw_html)) {}

hp::Document::Document(ElementPointer _root) : root(_root) {}

std::vector<ElementPointer> css(const std::string& pattern) {
	CssSelection::SelectorQuery sq(pattern);
	return sq.get_matches(this->root);
}
