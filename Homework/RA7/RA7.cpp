#include <iostream>
#include <string>
#include <stack>
#include "infixToPostfix.cpp"

using namespace std;

void infToPostfix(string instr);

int main ( ) 
{
	string instr;
	cout<<"Please enter an infix notation expression using single lowercase characters:" << endl;
	getline(cin, instr);
	infToPostfix(instr);
	
	return 0;	
}