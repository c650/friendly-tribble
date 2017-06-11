#include "./html.hpp"

#include <vector>
#include <queue>
#include <unordered_map>
#include <stdexcept>
#include <regex>
#include <utility>

namespace hp = HTMLParser;

typedef std::queue<std::pair<size_t, std::smatch>> match_type;

static match_type do_regex_search(std::string s, const std::regex& pattern);

/* incomplete garbage */
hp::BaseElementObjectPointer hp::Document::parse_raw_html(const std::string& raw_html) {
	const static std::regex start_tag_regex("<([a-zA-Z]+[1-6]?).*>");
	const static std::regex   end_tag_regex("</([a-zA-Z]+[1-6]?)>");

	/* this can be much better than it is now. */
	match_type beginnings = do_regex_search(raw_html, start_tag_regex);
	match_type ends       = do_regex_search(raw_html,   end_tag_regex);

	if (beginnings.size() != ends.size()) {
		throw html_parse_error("Open/closing tags unmatched.");
	}

	/* nothing else to parse...? */
	if (beginnings.empty()) {
		return nullptr;
	}

	size_t req = 1;
	std::pair<size_t, std::smatch> first = beginnings.front();
	beginnings.pop();
}

static match_type do_regex_search(std::string s, const std::regex& pattern) {
	match_type results;

	size_t offset = 0;
	std::smatch match;
	while (std::regex_search(s, match, pattern)) {
		results.push(std::make_pair(offset + match.prefix().length(), match));

		offset += match.prefix().length() + match[0].length();

		s = match.suffix();
	}

	return results;
};
