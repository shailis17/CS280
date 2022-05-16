//Counting Lines

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

int main()
{
    string name;
    cout << "Enter the name of a file to read from: ";
    cin >> name;

    ifstream myInfile;
    myInfile.open(name.c_str());

    cout << endl;
    
    //check if file doesn't exist
    if(!myInfile.is_open())
    {
        cout << endl << "File cannot be opened " << name << endl;
        return 0;
    }

    //declare variables
    int totalCount = 0, comment = 0, command = 0, b = 0, i = 0, r = 0;
    string bold = "bold", italic = "italic", reg = "regular";
    
    //traverse file line by line
    string line;
    while(getline(myInfile, line))
    {
        totalCount++;
        if(line[0] == '#')
            comment++;
        if(line[0] == '%')
        {
            istringstream is(line.substr(1));
            string c;
            is >> c;

            if(c == bold)
                b++;
            else if(c == italic)
                i++;
            else if(c == reg)
                r++;
            else
            {
                cout << endl << "Error: Unrecognizable command in line " << totalCount << endl;
                command++;
            }
        }
    }

    //print results
    cout << endl << "Total lines: " << totalCount << endl; 
    cout << "Commented lines: " << comment << endl; 
    cout << "Command lines: " << (command + b + i + r) << endl; 
    cout << "Bold commands: " << b << endl; 
    cout << "Italic commands: " << i << endl; 
    cout << "Regular commands: " << r << endl; 

    return 0;
}