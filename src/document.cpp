/*
	document.cpp
	Copyright (C) c650, 2017
*/
#include "./include/html.hpp"

#include <vector>
#include <queue>
#include <unordered_map>
#include <stdexcept>
#include <regex>
#include <utility>

namespace hp = HTMLParser;

hp::Document::Document(const std::string& raw_html) : Document(parse_raw_html(raw_html)) {}

hp::Document::Document(BaseElementObjectPointer _root) : root(_root) {}
