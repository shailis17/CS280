#include "lex.h"
#include <map>
#include <string>
#include <iostream>

using namespace std;

LexItem id_or_kw(const string& lexeme, int linenum)
{
    map<string, Token> keywords = 
    {
        {"PROGRAM", PROGRAM}, {"WRITELN", WRITELN}, {"INTEGER", INTEGER}, {"BEGIN", BEGIN},
        {"END", END}, {"IF", IF}, {"REAL", REAL}, {"STRING", STRING}, {"VAR", VAR},
        {"ELSE", ELSE}, {"FOR", FOR}, {"THEN", THEN}, {"DO", DO}, {"TO", TO}, {"DOWNTO", DOWNTO}
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

    out << t << " (" << tok.GetLexeme() << ") " << "Line #: " << tok.GetLinenum() << endl;
    return out;
}