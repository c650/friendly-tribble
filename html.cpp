#include "./html.hpp"

#include <vector>
#include <queue>
#include <unordered_map>
#include <stdexcept>
#include <regex>
#include <utility>

namespace hpbeo = HTMLParser::BaseElementObject;

hpbeo::BaseElementObject();
hpbeo::~BaseElementObject();

/* this should throw some error when the HTML is invalid. */
hpbeo::BaseElementObject(const std::string& raw_html)
	: hpbeo::BaseElementObject(raw_html, null) {}

hpbeo::BaseElementObject(const std::string& raw_html, BaseElementObjectPointer _parent)
: parent(_parent) {
	parse_raw_html(raw_html);
}

/* incomplete garbage */
void HTMLParser::Document::parse_raw_html(const std::string& raw_html) {
	const static std::regex start_tag_regex("<([a-zA-Z]+[1-6]?).*>");
	const static std::regex   end_tag_regex("</([a-zA-Z]+[1-6]?)>");

	auto do_regex_search = [](std::string s, const std::regex& pattern) {
		std::queue<std::pair<size_t /* loc in orig s*/, std::smatch>> results;

		size_t offset = 0;
		std::smatch match;
		while (std::regex_search(s, match, pattern)) {
			results.push(std::make_pair(offset + match.prefix().length(), match));

			offset += match.prefix().length() + match[0].length();

			s = match.suffix();
		}

		return results;
	}

	/* this can be much better than it is now. */
	auto beginnings = do_regex_search(raw_html, start_tag_regex);
	auto ends       = do_regex_search(raw_html,   end_tag_regex);

	if (beginnings.length() != ends.length()) {
		throw html_parse_error("Open/closing tags unmatched.");
	}

	/* nothing else to parse...? */
	if (beginnings.empty()) {
		return;
	}
	                                /* checks if tags are of same type. */
	if (beginnings.length() == 1 && ends.front().second[1] == beginnings.front().second[1]) {

	}

	size_t req = 1;
	std::pair<size_t, std::smatch> first = beginnings.pop();
}

/* should allow access to attributes... */
std::string&
hpbeo::operator[](const std::string& attr) {
	return attributes[attr];
}

/* const access involves making*/
const std::string&
hpbeo::operator[](const std::string& attr) const  {
	return attributes[attr];
}

void hpbeo::add_child(BaseElementObjectPointer child) {
	if (child != nullptr) /* no sense in adding null children. */
		children.push_back(child);
}

void hpbeo::set_parent(hpbeo::BaseElementObjectPointer parent) {
	this->parent = parent;
}

const hpbeo::BaseElementObjectPointer hpbeo::get_parent(void) const {
	return this->parent;
}

void hpbeo::set_tag_name(const std::string& name) {
	this->tag_name = name;
}

const std::string& hpbeo::get_tag_name(void) const {
	return this->tag_name;
}

std::vector<hpbeo::BaseElementObjectPointer>&& hpbeo::css(const std::string& pattern) const {
	std::vector<BaseElementObjectPointer> results; /* all matches to pattern go here. */

	// do all the stuff.

	return results;
}
