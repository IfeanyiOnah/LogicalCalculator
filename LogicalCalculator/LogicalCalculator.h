#pragma once
#include<iostream>
#include<exception>
#include<sstream>
#include<string>


class Token_stream;
class Variable_base;

//class for error handling
class execution_error : std::exception
{
public:
	int er_no;
	execution_error(const std::string& s, int err = 0) :str(s), er_no(err) {}
	~execution_error() {}
	char const* what()const {
		return str.c_str();
	}

private:
	const std::string str;
};
//function declarations
void error_msg(const std::string& s);

void error_msg(const std::string& s1, const std::string& s2);

void error_msg(const std::string& s1, const int i);

void keep_window_open();

void keep_window_open(const std::string& s);



int expression(Token_stream& ts, Variable_base& vb);
int primary(Token_stream& ts, Variable_base& vb);
int Term1(Token_stream& ts, Variable_base& vb);
int Term2(Token_stream& ts, Variable_base& vb);
int Term3(Token_stream& ts, Variable_base& vb);
void execute_cal();