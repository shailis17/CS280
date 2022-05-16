#include <iostream>
#include <fstream>       
#include <string>
#include <cctype>
#include <sstream>

using namespace std; 

int main(int argc, char *argv [])
{
    ifstream myFile;

    //check for file name
    if(argc >= 2 ) 
    {
        myFile.open(argv[1]);
        if(!myFile.is_open() )
        { /* error */ 
            cout << "File cannot be opened: " << argv[1] << endl; 
            return 0;
        }
    }
    else
    {
        cout << "No file name is found" << endl;
        return 0;
    }

    //declare variables
    int total = 0, nonBlank = 0, words = 0, integers = 0, names = 0;

    //traverse file line by line
    string line;
    while(getline(myFile, line))
    {
        total++;
        if(line.length() > 0)
            nonBlank++;
        
        string word;
        stringstream s(line);

        while(s >> word)
        {
            words++;

            if(isdigit(word[0]))
            {
                bool isInt = true;

                for(char c : word)
                {
                    if(!isdigit(c))
                    {
                        isInt = false;
                        break;
                    }
                }

                if(isInt)
                    integers++;
            }
            else
            {
                if(isalpha(word[0]))
                {
                    char last = word[word.length() - 1];
                    if(isdigit(last) || isalpha(last) || last == '_' || last == '@' || last == '.')
                        names++;
                }
                    
            }
        }
    }
    
    if(total == 0)
    {
        cout << "File is empty." << endl;
        return 0;
    }

    //print results
    cout << "Total Number of Lines: " << total << endl;
    cout << "Number of non-blank lines: " << nonBlank << endl;
    cout << "Number of Words: " << words << endl;
    cout << "Number of Integers: " << integers << endl;
    cout << "Number of Names: " << names << endl;

    return 0;
}