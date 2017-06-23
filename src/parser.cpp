/*
	parser.cpp
	Copyright (C) c650, 2017
*/

#include "./include/html.hpp"
#include "./include/helpers.hpp"
#include "./include/debug.hpp"

#include <vector>
#include <queue>
#include <unordered_map>
#include <stdexcept>
#include <regex>
#include <utility>

/* std::tolower */
#include <cctype>

/* std::transform */
#include <algorithm>

namespace hp = HTMLParser;

/* TODO: change from std::string copies to iterators... */
struct match_type {
	size_t index, len;
	std::string tag_name;
	std::string attributes;
	bool is_beginning_tag;
	bool is_lone_tag; /* e.g., <img /> */
};

template<class BidirIt>
static std::vector<match_type> find_all_tags(BidirIt first, BidirIt last, bool is_beginning_tag);

static std::vector<hp::BaseElementObjectPointer>
process_tags(const std::string& raw_html, std::vector<match_type>::iterator begin, std::vector<match_type>::iterator end);

template<class BidirIt>
static std::unordered_map<std::string,std::string> scan_attributes(BidirIt first, BidirIt last);

hp::BaseElementObjectPointer hp::Document::parse_raw_html(const std::string& raw_html) {

	std::vector<match_type> beginnings = find_all_tags(raw_html.begin(), raw_html.end(), true);
	std::vector<match_type> ends       = find_all_tags(raw_html.begin(), raw_html.end(), false);

	/* nothing else to parse...? */
	if (beginnings.empty()) {
		return nullptr;
	}

	std::vector<match_type> all = beginnings;
	all.insert(all.end(), ends.begin(), ends.end());

	std::sort(all.begin(), all.end(), [](const match_type& a, const match_type& b){return a.index < b.index;});
	for (auto& m : all) {
		debug_print(raw_html.substr(m.index, m.len) + '\n');
	}

	return process_tags(raw_html, all.begin(), all.end()).front();
}

template<class BidirIt>
static std::vector<match_type> find_all_tags(BidirIt first, BidirIt last, bool is_beginning_tag) {
	const static std::regex start_tag_regex("<([a-zA-Z]+[1-6]?)\\s*([^>]*)>"); /* change from .* to [^>]* because I'm dumb. */
	const static std::regex   end_tag_regex("</([a-zA-Z]+[1-6]?)>");

	const std::regex& pattern = is_beginning_tag ? start_tag_regex : end_tag_regex;

	std::vector<match_type> results;

	general_regex_search(first, last, pattern, [is_beginning_tag, &results](const std::smatch& match, const size_t& offset){

		results.push_back({ offset + match.prefix().length(),                    /* index */
		                    match.str().length(),                                /* len */
							match[1].str(),                                      /* tag_name */
							is_beginning_tag ? match[2].str() : "",              /* attributes */
							is_beginning_tag,                                    /* is_beginning_tag */
							false /* 6/22/17 9:24PM -- eval below */ });         /* is_lone_tag */

		results.back().is_lone_tag = is_beginning_tag && (match[2].str().back() == '/' || hp::check_lone_tags(results.back().tag_name));

		debug_print("Found tag: " + match.str() + " at: " + std::to_string(results.back().index) + '\n');
		if (results.back().is_lone_tag)
			debug_print("\tTag is a lone tag.\n");

		debug_print("Submatches:\n");
		for (size_t i = 0; i < match.size(); ++i) {
			debug_print("\t" + std::to_string(i+1) + " " + match[i].str() + "\n");
		}
	});

	return results;
};

static std::vector<hp::BaseElementObjectPointer> process_tags(const std::string& raw_html, std::vector<match_type>::iterator begin, std::vector<match_type>::iterator end) {
	std::vector<hp::BaseElementObjectPointer> top_level_elements;

	auto start = begin;
	auto finish = end;

	while (start != end) {
		if (!start->is_beginning_tag) {
			++start;
			continue;
		}

		int count = !start->is_lone_tag; /* if the tag is lone, count = 0 and loop never starts; otherwise, count is 1 */
		finish = start;
		while (count) {
			if (++finish == end) {
				throw hp::html_parse_error("Uneven tags or really crappy C++.");
			}

			if (finish->is_lone_tag) {
				continue;
			}

			finish->is_beginning_tag ? ++count : --count;
		}

		debug_print("start tag: " + start->tag_name + " and end tag: " + finish->tag_name + "\n");

		// do math for length of text area within tags.
		size_t length;
		if (start != finish) {
		 	length = finish->index - start->index - start->len;
		} else {
			length = 0;
		}

		debug_print("length = " + std::to_string(length) + '\n');

		/* create the new element. */
		hp::BaseElementObjectPointer element = new hp::BaseElementObject(start->tag_name, raw_html.substr(start->index + start->len, length));

		/* scan attributes */
		element->set_attributes(scan_attributes(raw_html.begin() + start->index, raw_html.begin() + start->index + start->len));

		/* take care of element's children */
		if (!start->is_lone_tag) {
			for (hp::BaseElementObjectPointer elem : process_tags(raw_html, start+1, finish)) {
				element->add_child(elem);
			}
		}

		if (element->has_attribute("class")) {
			debug_print("\thas class: " + (*element)["class"] + '\n');
		}

		/*
			add the new element to a std::vector of top-level elements.
			NOTE: not necessarily "top-level", but same-level in all cases. Remember that this is DFS-ish.
		*/
		top_level_elements.push_back(element);

		start = finish + 1;
	}

	return top_level_elements;
}

template<class BidirIt>
static std::unordered_map<std::string,std::string> scan_attributes(BidirIt first, BidirIt last) {
	static const std::regex attr_pattern("(\\S+)=\"([^\"]*)\"");

	std::unordered_map<std::string, std::string> attributes;

	general_regex_search(first, last, attr_pattern, [&attributes](const std::smatch& match, const size_t& offset){
		attributes[match[1]] = match[2];
	});

	return attributes;
}

bool hp::check_lone_tags(std::string tag) {
	std::transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
	for (auto& t : LONE_TAGS) {
		if (tag == t) return true;
	}
	return false;
}
