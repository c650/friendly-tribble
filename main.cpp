/*
	main.cpp
	Copyright (C) c650, 2017
*/
#include "./src/include/html.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(void) {
	// HTMLParser::Document d("<html><head></head><body class=\"woot\"><img src=\"/img.png\"/></body></html>");

	std::string text;

	std::ifstream fs("tests/test.html");

	std::string tmp;
	while(std::getline(fs, tmp))
		text += tmp;

	HTMLParser::Document d(text);

	return 0;
}
