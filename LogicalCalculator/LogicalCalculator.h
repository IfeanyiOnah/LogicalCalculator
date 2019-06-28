#pragma once
#include<iostream>
#include<exception>
#include<sstream>

//class for error handling
class execution_error : std::exception
{
public:
	execution_error(const std::string& s) :str(s) {}

	char const* what()const {
		return str.c_str();
	}

private:
	const std::string str;
};

void error_msg(const std::string& s) {
	throw execution_error(s);
}

void error_msg(const std::string& s1, const std::string& s2) {
	error_msg(s1 + s2);
}

void error_msg(const std::string& s1, const int i) {
	std::stringstream s;
	s << s1 << ": " << i;
	error_msg(s.str());
}
