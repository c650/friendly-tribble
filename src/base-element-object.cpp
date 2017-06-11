#include "./html.hpp"

#include <vector>
#include <queue>
#include <unordered_map>
#include <stdexcept>
#include <regex>
#include <utility>

namespace hp = HTMLParser;

hp::BaseElementObject::BaseElementObject();
hp::BaseElementObject::~BaseElementObject();

/* this should throw some error when the HTML is invalid. */
hp::BaseElementObject::BaseElementObject(const std::string& raw_html)
	: hp::BaseElementObject::BaseElementObject(raw_html, null) {}

hp::BaseElementObject::BaseElementObject(const std::string& raw_html, BaseElementObjectPointer _parent)
: parent(_parent) {
	parse_raw_html(raw_html);
}

std::vector<hp::BaseElementObjectPointer>&& hp::BaseElementObject::css(const std::string& pattern) const {
	std::vector<BaseElementObjectPointer> results; /* all matches to pattern go here. */

	// do all the stuff.

	return results;
}

/* should allow access to attributes... */
std::string&
hp::BaseElementObject::operator[](const std::string& attr) {
	return attributes[attr];
}

/* const access involves making*/
const std::string&
hp::BaseElementObject::operator[](const std::string& attr) const  {
	return attributes[attr];
}

void hp::BaseElementObject::add_child(BaseElementObjectPointer child) {
	if (child != nullptr) /* no sense in adding null children. */
		children.push_back(child);
}

void hp::BaseElementObject::set_parent(hp::BaseElementObjectPointer parent) {
	this->parent = parent;
}

const hp::BaseElementObjectPointer hp::BaseElementObject::get_parent(void) const {
	return this->parent;
}

void hp::BaseElementObject::set_tag_name(const std::string& name) {
	this->tag_name = name;
}

const std::string& hp::BaseElementObject::get_tag_name(void) const {
	return this->tag_name;
}
