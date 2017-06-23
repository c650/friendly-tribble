/*
	element.cpp
	Copyright (C) c650, 2017
*/
#include "./include/html.hpp"
#include "./include/helpers.hpp"

#include <vector>
#include <queue>
#include <unordered_map>
#include <stdexcept>
#include <regex>
#include <utility>

/* std::transform */
#include <algorithm>

/* std::tolower */
#include <cctype>

namespace hp = HTMLParser;

/* this should throw some error when the HTML is invalid. */
hp::Element::Element(const std::string& tag_name, const std::string& text)
	: hp::Element::Element(tag_name, text, nullptr) {}

hp::Element::Element(const std::string& _tag_name, const std::string& _text, ElementPointer _parent)
: tag_name(_tag_name), text(_text), parent(_parent) {
	std::transform(tag_name.begin(),tag_name.end(),tag_name.begin(), ::tolower);
}

hp::Element::~Element() {
	for (ElementPointer& child : children) {
		delete child;
		child = nullptr;
	}
}

std::vector<hp::ElementPointer> hp::Element::css(const std::string& pattern) const {

	std::vector<ElementPointer> results; /* all matches to pattern go here. */

	return results;
}

/* should allow access to attributes... */
std::string&
hp::Element::operator[](const std::string& attr) {
	return attributes[attr];
}

/* const access involves making*/
const std::string&
hp::Element::operator[](const std::string& attr) const  {
	return attributes.at(attr);
}

bool hp::Element::has_attribute(const std::string& attr) const {
	try {
		attributes.at(attr);
	} catch (...) {
		return false;
	}
	return true;
}

void hp::Element::add_child(ElementPointer child) {
	if (child != nullptr) {/* no sense in adding null children. */
		children.push_back(child);
		child->set_parent(this);
	}
}

void hp::Element::set_parent(hp::ElementPointer parent) {
	this->parent = parent;
}

const hp::ElementPointer hp::Element::get_parent(void) const {
	return this->parent;
}

void hp::Element::set_tag_name(const std::string& name) {
	this->tag_name = name;
}

const std::string& hp::Element::get_tag_name(void) const {
	return this->tag_name;
}

void hp::Element::set_attributes(std::unordered_map<std::string,std::string>&& new_attrs) {
	this->attributes = new_attrs;
}
