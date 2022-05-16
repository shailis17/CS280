//driver program

#include <iostream>
#include <string>
#include "PrintRevNums.cpp"
using namespace std;

void PrintRevNums(int sum); //implement in PrintRevNums.cpp 

int main()
{
	int sum = 0;
	
	cout << "Enter any sequence of positive integer numbers, then enter 0 to end: ";
	PrintRevNums(sum);
	return 0;
}
