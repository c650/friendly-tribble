#include "./html.hpp"

#include <vector>
#include <unordered_map>
#include <stdexcept>

namespace hpbeo = HTMLParser::BaseElementObject;

hpbeo::BaseElementObject();
hpbeo::~BaseElementObject();

hpbeo::BaseElementObject(BaseElementObject&& other);

hpbeo::BaseElementObject&
hpbeo::operator=(BaseElementObject&& other);

/* this should throw some error when the HTML is invalid. */
hpbeo::BaseElementObject(const std::string& raw_html)
	:

hpbeo::BaseElementObject(const std::string& raw_html, BaseElementObjectPointer _parent);

/* should allow access to attributes... */
std::string&
hpbeo::operator[](const std::string&);

/* const access involves making*/
const std::string&
hpbeo::operator[](const std::string&) const;

std::vector<hpbeo::BaseElementObjectPointer>&&
hpbeo::css(const std::string& pattern) const;
