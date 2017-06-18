#include "./include/html.hpp"
#include "./include/debug.hpp"

#include <vector>
#include <queue>
#include <unordered_map>
#include <stdexcept>
#include <regex>
#include <utility>

#include <iostream>

namespace hp = HTMLParser;

struct match_type {
	size_t index, len;
	std::string tag_name;
	std::string attributes;
	bool is_beginning_tag;
};

template<class BidirIt>
static std::vector<match_type> find_all_tags(BidirIt first, BidirIt last, const std::regex& pattern, bool is_beginning_tag);

static std::vector<hp::BaseElementObjectPointer> process_tags(const std::string& raw_html, std::vector<match_type>::iterator begin, std::vector<match_type>::iterator end);

template<class BidirIt>
static std::unordered_map<std::string,std::string> scan_attributes(BidirIt first, BidirIt last);

template<class BidirIt, class Func>
static void general_regex_search(BidirIt first, BidirIt last, const std::regex& pattern, Func f);

hp::BaseElementObjectPointer hp::Document::parse_raw_html(const std::string& raw_html) {
	const static std::regex start_tag_regex("<([a-zA-Z]+[1-6]?)([^>])*>"); // change from .* to [^>]* because I'm dumb.
	const static std::regex   end_tag_regex("</([a-zA-Z]+[1-6]?)>");

	/* this can be much better than it is now. */
	std::vector<match_type> beginnings = find_all_tags(raw_html.begin(), raw_html.end(), start_tag_regex, true);
	std::vector<match_type> ends       = find_all_tags(raw_html.begin(), raw_html.end(),  end_tag_regex, false);

	if (beginnings.size() != ends.size()) {
		std::cout << beginnings.size() << " " << ends.size() << '\n';
		throw html_parse_error("Open/closing tags unmatched.");
	}

	/* nothing else to parse...? */
	if (beginnings.empty()) {
		return nullptr;
	}

	std::vector<match_type> all = beginnings;
	all.insert(all.end(), ends.begin(), ends.end());

	std::sort(all.begin(), all.end(), [](const match_type& a, const match_type& b){return a.index < b.index;});
	for (auto& m : all) {
		std::cout << raw_html.substr(m.index, m.len) << '\n';
	}

	return process_tags(raw_html, all.begin(), all.end()).front();
}

template<class BidirIt>
static std::vector<match_type> find_all_tags(BidirIt first, BidirIt last, const std::regex& pattern, bool is_beginning_tag) {
	std::vector<match_type> results;

	general_regex_search(first, last, pattern, [is_beginning_tag, &results](const std::smatch& match, const size_t& offset){
		results.push_back({ offset + match.prefix().length(), match.str().length(), match[1].str(), is_beginning_tag ? match[2].str() : "", is_beginning_tag });
		debug_print("Found tag: " + match.str() + " at: " + std::to_string(results.back().index) + '\n');
	});

	return results;
};

static std::vector<hp::BaseElementObjectPointer> process_tags(const std::string& raw_html, std::vector<match_type>::iterator begin, std::vector<match_type>::iterator end) {
	std::vector<hp::BaseElementObjectPointer> top_level_elements;

	auto start = begin;
	auto finish = end-1;

	int count = 1;

	while (start != end) {
		if (!start->is_beginning_tag) {
			++start;
			continue;
		}
		for (finish = start+1; finish != end; ++finish) {
			if (!finish->is_beginning_tag) {
				--count;
			} else {
				++count;
			}
			if (count <= 0)
				break;
		}

		debug_print("start tag: " + start->tag_name + " and end tag: " + finish->tag_name + "\n");

		// do math for start to finish of entire element and stuff within
		size_t length = finish->index - start->index - start->len;

		debug_print("length = " + std::to_string(length) + '\n');

		top_level_elements.push_back(new hp::BaseElementObject(start->tag_name, raw_html.substr(start->index + start->len, length)));

		// take care of element's children
		for (hp::BaseElementObjectPointer elem : process_tags(raw_html, start+1, finish))
			top_level_elements.back()->add_child(elem);

		// TODO: Read attributes.
		top_level_elements.back()->set_attributes(scan_attributes(raw_html.begin() + start->index, raw_html.begin() + start->index + start->len));

		if (top_level_elements.back()->has_attribute("class")) {
			debug_print("\thas class: " + (*top_level_elements.back())["class"] + '\n');
		}

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

template<class BidirIt, class Func>
static void general_regex_search(BidirIt first, BidirIt last, const std::regex& pattern, Func f) {
	std::smatch match;
	size_t offset = 0;
	while (std::regex_search(first + offset, last, match, pattern)) {

		f(match, offset);

		offset += match.position() + match.length();
	}
}
