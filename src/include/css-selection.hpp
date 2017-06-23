/*
	css-selection.hpp
	Copyright (C) c650, 2017
*/
#ifndef _CSS_SELECTION_H
#define _CSS_SELECTION_H

#include "./html.hpp"

namespace CssSelection {
	/*
		Interface to outline basic selector matching.
	*/
	struct BaseSelector {

		BaseSelector() {}
		virtual ~BaseSelector() = 0;

		virtual bool matches(const HTMLParser::Element* const element) const = 0;

	};
};
#endif
