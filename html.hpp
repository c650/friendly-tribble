#ifndef __HTML_HEADER
#define __HTML_HEADER

#include <vector>
#include <unordered_map>
#include <stdexcept>

namespace HTMLParser {

	typedef std::runtime_error html_parse_error;

	class BaseElementObject {

	  public:
		typedef BaseElementObject* BaseElementObjectPointer;

	  private:
		std::vector<BaseElementObjectPointer> children; /* responsible for cleanup */
		BaseElementObjectPointer              parent;   /* NOT responsible for cleanup */

		std::map<std::string,std::string> attributes; /* an element's attributes (e.g., class, id, etc.) */

		std::string tag_name;

	/* PUBLIC METHODS */
	  public:

		BaseElementObject();
		~BaseElementObject();

		/*
			No copy-constructing because then two copies of the same element
			(possibly) point to the same parent element and that could get messy...

			NOTE: May change later...
		*/
		BaseElementObject(BaseElementObject& other) = delete;
		BaseElementObject& operator=(BaseElementObject& other) = delete;

		BaseElementObject(BaseElementObject&& other);

		BaseElementObject& operator=(BaseElementObject&& other);

		/* this should throw some error when the HTML is invalid. */
		BaseElementObject(const std::string& raw_html);

	private:

		BaseElementObject(const std::string& raw_html, BaseElementObjectPointer _parent);

	public:

		/* should allow access to attributes... */
		std::string& operator[](const std::string&);

		/* const access involves making*/
		const std::string& operator[](const std::string&) const;

		/* Arithmetic on elements is undefined. */
		BaseElementObject operator+(const BaseElementObject&) = delete;
		BaseElementObject operator-(const BaseElementObject&) = delete;
		BaseElementObject operator*(const BaseElementObject&) = delete;
		BaseElementObject operator/(const BaseElementObject&) = delete;
		/* += and other sugars shouldn't work with these guys disabled. */

		/* This will return all child elements that meet the CSS pattern. */
		std::vector<BaseElementObjectPointer>&& css(const std::string& pattern) const;

	};
}

#endif
