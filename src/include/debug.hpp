/*
	debug.hpp
	Copyright (C) c650, 2017
*/
#ifndef DEBUG_PRINTOUT_H
#define DEBUG_PRINTOUT_H

#include <iostream>
#include <string>

void debug_print(const std::string& msg) {
	#ifdef DEBUG_PRINTOUT
		std::cout << msg;
	#endif
}

#endif
