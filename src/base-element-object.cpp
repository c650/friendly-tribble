#include "./html.hpp"

#include <vector>
#include <queue>
#include <unordered_map>
#include <stdexcept>
#include <regex>
#include <utility>

namespace hp = HTMLParser;
namespace hpbeo = hp::BaseElementObject;

hpbeo::BaseElementObject();
hpbeo::~BaseElementObject();

/* this should throw some error when the HTML is invalid. */
hpbeo::BaseElementObject(const std::string& raw_html)
	: hpbeo::BaseElementObject(raw_html, null) {}

hpbeo::BaseElementObject(const std::string& raw_html, BaseElementObjectPointer _parent)
: parent(_parent) {
	parse_raw_html(raw_html);
}

std::vector<hp::BaseElementObjectPointer>&& hpbeo::css(const std::string& pattern) const {
	std::vector<BaseElementObjectPointer> results; /* all matches to pattern go here. */

	// do all the stuff.

	return results;
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

void hpbeo::set_parent(hp::BaseElementObjectPointer parent) {
	this->parent = parent;
}

const hp::BaseElementObjectPointer hpbeo::get_parent(void) const {
	return this->parent;
}

void hpbeo::set_tag_name(const std::string& name) {
	this->tag_name = name;
}

const std::string& hpbeo::get_tag_name(void) const {
	return this->tag_name;
}
