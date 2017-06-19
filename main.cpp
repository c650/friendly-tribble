#include "./src/include/html.hpp"
#include <iostream>

int main(void) {
	HTMLParser::Document d("<html><head></head><body class=\"woot\"><img src=\"/img.png\"/></body></html>");
	return 0;
}
