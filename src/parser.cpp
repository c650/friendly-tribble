#include "./html.hpp"

#include <vector>
#include <queue>
#include <unordered_map>
#include <stdexcept>
#include <regex>
#include <utility>

namespace hp = HTMLParser;

struct match_type {
	size_t index;
	std::smatch match;
	bool is_beginning_tag;
};

static std::vector<match_type> do_regex_search(std::string s, const std::regex& pattern, bool is_beginning_tag);

static std::vector<hp::BaseElementObjectPointer> process_tags(std::vector<match_type>::iterator begin, std::vector<match_type>::iterator end);

/* incomplete garbage */
hp::BaseElementObjectPointer hp::Document::parse_raw_html(const std::string& raw_html) {
	const static std::regex start_tag_regex("<([a-zA-Z]+[1-6]?).*>");
	const static std::regex   end_tag_regex("</([a-zA-Z]+[1-6]?)>");

	/* this can be much better than it is now. */
	std::vector<match_type> beginnings = do_regex_search(raw_html, start_tag_regex, true);
	std::vector<match_type> ends       = do_regex_search(raw_html,  end_tag_regex, false);

	if (beginnings.size() != ends.size()) {
		throw html_parse_error("Open/closing tags unmatched.");
	}

	/* nothing else to parse...? */
	if (beginnings.empty()) {
		return nullptr;
	}

	BaseElementObjectPointer element = nullptr;

	std::vector<match_type> all = beginnings;
	all.insert(all.end(), ends.begin(), ends.end());

	std::sort(all.begin(), all.end(), [](const match_type& a, const match_type& b){return a.index < b.index;});



	return element;
}

static std::vector<match_type> do_regex_search(std::string s, const std::regex& pattern, bool is_beginning_tag) {
	std::vector<match_type> results;

	size_t offset = 0;
	std::smatch match;
	while (std::regex_search(s, match, pattern)) {
		results.push_back({ offset + match.prefix().length(), match, is_beginning_tag });

		offset += match.prefix().length() + match[0].length();

		s = match.suffix();
	}

	return results;
};

static std::vector<hp::BaseElementObjectPointer> process_tags(std::vector<match_type>::iterator begin, std::vector<match_type>::iterator end) {
	std::vector<hp::BaseElementObjectPointer> top_level_elements;

	auto start = begin;
	auto finish = end-1;

	int count = 0;

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

		// do math for start to finish of entire element and stuff within
		size_t length = finish->index - start->index - start->match[0].length();
		top_level_elements.push_back(new hp::BaseElementObject(start->match[1], start->match.suffix().str().substr(0, length)));

		// take care of element's children
		for (hp::BaseElementObjectPointer elem : process_tags(start+1, finish))
			top_level_elements.back()->add_child(elem);

		// TODO: Read attributes.


		start = finish + 1;
	}

	return top_level_elements;
}
