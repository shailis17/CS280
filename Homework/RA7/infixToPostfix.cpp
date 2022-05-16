#include <iostream>
#include <string>
#include <stack>
#include <cctype>

using namespace std;

void infToPostfix(string instr)
{
    string postfix = "";
    stack<char> opsStack;
    bool paren = false;

    for(int i = 0; i < instr.length(); i++)
    {
        char c = instr[i];
        
        if(isspace(c))
            continue;
        
        if(isalpha(c))
        {
            postfix += c;
            postfix += ' ';
            continue;
        }
             
        if(c == ')')
        {
            while(opsStack.top() != '(')
            {
                postfix += opsStack.top();
                postfix += ' ';
                opsStack.pop();
            }
            opsStack.pop();
            paren = false;
        }
        else
        {
            if(c == '(')
                paren = true;
            
            if(!paren && !opsStack.empty())
            {
                if(c == '/' || c == '*')
                    if(opsStack.top() != '+' && opsStack.top() != '-')
                    {   
                        postfix += opsStack.top();
                        postfix += ' ';
                        opsStack.pop();
                    }
                if(c == '+' || c == '-')
                    if(opsStack.top() != '(')
                    {
                        postfix += opsStack.top();
                        postfix += ' ';
                        opsStack.pop();
                    }        
            }
            opsStack.push(c);   
        }
    }
    
    while(!opsStack.empty())
    {
        postfix += opsStack.top();
        postfix += ' ';
        opsStack.pop();
    }

    cout << postfix << endl;
}