/*
	html.hpp
	Copyright (C) c650, 2017
*/
#ifndef __HTML_HEADER
#define __HTML_HEADER

#include <vector>
#include <unordered_map>
#include <stdexcept>

namespace HTMLParser {

	typedef std::runtime_error html_parse_error;

	class Element;
	typedef Element* ElementPointer;

	class Document {
		ElementPointer root;

	public:

		Document(const std::string& raw_html);
		Document(ElementPointer _root);

		Document(Document& other) = delete;
		Document& operator=(Document& other) = delete;

		std::vector<ElementPointer> css(const std::string& pattern);

	protected:
		friend Element;
		ElementPointer parse_raw_html(const std::string& raw_html);
	};

	class Element {

	  private:
		std::vector<ElementPointer> children; /* responsible for cleanup */
		ElementPointer              parent;   /* NOT responsible for cleanup */

		std::unordered_map<std::string,std::string> attributes; /* an element's attributes (e.g., class, id, etc.) */

		std::string tag_name;

		std::string text;

		/* allow this function to do dirty stuff to Elements */
		friend ElementPointer Document::parse_raw_html(const std::string& raw_html);

	/* PUBLIC METHODS */
	  public:

		~Element();

		/*
			No copy-constructing because then two copies of the same element
			(possibly) point to the same parent element and that could get messy...

			NOTE: May change later...
		*/
		Element(Element& other) = delete;
		Element& operator=(Element& other) = delete;

		Element(const std::string& tag_name, const std::string& text);

	  private:

		Element(const std::string& tag_name, const std::string& text, ElementPointer _parent);

	  public:

		/* should allow access to attributes... */
		std::string& operator[](const std::string& attr);

		/* const access involves making*/
		const std::string& operator[](const std::string& attr) const;

		/* checks if attribute exists... */
		bool has_attribute(const std::string& attr) const;

		/* Arithmetic on elements is undefined. */
		Element operator+(const Element&) = delete;
		Element operator-(const Element&) = delete;
		Element operator*(const Element&) = delete;
		Element operator/(const Element&) = delete;
		/* += and other sugars shouldn't work with these guys disabled. */

		void add_child(ElementPointer child);

		void set_parent(ElementPointer parent);
		const ElementPointer get_parent(void) const;

		void set_tag_name(const std::string& name);
		const std::string& get_tag_name(void) const;

		void set_attributes(std::unordered_map<std::string,std::string>&& new_attrs);
	};

	/*
		These are all the tags that are not allowed to or might not have closing tags.

		They are called empty elements or (some of them) void elements, but you'll see
		"lone_tag" in the source code.
	*/
	const std::vector<std::string> LONE_TAGS = {"link","meta","br","hr","base", "basefont", "br", "wbr", "source","img",
	                                 "param","track","area","input","col","frame","embed","keygen"};

	/*
		function to check the LONE_TAGS above against some tag.

		@param tag the string of tag to check (case insensitive)
		@return true if the tag is in the LONE_TAGS list.
	*/
	bool check_lone_tags(std::string tag);
}

#endif
