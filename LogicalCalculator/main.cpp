//--------------assumption on the user analysis of the program-----------------
//It is assume that the user want a program that do a logical computation of any giving expression
//It is also assume that the user wishes to enter base ten numbers instead of binary numbers
//It is also assume that the user wishes to declare or defined a variabe to be used later in the program
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
//3. The user enter "let Name = Expression" to save the expression for later use
//4. The user enter "Name = Expression" to save the Expression in already defined Name
//4. The user enter letter 'q' to quit the program



#include "exception_header.h"
using namespace std;


int main() {
	try {
		execute_cal();
	}

	catch (execution_error& e) {

		cerr << e.what() << endl;
	}

	catch (exception& e) {

		cerr << e.what() << endl;
	}
	keep_window_open();
	return 0;
}
