/*
	css-selection.hpp
	Copyright (C) c650, 2017
*/
#ifndef _CSS_SELECTION_H
#define _CSS_SELECTION_H

#include "./html.hpp"

#include <vector>

namespace CssSelection {
	/*
		Interface to outline basic selector matching.
	*/
	struct BaseSelector {

		BaseSelector() {}
		virtual ~BaseSelector() = 0;

		virtual bool matches(const HTMLParser::Element* const element) const = 0;

	};

	/*
		Groups selectors together.
	*/
	class SelectorGroup {
		std::vector<BaseSelector*> selectors;

	public:
		SelectorGroup();
		SelectorGroup(const std::string& str);

		~SelectorGroup();

		void add(BaseSelector* sel);

		bool match(const HTMLParser::Element* const element) const;
	};

	/*
		parse_selector_substring will parse str into a selector group.

		@param str the string to parse. Should look like "a.link-class[href="https://google.com/"]"
		@param results a vector in which the results of parsing will be stored.
	*/
	void parse_selector_substring(const std::string& str, std::vector<BaseSelector*>& result);

	/*
		parse_selector_whole_string will parse str into a bunch of selector groups.

		@param str the string to parse. Ex. "div.foo > div.bar, a.sauce[alt="foobar"]"
		@param results a vector of SelectorGroups parsed out from str.
	*/
	void parse_selector_whole_string(const std::string& str, std::vector<SelectorGroup>& results);
};
#endif
