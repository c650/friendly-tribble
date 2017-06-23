/*
	base-element-object.cpp
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

namespace hp = HTMLParser;

/* this should throw some error when the HTML is invalid. */
hp::BaseElementObject::BaseElementObject(const std::string& tag_name, const std::string& text)
	: hp::BaseElementObject::BaseElementObject(tag_name, text, nullptr) {}

hp::BaseElementObject::BaseElementObject(const std::string& _tag_name, const std::string& _text, BaseElementObjectPointer _parent)
: tag_name(_tag_name), text(_text), parent(_parent) {}

hp::BaseElementObject::~BaseElementObject() {
	for (BaseElementObjectPointer& child : children) {
		delete child;
		child = nullptr;
	}
}

std::vector<hp::BaseElementObjectPointer> hp::BaseElementObject::css(const std::string& pattern) const {

	std::vector<BaseElementObjectPointer> results; /* all matches to pattern go here. */

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
	return attributes.at(attr);
}

bool hp::BaseElementObject::has_attribute(const std::string& attr) const {
	try {
		attributes.at(attr);
	} catch (...) {
		return false;
	}
	return true;
}

void hp::BaseElementObject::add_child(BaseElementObjectPointer child) {
	if (child != nullptr) {/* no sense in adding null children. */
		children.push_back(child);
		child->set_parent(this);
	}
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

void hp::BaseElementObject::set_attributes(std::unordered_map<std::string,std::string>&& new_attrs) {
	this->attributes = new_attrs;
}
