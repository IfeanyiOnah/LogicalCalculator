//--------------assumption on the user analysis of the program-----------------
//It is assume that the user want a program that do a logical computation of any giving expression
//It is also assume that the user wishes to enter base ten numbers instead of binary numbers
//The only acceptable logical computation are and(&), or(|), not(!) and exclusive or(^)
//the order of priority for compuation are:
//1. Paranthesis()
//2. not(!)
//3. and(&)
//4. exclusive or(^)
//5. or(|)


//.......user input choice of input/output method.....
//1. The input and output has to be read and write from/to a console 
//2. The user enter the expression with a termination of semicolon(;) followed by enter button to show end of expression
//3. The user enter letter 'q' to quit the program
//4. 

//-------------design of the program based on the user analysis on the programmer side------------------

		//----------sudo code--------------
				//while 'q' is not enter
				//read input from console
				//execute the program
				// write the output to the console

//---------------------analysis on the program execution------------------
		//--------------grammer for logical expression-----------
						//Number: 
									// integer literals in base 10
						//Primary: 
									//Number
									// '(' expression ')'
						//Term1:
									//Primary
									//!Term1
						//Term2:
									//Term1
									//Term2 & Term1
						//Term3:
									//Term2
									//Term3 ^ Term2
						//Expression:
									//Term3
									//Expression | Term3


//-------------implementation of the program based on the design------------------
//In order to implement a program that follow the rule of this grammer 
//the input expression has to be tokenize inform of (kind,value) pair token
 

#include "LogicalCalculator.h"
#define QUIT 'q'
#define RES ';'

using namespace std;

const char int_kind = '8';


//class for the token
class Token {
public:

	//modifiable member function
	//construct to make a token from a character(kind) 
	Token(char ch):m_kind(ch), m_value(0){}

	//construct to make a token from a character and double(value)
	Token(char ch, int val):m_kind(ch), m_value(val){}


	//non-modifiable member function
	//function to return kind
	char kind() { return m_kind; }
	char kind()const { return m_kind; }

	//function to return val
	int& value() { return m_value; }
	const int& value()const { return m_value; }

private:
	char m_kind;
	int m_value;
};

//token stream to hold/buffer the token

class Token_stream {
public:
	//Modifiable member function

	//construct a default token stream
	Token_stream():m_buffer(0), m_full(false){}

	//function to put back token
	void put_back(Token& tok) {
		m_buffer = tok;
		m_full = true;
	}

	//get token function: it takes an empty token and return a full token
	const Token& get_token();

private:
	Token m_buffer;
	bool m_full;

};

const Token& Token_stream::get_token() {

	if (m_full) {
		m_full = false;
		return m_buffer;
	
	}

	//read a character
	char ch;
	cin >> ch;

	switch (ch) {
		//check for any term expect value
	case '|': case '&': case '^': case RES: case QUIT: case '~': case '(': case ')':
	{
		return Token(ch);

	}
	case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
	{
		cin.unget();
		int val = 0;
		cin >> val;
		return Token(int_kind, val);
	}

	default:
		error_msg("bad input");
		break;
	}
}

Token_stream ts;

int expression();

//primary grammer expression
int primary() {

	Token t = ts.get_token();
	int lval = t.value();
	switch (t.kind()) {
	case '(':
	{
		lval = expression();
		t = ts.get_token();
		if (t.kind() != ')')error_msg("bad closing parathensis");
		return lval;
	}
	case int_kind:
		return lval;
	case '~':
		ts.put_back(t);
		return lval;
	default:
		error_msg("bad primary");
	}

}

//Term1 grammer expression
int Term1() {
	int lval = primary();
	Token t = ts.get_token();
	
	if (t.kind() == '~') {
		lval = primary();
		return ~lval;
	}
	else if (t.kind() != int_kind) {
		ts.put_back(t);
	}
	return lval;
}

//Term2 grammer expression
 int Term2() {

	 int lval = Term1();
	 Token t = ts.get_token();
	 if (t.kind() == '&') {
		 lval = lval & Term1();
	 }else
	 ts.put_back(t);
	 return lval;
}

//Term3 grammer expression
int Term3() {
	int lval = Term2();
	Token t = ts.get_token();
	if (t.kind() == '^') {
		lval = lval ^ Term2();
	}else
	ts.put_back(t);
	return lval;
}

//Term3 grammer expression
int expression() {
	int lval = Term3();
	Token t = ts.get_token();
	
	if (t.kind() == '|') {
		lval |= Term3();
	}
	else ts.put_back(t);
	return lval;
}

void execute_cal() {

	cout << "Please enter any integer logical expression you wish to compute" << endl;
	cout << "The only acceptable logical computation are: and(&), or(|), compliment(~) and exclusive or(^)\n";
	cout << " example '2&3;' with a semicolon(;) to print the result\n";


	Token t(0);
	int val = 0;
	while (1) {
		t = ts.get_token();
		
		if (t.kind() == QUIT)break;
		if (t.kind() == RES)
			cout << val << endl;
		else ts.put_back(t);
		val = expression();
	
	}

}

int main() {
	try {
		execute_cal();
	}

	catch (execution_error& e) {

		cerr << e.what() << endl;
	}
	cout << "enter '~' character to exit" << endl;
	while (getchar() !='~');
	return 0;
}
