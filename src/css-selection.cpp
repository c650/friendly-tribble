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

/* std::tolower  */
#include <cctype>

/* std::regex    */
#include <regex>

namespace CssSelection {

	class TagSelector : public BaseSelector {
		std::string tag_to_match;

	public:

		TagSelector(const std::string& _tag) : BaseSelector(), tag_to_match(_tag) {
			std::transform(tag_to_match.begin(), tag_to_match.end(), tag_to_match.begin(), ::tolower);
		}
		~TagSelector() {}

		bool match(const HTMLParser::Element* const element) const {
			return element->get_tag_name() == tag_to_match || tag_to_match == "*";
		}
	};

	class ClassIDSelector : public BaseSelector {
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

		bool match(const HTMLParser::Element* const element) const {
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

/*
	SelectorGroup implementations.
*/
	SelectorGroup::SelectorGroup(const std::string& str) {
		parse_selector_substring(str);
	}

	SelectorGroup::~SelectorGroup() {
		for (auto& bs : selectors) {
			delete bs;
			bs = nullptr;
		}
	}

	void SelectorGroup::add(BaseSelector* sel) {
		selectors.push_back(sel);
	}

	bool SelectorGroup::match(const HTMLParser::Element* const element) const {
		bool yes = true;
		for (auto& bs : selectors) {
			yes = yes && bs->match(element);
		}
		return yes;
	}

	void SelectorGroup::parse_selector_substring(const std::string& str) {

		/* Check for a tag */
		const static std::regex tag_pattern("^([^.#\\s]+)(?=[.#\\[]|$)");
		std::vector<std::string> res = general_regex_search(str.begin(),str.end(),tag_pattern);
		if (!res.empty()) {
			this->selectors.push_back(new TagSelector(res.front()));
		}

		const static std::regex class_id_pattern("(?=^|[^.#\\s]+|\\S*)([#.a-zA-Z0-9_]+)");
		res = general_regex_search(str.begin(),str.end(),class_id_pattern);
		if (!res.empty()) {
			this->selectors.push_back(new ClassIDSelector(res.front()));
		}

		// TODO: other attribute things.
	}


/*
	SelectorQuery implementations.
*/
	SelectorQuery::SelectorQuery(const std::string& query) {
		const static std::regex pattern("[\\b\\s,>]+|[^\\b\\s,>]+");
		std::vector<std::string> tmp = split_on_regex(query.begin(), query.end(), pattern);

		for (size_t i = 0; i < tmp.size(); i += 2) {
			this->selector_groups.push_back(SelectorGroup(tmp.at(i)));
		}
		for (size_t i = 1; i < tmp.size(); i += 2) {
			if (tmp.at(i).find(">") != std::string::npos) {
				this->delimeters.push_back(Delims::DIRECT_DESCENDENT);
			} else if (tmp.at(i).find(",") != std::string::npos) {
				this->delimeters.push_back(Delims::BETWEEN_TWO_DIFF_PATTERNS);
			} else if (tmp.at(i).find(" ") != std::string::npos) {
				this->delimeters.push_back(Delims::A_DESCENDENT);
			} else {
				throw css_parse_error("Something went wrong parsing the selector query.");
			}
		}
	}

	std::vector<HTMLParser::ElementPointer> SelectorQuery::get_matches(HTMLParser::ElementPointer root) {
		/*if (root == nullptr)*/ return std::vector<HTMLParser::ElementPointer>(0);
	}
};
