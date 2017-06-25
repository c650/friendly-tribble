/*
	css-selection.hpp
	Copyright (C) c650, 2017
*/
#ifndef _CSS_SELECTION_H
#define _CSS_SELECTION_H

#include "./html.hpp"

#include <vector>

/* std::runtime_error */
#include <stdexcept>

namespace CssSelection {

	typedef std::runtime_error css_parse_error;
	/*
		Interface to outline basic selector matching.
	*/
	struct BaseSelector {

		BaseSelector() {}
		virtual ~BaseSelector() {}

		virtual bool match(const HTMLParser::Element* const element) const = 0;
	};

	/*
		Groups selectors together.
	*/
	class SelectorGroup {
		std::vector<BaseSelector*> selectors;

	public:
		SelectorGroup(const std::string& str);

		~SelectorGroup();

		void add(BaseSelector* sel);

		bool match(const HTMLParser::Element* const element) const;

	private:
		/*
			parse_selector_substring will parse str into a selector group.

			@param str the string to parse. Should look like "a.link-class[href="https://google.com/"]"
			@param results a vector in which the results of parsing will be stored.
		*/
		void parse_selector_substring(const std::string& str);
	};

	/* this is where the real shit happens man. */
	class SelectorQuery {
		enum Delims {
			DIRECT_DESCENDENT,
			A_DESCENDENT,
			BETWEEN_TWO_DIFF_PATTERNS
		};

		std::vector<SelectorGroup> selector_groups;
		std::vector<Delims> delimeters;

	public:
		/*
			This will split up the query into substrings that are SelectorGroups
			and delimeters.
		*/
		SelectorQuery(const std::string& query);

		std::vector<HTMLParser::ElementPointer> get_matches(HTMLParser::ElementPointer root);

	};
};
#endif
