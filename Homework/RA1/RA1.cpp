/*
 * Week 1: Welcome 
 */

#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
    string first, last;

    cout << "Welcome to CS 280." << endl;
    cout << "What is your first and last names?" << endl;
    cin >> first;
    cin >> last;
    cout << "Hello, " << first << " " << last << endl;

    return 0;
}

