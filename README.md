# LogicalCalculator
 This program perform a simple logical calculations on integer numbers by reading from cin and writing to cout

The only acceptable logical computation are and(&), or(|), complement(~) and exclusive or(^)


The order of priority for compuation are:
1. complement(~)
2. and(&)
3. exclusive or(^)
4. or(|)

The grammer for the input expression is given as follows:

Statement:
	Definition
	Assignment
	Expressions
	Print
	Quit

Print:
	;
Quit:
	q


Definition:
	"let" Name "=" Expression
	Update

Name: 
	string of characters (Must start with letter, only digits and letters are allowed)

Update:
	Name "=" Expression
	Expression

Expression:
	Term3
	Expression | Term3
Term3:
	Term2
	Term3 ^ Term2

Term2:
	Term1
	Term2 & Term1

Term1:
	~Primary
	Primary

Primary:
	Number
	Variable_Name
	'(' Definition:')'
Number: 
	Integer literal
Variable_Name: 
		Name of already defined variable

The input come from cin through the Token stream
