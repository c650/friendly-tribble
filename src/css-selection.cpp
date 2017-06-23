/*
	css-selection.cpp
	Copyright (C) c650, 2017
*/
#include "./include/html.hpp"
#include "./include/helpers.hpp"
#include "./include/css-selection.hpp"

#include <vector>
#include <string>

/* std::transform */
#include <algorithm>

/* std::tolower */
#include <cctype>

namespace CssSelection {

	class TagSelector : BaseSelector {
		std::string tag_to_match;

	public:

		TagSelector(const std::string& _tag) : BaseSelector(), tag_to_match(_tag) {
			std::transform(tag_to_match.begin(), tag_to_match.end(), tag_to_match.begin(), ::tolower);
		}
		~TagSelector() {}

		bool matches(const HTMLParser::Element* const element) const {
			return element->get_tag_name() == tag_to_match;
		}
	};

	class ClassIDSelector : BaseSelector {
		std::string name; /* class or id without . or # */
		bool is_class;

	public:

		/*
			constructor
			@param str the class or id (e.g., #id or .class)
		*/
		ClassIDSelector(const std::string& str) : BaseSelector() {
			if (str.length() < 2) {
				throw std::invalid_argument("str is too short to be a class or id.");
			}
			name = str.substr(1);
			is_class = str.front() == '.';
		}

		~ClassIDSelector() {}

		bool matches(const HTMLParser::Element* const element) const {
			if (!element->has_attribute(is_class ? "class" : "id"))
				return false;

			bool yes = false;
			const std::string& attr = (*element)[is_class ? "class" : "id"];
			general_regex_search(attr.begin(), attr.end(), std::regex("\\s*([-_a-zA-Z0-9])\\s*"), [&yes, this](std::smatch m, size_t _){
				yes = yes || m[1] == this->name;
			});
			return yes;
		}
	};
};
