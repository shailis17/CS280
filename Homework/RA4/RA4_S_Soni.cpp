#include <iostream>
#include <fstream>       
#include <string>
#include <cctype>
#include <sstream>
#include <map>

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
        cout << "No file is found." << endl;
        return 0;
    }

    map<char, int> punct;
    int total = 0;

    string line;
    while(getline(myFile, line))
    {
        for(char c : line)
        {
            if(!isspace(c))
                total++;
            
            if(ispunct(c))
            {
                if(punct.find(c) == punct.end())
                    punct.insert(pair<char, int>(c,1));
                else
                    punct[c]++;
            }
        }
    }

    if(total == 0)
    {
        cout << "File is empty." << endl;
        return 0;
    }

    cout << "Number of characters in the file: " << total << endl;
    cout << "Punctuation characters in the file and their occurrences:" << endl << endl;

    map<char, int>::iterator it;
    char maxChar = punct.begin()->first;
    int maxCount = punct.begin()->second;
    for(it = punct.begin(); it != punct.end(); it++)
    {
        cout << it->first << ": " << it->second << endl;
        if(it->second > maxCount)
        {
            maxChar = it->first;
            maxCount = it->second;
        }
    }    

    cout << endl;
    cout << "The punctuation character with maximum occurrences is \"" << maxChar << "\" found " << maxCount << " times." << endl;

    return 0;
}