#include "lex.h"
//#include "lex.cpp" ==> vocareum links differently
#include <fstream>
#include <iostream>
#include <string>
#include <map>

using namespace std;
void printS(map<string, LexItem> m);
void printI(map<int, LexItem> m);
void printR(map<double, LexItem> m);
void printID(map<string, LexItem> m);

int main(int argc, char *argv [])
{
    ifstream myFile;
    string filename;
    int files = 0;
    bool v = false, iconst = false, rconst = false, sconst = false, ident = false;

    for(int i = 1; i < argc; i++)
    {
        string a(argv[i]);
        
        if(a[0] == '-') //check for flag
        {
            if(a == "-v")
                v = true;
            else if (a == "-iconst")
            {
                iconst = true;     
            }
            else if(a == "-rconst")
            {
                rconst = true;
            }
            else if(a == "-sconst")
            {
                sconst = true;
            }
            else if(a == "-ident")
            {
                ident = true;
            }
            else
            {
                cout << "UNRECOGNIZED FLAG " << argv[i] << endl;
                return 0;
            }
        }
        else
        {
            files++;
            if(files > 1)
            {    
                cout << "ONLY ONE FILE NAME ALLOWED." << endl;
                return 0;
            }
            else
            {
                filename = a;   
            }
        }
    }
    
    if(files == 0)
    {
        cout << "NO SPECIFIED INPUT FILE NAME." << endl;
        return 0;
    }
    
    myFile.open(filename);
    
    if(!myFile.is_open())
    {
        cout << "CANNOT OPEN the File " << filename << endl;
        return 0;
    }
    
    if(myFile.peek() == ifstream::traits_type::eof())
    {
        cout << "Lines: 0" << endl;
        return 0;
    }
    
    map<int, LexItem> integers;
    map<double, LexItem> reals;
    map<string, LexItem> strings;
    map<string, LexItem> identifiers;
    
    
    LexItem t; Token type;
    int linenum = 1;
    //t.GetLinenum()
    t = getNextToken(myFile, linenum);
    type = t.GetToken();

    int numTokens = 0;

    while(type != DONE && type != ERR)
    {
        numTokens++;
        
        string lex = t.GetLexeme();

        if(v)
        {
            cout << t << endl;
        }
        
        if(iconst)
        {
            if(type == ICONST)
                integers.insert(pair<int, LexItem>(stoi(lex),t));
        }

        if(rconst)
        {
            if(type == RCONST)
                reals.insert(pair<double, LexItem>(stod(lex),t));
        }

        if(sconst)
        {
            if(type == SCONST)
                strings.insert(pair<string, LexItem>(lex,t));
        }

        if(ident)
        {
            if(type == IDENT)
                identifiers.insert(pair<string, LexItem>(lex,t));
        }
        
        linenum = t.GetLinenum();
        t = getNextToken(myFile, linenum);   
        type = t.GetToken();
    }

    if(type == ERR)
    {
        if(t.GetLexeme() == "*)")
        {
            cout << endl << "Missing a comment end delimiters '*)' at line " << linenum << endl;
            type = DONE;
        }
        else
            cout << "Error in line " << linenum << " (" << t.GetLexeme() << ")" << endl;   
    }
    
    if(type == DONE)
    {
        cout << "Lines: " << linenum << endl;
        cout << "Tokens: " << numTokens << endl;
        
        if(sconst)
        {
            cout << "STRINGS:" << endl;
            printS(strings);
        }
        
        if(iconst)
        {
            cout << "INTEGERS:" << endl;
            printI(integers);
        }
        
        if(rconst)
        {
            cout << "REALS:" << endl;
            printR(reals);
        }
        
        if(ident)
        {
            cout << "IDENTIFIERS:" << endl;
            printID(identifiers);
        }
            
    }
    
    return 0;
    
}

void printS(map<string, LexItem> m)
{
    map<string, LexItem>::iterator it;
    
    for(it = m.begin(); it != m.end(); it++)
    {
        cout << it->first << endl;
    }       
}

void printID(map<string, LexItem> m)
{
    map<string, LexItem>::iterator it;
    
    for(it = m.begin(); it != m.end(); it++)
    {
        if(it != m.begin()) 
            cout << ", ";
        cout << it->first;
    }    
    
    cout << endl;
}

void printI(map<int, LexItem> m)
{
    map<int, LexItem>::iterator it;
    
    for(it = m.begin(); it != m.end(); it++)
    {
        cout << it->first << endl;
    }       
}

void printR(map<double, LexItem> m)
{
    map<double, LexItem>::iterator it;
    
    for(it = m.begin(); it != m.end(); it++)
    {
        cout << it->first << endl;
    }       
}