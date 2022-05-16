#include "lex.h"
#include <map>
#include <string>
#include <cctype>

LexItem getNextToken(istream& in, int& linenum)  //check for \n and increment linenum
{ 
    enum TokState {START, INID, INSTRING, ININT, INREAL, INCOMMENT}
    lexstate = START;
    string lexeme = "";
    char c;

    while(in.get(c)) 
    {
        /*if(c == -1)
            return LexItem(DONE, "DONE", linenum);*/
        if(c == '\n')
        {
            if(in.peek() == -1)
                return LexItem(DONE, "DONE", linenum);
              
            linenum++;
            continue;
        }
        
        while(isspace(c))
        {
            if(c == '\n' && in.peek() != -1)
                linenum++;
            if(in.peek() == -1)
                return LexItem(DONE, "DONE", linenum);
            in.get(c);
        }
        
        if(lexstate == START) 
        {
            if(c == '\n')
            {
                linenum++;
            }

            if(isalpha(c))
                lexeme += toupper(c);
            else
                lexeme += c;
            
            if(isalpha(c) || c == '_')
            {
                lexstate = INID;
            }
            else if(c == '\'')
            {
                lexstate = INSTRING;
            }
            else if(isdigit(c))
            {
                lexstate = ININT;
            }
            else if(c == '.')
            {
                lexstate = INREAL;   
            }
            else if(c == '(')
            {
                in.get(c);

                if(c == '*')
                {
                    lexeme += c;
                    lexstate = INCOMMENT;
                }
                else
                {
                    in.putback(c);
                    return LexItem(LPAREN, lexeme, linenum);
                }
            }
            else
            {
                if(c == '+')
                    return LexItem(PLUS, "+", linenum);
                else if(c == '-')
                    return LexItem(MINUS, "-", linenum);
                else if(c == '*')
                    return LexItem(MULT, "*", linenum);
                else if(c == '/')
                    return LexItem(DIV, "/", linenum);
                else if(c == ':')
                {
                    char ch;
                    in.get(ch);
                    
                    if(ch == '=')
                        return LexItem(ASSOP, ":=", linenum);
                    else
                    {
                        in.putback(ch);
                        return LexItem(COLON, ":", linenum);
                    }
                }
                else if(c == '(')
                    return LexItem(LPAREN, "(", linenum);
                else if(c == ')')
                    return LexItem(RPAREN, ")", linenum);
                else if(c == '=')
                    return LexItem(EQUAL, "=", linenum);
                else if(c == '<')
                    return LexItem(LTHAN, "<", linenum);
                else if(c == '>')
                    return LexItem(GTHAN, ">", linenum);
                else if(c == ';')
                    return LexItem(SEMICOL, ";", linenum);
                else if(c == ',')
                    return LexItem(COMMA, ",", linenum);
                else
                    return LexItem(ERR, lexeme, linenum);
            }
        }
        
        if(lexstate == INID) //letter followed by 0+ letters/digit
        {
            in.get(c);

            while(isalnum(c) || c == '_')
            {
                if(isalpha(c))
                    lexeme += toupper(c);
                else
                    lexeme += c;
                in.get(c);
            }

            in.putback(c);

            return id_or_kw(lexeme, linenum);
        }
        
        if(lexstate == INSTRING)
        {
            in.get(c);

            while(c != '\n' && c != '\'' && c != '\"' && c != EOF)
            {
                lexeme += c;
                in.get(c);
            }
            
            if(c == '\n')
            {
                in.putback(c);
                return LexItem(ERR, lexeme, linenum);
            }
            else if(c == '\"')
            {
                return LexItem(ERR, lexeme + c, linenum);   
            }
            else if(c == '\'')
            {
                return LexItem(SCONST, lexeme + "\'", linenum);
            }
            else
            {
                return LexItem(ERR, lexeme , linenum);
            }
        }
        
        if(lexstate == ININT) //ICONST = 1 or more digits
        {
            in.get(c);

            while(isdigit(c)) //pull in digits
            {
                lexeme += c;
                in.get(c);
            }

            if(c == '.') //if you see . ==> RCONSTS
            {
                lexeme += c;
                lexstate = INREAL;
            }
            else
            {
                in.putback(c);
                    
                return LexItem(ICONST, lexeme, linenum);
            }
        }

        if(lexstate == INREAL)
        {
            in.get(c);

            while(isdigit(c)) //pull in digits
            {
                lexeme += c;
                in.get(c);
            }

            if(c == '.') //if you see . ==> ERR
            {
                lexeme += c;

                while(isdigit(c)) //pull in digits
                {
                    lexeme += c;
                    in.get(c);
                }

                in.putback(c);

                return LexItem(ERR, lexeme, linenum);
            }
            else
                in.putback(c);

            return LexItem(RCONST, lexeme, linenum);
        }

        if(lexstate == INCOMMENT)
        {
            in.get(c);

            while(c != '*' && in.peek() != -1)
            {
                lexeme += c;
                if(c == '\n')
                    linenum++;

                in.get(c);
            }
            
            if(in.peek() == -1)
                return LexItem(ERR, "*)", linenum);
            else
            {
                lexeme += c;
                in.get(c);

                if(c == ')')
                {
                    lexeme = "";
                    lexstate = START;
                    continue;
                }
                else
                    in.putback(c);
            }
        }
    }
    
    return LexItem(DONE, "DONE", linenum);
}

LexItem id_or_kw(const string& lexeme, int linenum) 
{
    map<string, Token> keywords = 
    {
        {"PROGRAM", PROGRAM}, {"WRITELN", WRITELN}, {"INTEGER", INTEGER}, {"BEGIN", BEGIN},
        {"END", END}, {"IF", IF}, {"REAL", REAL}, {"STRING", STRING}, {"VAR", VAR},
        {"ELSE", ELSE}, {"FOR", FOR}, {"THEN", THEN}, {"DO", DO}, 
        {"TO", TO}, {"DOWNTO", DOWNTO}
    };

    map<string, Token>::iterator it = keywords.find(lexeme); 
    
    if(it != keywords.end())
    {
        return LexItem(it->second, lexeme, linenum);
    }
    else
    {
        return LexItem(IDENT, lexeme, linenum);
    }
}

ostream& operator<<(ostream& out, const LexItem& tok)
{
    Token type = tok.GetToken();
    
    map<Token, string> tokens = {
        {PROGRAM, "PROGRAM"}, {WRITELN, "WRITELN"}, {INTEGER, "INTEGER"}, 
        {BEGIN, "BEGIN"}, {END, "END"}, {IF, "IF"}, {REAL, "REAL"}, {STRING, "STRING"},
        {VAR, "VAR"}, {ELSE, "ELSE"}, {FOR, "FOR"}, {THEN, "THEN"}, {DO, "DO"}, 
        {TO, "TO"}, {DOWNTO, "DOWNTO"}, {IDENT, "IDENT"}, {ICONST, "ICONST"},
        {RCONST, "RCONST"}, {SCONST, "SCONST"}, {PLUS, "PLUS"}, {MINUS, "MINUS"}, 
        {MULT, "MULT"}, {DIV, "DIV"}, {ASSOP, "ASSOP"}, {LPAREN, "LPAREN"}, {RPAREN, "RPAREN"}, 
        {COMMA, "COMMA"}, {EQUAL, "EQUAL"}, {GTHAN, "GTHAN"}, {LTHAN, "LTHAN"}, 
        {SEMICOL, "SEMICOL"}, {COLON, "COLON"}, {ERR, "ERR"}, {DONE, "DONE"}
    };

    map<Token, string>::iterator it = tokens.find(type); 
    string t = it->second;

    out << t;

    if(type == IDENT || type == ICONST || type == RCONST || type == SCONST)
    {
        string lexeme = tok.GetLexeme();
        
        if(type == SCONST)
            lexeme = lexeme.substr(1, lexeme.length() - 2);
        
        out << "(" << lexeme << ")";
    }
    
    return out;
}