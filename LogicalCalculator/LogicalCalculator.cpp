//-------------implementation of the program based on the design------------------
//In order to implement a program that follow the rule of the grammer in read me 
//the input expression has to be tokenize inform of (kind,value) pair token

#include "LogicalCalculator.h"
#include<vector>
#include<map>

using namespace std;

const char quit = 'q';
const char int_kind = '8';
const string dcl_key = "let";
const char var_def = 'D';
const char name_var = 'N';
const char var_exist = 'S';
const string inst_key = "help";
const char help = 'h';
const char print = '\n';



enum error_numbers{variable_decl = 1,wrong_name_decl};


//class for the token
class Token {
public:

	//modifiable member function
	//construct to make a token from a character(kind) 
	Token(char ch):m_kind(ch), m_value(0),m_name(string()){}

	//construct to make a token from a character and double(value)
	Token(char ch, int val):m_kind(ch), m_value(val),m_name(string()){}

	//construct to make a token from a character and double(value)
	Token(char ch, string str) :m_kind(ch), m_name(str) {}


	//non-modifiable member function
	//function to return kind
	char& kind() { return m_kind; }
	const char& kind()const { return m_kind; }

	//function to return val
	 int& value() { return m_value; }
	 const int& value()const { return m_value; }

	const string& name()const { return m_name; }
	string& name() { return m_name; }

private:
	char m_kind;
	int m_value;
	string m_name;
};

//token stream to hold/buffer the token

class Token_stream {
public:
	//Modifiable member function

	//construct a default token stream
	Token_stream():m_buffer(0), m_full(false){}

	~Token_stream(){}
	//function to put back token
	void put_back(Token& tok) {
		m_buffer = tok;
		m_full = true;
	}

	//get token function: it takes an empty token and return a full token
	Token& get_token();

private:
	Token m_buffer;
	bool m_full;

};

//another alternative input stream for reading from token stream 
// it can also read from cin when the buffer is empty
Token_stream& operator>>( Token_stream& ts, Token& t) {
	t = ts.get_token();
	return ts;
}

//another alternative output stream for writing to token stream
void operator<<(Token_stream& ts, Token& t) {
	ts.put_back(t);

}

bool special_name(char ch)
//this function check if a special char is use for definition
{
	switch (ch) {
	case quit:
		return true;
	default:
		return false;
	}

}
string get_name()
//get any supported name from cin
{
	char ch =0;
	string str;


	cin >> ch;
	if (cin.fail() || cin.bad())error_msg("unable to read from cin");

	if (!isalpha(ch)) {
		ch = 0;
		error_msg("Unacceptable name declaration", wrong_name_decl);
	}
		
	str += ch;
	while (cin.get(ch) && (isalpha(ch) || isdigit(ch))) {

		if (isspace(ch))break;
		str += ch;
	}
		
	 cin.unget();
	return  str;

}
  Token& Token_stream::get_token() {

	if (m_full) {
		m_full = false;
		return m_buffer;
	
	}


	//read a character
	char ch;

	while (cin.get(ch) && isspace(ch) && ch != print);
	if (cin.fail() || cin.bad())error_msg("unable to read from cin");

	switch (ch) {
		//check for any term expect value
	case '|': case '&': case '^': case print: case quit: case '~': case '(': case ')':
	{
		m_buffer = Token(ch);
			return m_buffer;

	}
	case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
	{
		cin.unget();

		int val = 0;
		cin >> val;

		m_buffer = Token(int_kind, val);
		return  m_buffer;
	}
	
	default:
	{
		cin.unget();
		string str = get_name();

		if (str == dcl_key) { // chack if the user wish to declare a value
			str = get_name();
			cin >> ch;
			if (cin.fail() || cin.bad())error_msg("unable to read from cin");

			if (ch != '=') error_msg("bad input: missing '='");
			m_buffer = Token(var_def, str);
			return m_buffer;
		}

		if (str == inst_key) { //prepare a token to print instruction
			m_buffer = Token(help);
			return m_buffer;
		}

		// read the next char to check if the user wishes to assign a value to already declared variable
		// or print the value after
			while(cin.get(ch) && isspace(ch) && ch !=print);
			if (cin.fail() || cin.bad())error_msg("unable to read from cin");

			switch (ch) {
			case '=':
				m_buffer = Token(var_exist, str);
				return m_buffer;
			default:
				// otherwise assume that the valariable already exist and prepare a token for it
				cin.unget();
				m_buffer = Token(name_var, str);
				return m_buffer;
			}
		
		error_msg("bad input");
		break;
	}

	}
}


//data base class to hold the Variable type
class Variable_base {
public:
	Variable_base():variable(map<string,int>()){}
	void set_varible(string& str, int d);  //set variable
	void define_var(string& str, int d); // set non-existing variable
	int get_value(string& name);  //get existing variable with a name

	~Variable_base(){}

private:
	bool is_declared(string& str);
	map<string, int> variable;
};



bool Variable_base::is_declared(string& str) {
	try {
		variable.at(str);
		return true;
}
	catch (exception) {

		return false;
	}
}

void Variable_base::define_var(string& str, int d) { 
	if (is_declared(str)) {
		string s = "variable already defined: " + str;
		error_msg(s, variable_decl);
	}
	else
	variable[str] = d; 
}

void Variable_base::set_varible(string& str, int d) {
	
	if (is_declared(str)) {
		variable[str] = d;
	}
	else {
		string s = "undefined variable: " + str;
		error_msg(s, variable_decl);
	}
		
		
}

int Variable_base::get_value(string& name) {
	try {
		return variable.at(name);
	}
	catch (exception) {
		string s = "undefined variable: " + name;
		error_msg(s,variable_decl);
		return -1;
	}
}



//primary grammer expression
int primary(Token_stream& ts, Variable_base& vb) {

	//Token t = ts.get_token();
	Token t(0);
	ts >> t;

	switch (t.kind()) {
	case '(':
	{
		int lval = expression(ts,vb);
		t = ts.get_token();
		if (t.kind() != ')')error_msg("bad closing parathensis");
		return lval;
	}
	case print:
		ts << t;
		return t.value();
	case int_kind:
		return t.value();
	case name_var:
			return vb.get_value(t.name());
	case quit:
		ts.put_back(t);
		t.value();
	case help:
		ts.put_back(t);
		t.value();
	case '~':
		ts.put_back(t);
		return t.value();
	default:
		error_msg("bad primary");
		return -1;
	}

}

//Term1 grammer expression
int Term1(Token_stream& ts, Variable_base& vb) {
	//Token t = ts.get_token();
	Token t(0);
	ts >> t;

	if (t.kind() == '~') 
		return ~primary(ts,vb);
	else
		ts << t;

	return primary(ts,vb);
}

//Term2 grammer expression
 int Term2(Token_stream& ts, Variable_base& vb) {

	 int lval = Term1(ts,vb);
	 Token t = ts.get_token();
	 if (t.kind() == '&') {
		 lval = lval & Term1(ts,vb);
	 }else
	 ts.put_back(t);
	 return lval;
}

//Term3 grammer expression
int Term3(Token_stream& ts, Variable_base& vb) {
	int lval = Term2(ts,vb);
	Token t = ts.get_token();
	if (t.kind() == '^') {
		lval = lval ^ Term2(ts,vb);
	}else
	ts.put_back(t);
	return lval;
}

//expression grammer 
int expression(Token_stream& ts, Variable_base& vb) {
	int lval = Term3(ts,vb);
	Token t = ts.get_token();
	
	if (t.kind() == '|') {
		lval |= Term3(ts,vb);
	}
	else ts.put_back(t);
	return lval;
}

//update grammer
int Update(Token_stream& ts, Variable_base& vb) {

	Token t = ts.get_token();

	if (t.name() != "" && t.kind() == var_exist) {
		int lval = expression(ts,vb);
		vb.set_varible(t.name(), lval);
		t.name() = "";
		return lval;
	}
	else ts.put_back(t);
	return expression(ts,vb);
}

//definition grammer
int definition(Token_stream& ts, Variable_base& vb) {

	//Token t = ts.get_token();
	Token t(0);
	ts >> t;
	int lval = 0;
	if (t.name() != "" && t.kind() == var_def) {

		//check for special name
		stringstream s(t.name());
		char ch = 0;
		s >> ch;
		if (special_name(ch)) {
			s << "Special name: " << ch;
			error_msg(s.str(), variable_decl);
		}

		lval = expression(ts,vb);
		vb.define_var(t.name(), lval);
		t.name() = "";
		return lval;
	}
	else ts<< t;
	return Update(ts, vb);
}

void instruction() {
	cout << "Please enter any integer logical expression you wish to compute\n\n";
	cout << "The only acceptable logical computation are:\n and(&), or(|), compliment(~) and exclusive or(^)\n\n";
	cout << " example '2&3' with enter button to print the result (as '=2') and 'q' to exit the program\n\n";
	cout << "You can also define a value in the form: 'let variable_name = expression' \n\n";
	cout << "You can also assign new value to already existing variable using: 'variable_name = expression' \n\n";
}
void execute_cal() {


	instruction();
	Token_stream ts;
	Variable_base vb; //data base for the variable

	Token t(0);
	int val = 0;

		while (1) {
			try {
				t = ts.get_token();
			
				if (t.kind() == quit)break;
				else if (t.kind() == help) {
					instruction();
					ts.get_token(); // remove end of line
				}
					
				else if (t.kind() == print)
					cout << "= " << val << endl;
				else ts.put_back(t);
				val = definition(ts, vb);
			}
			catch (execution_error& e) {
			
			 if (e.er_no == variable_decl || e.er_no == wrong_name_decl) { // clean all the mess and continue
					char ch = 0;
					cin.unget(); // go a bit backward
					cin.clear(); // restore the state
					while (cin.get(ch) && ch != print); // clear the buffer
					cerr << e.what() << endl;
					val = 0;
					ts = Token_stream(); // reinitialize buffer
				}
				else throw;
			}

		}
	



}


// functions for handling the error
inline void error_msg(const std::string& s) {
	throw execution_error(s);
}

inline void error_msg(const std::string& s1, const std::string& s2) {
	error_msg(s1 + s2);
}

inline void error_msg(const std::string& s1, const int i) {
	throw execution_error(s1, i);
}

void keep_window_open()
{
	std::cin.clear();
	std::cout << "Please enter a character to exit\n";
	char ch;
	std::cin >> ch;
	return;
}

void keep_window_open(const std::string& s)
{
	if (s == "") return;
	std::cin.clear();
	std::cin.ignore(120, '\n');
	for (;;) {
		std::cout << "Please enter " << s << " to exit\n";
		std::string ss;
		while (std::cin >> ss && ss != s)
			std::cout << "Please enter " << s << " to exit\n";
		return;
	}
}


