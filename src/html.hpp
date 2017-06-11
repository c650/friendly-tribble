#ifndef __HTML_HEADER
#define __HTML_HEADER

#include <vector>
#include <unordered_map>
#include <stdexcept>

namespace HTMLParser {

	typedef std::runtime_error html_parse_error;

	class BaseElementObject;
	typedef BaseElementObject* BaseElementObjectPointer;

	class Document {
		BaseElementObjectPointer root;

	public:

		Document(const std::string& raw_html);

		Document(BaseElementObjectPointer root);

		Document(Document& other) = delete;
		Document& operator=(Document& other) = delete;

		BaseElementObjectPointer parse_raw_html(const std::string& raw_html);
	};

	class BaseElementObject {

	  private:
		std::vector<BaseElementObjectPointer> children; /* responsible for cleanup */
		BaseElementObjectPointer              parent;   /* NOT responsible for cleanup */

		std::unordered_map<std::string,std::string> attributes; /* an element's attributes (e.g., class, id, etc.) */

		std::string tag_name;

		std::string text;

		/* allow this function to do dirty stuff to BaseElementObjects */
		friend BaseElementObjectPointer Document::parse_raw_html(const std::string& raw_html);

	/* PUBLIC METHODS */
	  public:

		~BaseElementObject();

		/*
			No copy-constructing because then two copies of the same element
			(possibly) point to the same parent element and that could get messy...

			NOTE: May change later...
		*/
		BaseElementObject(BaseElementObject& other) = delete;
		BaseElementObject& operator=(BaseElementObject& other) = delete;

		BaseElementObject(const std::string& tag_name, const std::string& text);

	  private:

		BaseElementObject(const std::string& tag_name, const std::string& text, BaseElementObjectPointer _parent);

	  public:

		/* should allow access to attributes... */
		std::string& operator[](const std::string& attr);

		/* const access involves making*/
		const std::string& operator[](const std::string& attr) const;

		/* checks if attribute exists... */
		bool has_attribute(const std::string& attr) const;

		/* Arithmetic on elements is undefined. */
		BaseElementObject operator+(const BaseElementObject&) = delete;
		BaseElementObject operator-(const BaseElementObject&) = delete;
		BaseElementObject operator*(const BaseElementObject&) = delete;
		BaseElementObject operator/(const BaseElementObject&) = delete;
		/* += and other sugars shouldn't work with these guys disabled. */

		/* This will return all child elements that meet the CSS pattern. */
		std::vector<BaseElementObjectPointer> css(const std::string& pattern) const;

		void add_child(BaseElementObjectPointer child);

		void set_parent(BaseElementObjectPointer parent);
		const BaseElementObjectPointer get_parent(void) const;

		void set_tag_name(const std::string& name);
		const std::string& get_tag_name(void) const;
	};
}

#endif
