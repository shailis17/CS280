/* Implementation of PLSL Interpreter
 * parseInt.cpp
 * Programming Assignment 3
 * Spring 2022
*/

#include <vector>
#include <string>
#include "parseInt.h"
#include "val.h"
//#include "lex.cpp"
using namespace std;
map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value> * ValQue; //declare a pointer variable to a queue of Value objects

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

bool IdentList(istream& in, int& line, vector<string> &IdList);

//Prog ::= PROGRAM IDENT; DeclBlock ProgBody 
bool Prog(istream& in, int& line){
    LexItem t = Parser::GetNextToken(in, line);
	if(t.GetToken() != PROGRAM){
		ParseError(line, "Missing PROGRAM.");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t.GetToken() != IDENT){
		ParseError(line, "Missing Program Name.");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t.GetToken() != SEMICOL){
		ParseError(line, "Missing semicolon after program name");
		return false;
	}
	
	bool status = DeclBlock(in, line);
	if(!status){
		ParseError(line, "Incorrect Declaration Section.");
        //cout << t << " " << t.GetLexeme() << endl;
		return false;
	}

	status = ProgBody(in, line);
	if(!status){
		ParseError(line, "Syntactic error in Program Body.");
        //cout << t << " " << t.GetLinenum() << endl;
		return false;
	}

	return true;
}

//DeclBlock ::= VAR {DeclStmt;} 
bool DeclBlock(istream& in, int& line){
	LexItem t = Parser::GetNextToken(in, line);
	if(t.GetToken() != VAR){
		ParseError(line, "Non-recognizable Declaration Block.");
		return false;
	}
	bool status = DeclStmt(in, line);
	
	if(!status){
		return true;
	}
	else{
		while(status){
            //cout << t << " " << t.GetLinenum() << endl;
            t = Parser::GetNextToken(in, line);
            //cout << t << " " << t.GetLinenum() << endl;
            if(t.GetToken() != SEMICOL){
                ParseError(line, "Missing semicolon after declstmt");
                //cout << t << " " << t.GetLinenum() << endl;
                return false;
            }
            else{
                t = Parser::GetNextToken(in, line);
                if(t.GetToken() == BEGIN)
                {
                    Parser::PushBackToken(t);
                    return true;
                }
                Parser::PushBackToken(t);
            }
            status = DeclStmt(in, line);
            //t = Parser::GetNextToken(in, line);
		}
	}
	//Parser::PushBackToken(t);
	return true;
}
//DeclStmt ::= Ident {, Ident} : (Integer | Real | String) 
bool DeclStmt(istream& in, int& line){
	vector<string> ids;
    bool status = IdentList(in, line, ids);
	
	if (!status)
	{
		ParseError(line, "Incorrect variable in Declaration Statement.");
		return status;
	}
    
    LexItem tok = Parser::GetNextToken(in, line);    
    
	if(tok.GetToken() == COLON)
	{
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == INTEGER || tok.GetToken() == REAL || tok.GetToken() == STRING)
		{
            //cout << tok << " " << tok.GetLinenum() << endl;
            for(string s : ids)
                SymTable[s] = tok.GetToken();
			return true;
		}
		else
		{
			ParseError(line, "Incorrect Declaration Type.");
			return false;
		}
	}
	else
	{
		Parser::PushBackToken(tok);
		return false;
	}
}

bool IdentList(istream& in, int& line, vector<string> &IdList) {
	bool status = false;
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok == IDENT)
	{
		//set IDENT lexeme to the type tok value
		identstr = tok.GetLexeme();
		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
            IdList.push_back(tok.GetLexeme());
		}	
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
		
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	
	tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		status = IdentList(in, line, IdList);
	}
	else if(tok == COLON)
	{
		Parser::PushBackToken(tok);
		return true;
	}
	else {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return status;
	
}

//ProgBody ::= BEGIN {Stmt;} END 
bool ProgBody(istream& in, int& line){
	LexItem t = Parser::GetNextToken(in, line);
	if(t.GetToken() != BEGIN){
		ParseError(line, "Missing BEGIN token");
        //cout << t << " " << t.GetLinenum() << endl;
		return false;
	}
	
    //cout << "found begin" << endl;
	bool status = Stmt(in, line);
    
	while(status){
        //cout << "found statement" << endl;
		t = Parser::GetNextToken(in, line);
        //cout << t << " " << t.GetLinenum() << endl;
		if(t.GetToken() != SEMICOL){
            //Parser::PushBackToken(t);
            cout << t << " " << t.GetLinenum() << endl;
			ParseError(line - 1, "Missing semicolon in Statement.");
            //cout << t << " " << t.GetLinenum() << endl;
			return false;
		}
		status = Stmt(in, line);
	}

    t = Parser::GetNextToken(in, line);
	if(t.GetToken() != END){
		ParseError(line, "Missing END token");
        //cout << t << " " << t.GetLinenum() << endl;
		return false;
	}

	return true;
}

//WriteStmt:= wi, ExpreList 
bool WriteLnStmt(istream& in, int& line) {
	LexItem t;
	//cout << "in WriteStmt" << endl;
	ValQue = new queue<Value>;
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression after WriteLn");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	
	//Evaluate: print out the list of expressions' values
	while (!(*ValQue).empty())
	{
		Value nextVal = (*ValQue).front();
		cout << nextVal;
		ValQue->pop();
	}
	cout << endl;

	return ex;
}//End of WriteLnStmt

bool ExprList(istream& in, int& line) {
	bool status = false;
	Value retVal;
	
	status = Expr(in, line, retVal);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
    //add retval to valqueue
    //cout << retVal << endl;
	ValQue->push(retVal);
    
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok.GetToken() == COMMA) {
		status = ExprList(in, line);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		//cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

//Stmt ::= AssigStmt | IfStmt | WriteLnStmt  
bool Stmt(istream& in, int& line) {
	bool status;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case WRITELN:
           // cout << "enter writeln" << endl;
		status = WriteLnStmt(in, line);
		break;
	case IF:
        //    cout << "enter ifstmt" << endl;
		status = IfStmt(in, line);
		break;
	case IDENT:
		Parser::PushBackToken(t);
          //  cout << "enter assignStmt" << endl;
        status = AssignStmt(in, line);
		break;		
	default:
		Parser::PushBackToken(t);
		return false;
	}

	return status;
}//End of Stmt

//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) {
	
	bool varstatus = false, status = false;
	LexItem t;
	Value retVal, v; 
	LexItem idtok;
	varstatus = Var(in, line, idtok);
	
	if (varstatus){
        //cout << "identifier " << idtok << endl;
		t = Parser::GetNextToken(in, line);
		
		if (t.GetToken() == ASSOP){
           // cout << "enter expr" << endl;
			status = Expr(in, line, retVal);
			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statment");
				return status;
			}
           // cout << "value = " << retVal << endl;
			
		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}

	//insert idtok.GetLexeme(), retVal into TempsResults
    //cout << "finished assignment " << idtok.GetLexeme() << " " << retVal << endl;
    
   if(retVal.GetType() == VINT){
       if(SymTable[idtok.GetLexeme()] == REAL)
       {
           float x = (float)retVal.GetInt();
           Value v(x);
           retVal = v;
           //cout << "retval " << retVal << " float " << Value(x) << endl;
       }
   }
    
   if(retVal.GetType() == VREAL){
       //cout << "real " << retVal;
       if(SymTable[idtok.GetLexeme()] == INTEGER)
       {
           int i = (int)retVal.GetReal();
           Value v(i);
           retVal = v;
           //cout << "retval " << retVal << " float " << Value(i) << endl;
       }
       //cout << endl;
   }
    
   if(retVal.GetType() == VSTRING){
       if(SymTable[idtok.GetLexeme()] != STRING)
       {
           ParseError(line, "Illegal Assignment Operator");
			return false;
       }
       //cout << endl;
   }
    
    //cout << "finished assignment " << idtok.GetLexeme() << " " << retVal << endl;
    TempsResults[idtok.GetLexeme()] = retVal;
	//TempsResults.insert({idtok.GetLexeme(), retVal});
	return status;	
}

//Var ::= IDENT 
bool Var(istream& in, int& line, LexItem & idtok)
{
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok.GetToken() == IDENT){
		identstr = tok.GetLexeme();
		
		if (!(defVar.find(identstr)->second))
		{
			ParseError(line, "Undeclared Variable");
			return false;
		}	

		idtok = tok;
		return true;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}

//Expr:= Term {(+|-) Term}
bool Expr(istream& in, int& line, Value & retVal) {
	Value val1, val2;
	//cout << "in Expr" << endl;
	bool t1 = Term(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	retVal = val1;
    //cout << "val 1 = " << retVal << endl;
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	//Evaluate: evaluate the expression for addition or subtraction
	while (tok.GetToken() == PLUS || tok.GetToken() == MINUS ) 
	{
		t1 = Term(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
       // cout << "val 2 = " << val2 << endl;
		
		if(tok.GetToken() == PLUS)
		{
			retVal = retVal + val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal addition operation.");
				//cout << "(" << tok.GetLexeme() << ")" << endl;		
				return false;
			}
		}
		else if(tok.GetToken() == MINUS)
		{
			retVal = retVal - val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal subtraction operation.");
				return false;
			}
		}
			
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
		
	}
	Parser::PushBackToken(tok);
   // cout << retVal << endl;
	return true;
}//end of Expr

//Term:= SFactor {(*|/) SFactor}
bool Term(istream& in, int& line, Value & retVal){
	Value val1, val2;
	//cout << "in Term" << endl;
	bool t1 = SFactor(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	retVal = val1;
    //cout << "val 1 = " << retVal << endl;
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	//Evaluate: evaluate the expression for multiplication or division
	while (tok.GetToken() == MULT || tok.GetToken() == DIV ) 
	{
		t1 = SFactor(in, line, val2);
		if(!t1) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
       // cout << "val 2 = " << val2 << endl;
		
		if(tok.GetToken() == MULT)
		{
            //cout << retVal << " " << val2 << endl;
			retVal = retVal * val2;
            //cout << retVal << endl;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal multiplication operation.");
				//cout << "(" << tok.GetLexeme() << ")" << endl;		
				return false;
			}
		}
		else if(tok.GetToken() == DIV)
		{
            if((val2.IsInt() && val2.GetInt() == 0) || (val2.IsReal() && val2.GetReal() == 0.00))
            {
                ParseError(line, "Run-Time Error-Illegal Division by Zero");
                return false;
            }
			retVal = retVal / val2;
			if(retVal.IsErr())
			{
				ParseError(line, "Illegal division operation.");
				return false;
			}
		}
			
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
		
	}
    //cout << retVal << endl;
	Parser::PushBackToken(tok);
	return true;
}

//SFactor ::= [(+ | -)] Factor  
bool SFactor(istream& in, int& line, Value& retVal){
	LexItem t = Parser::GetNextToken(in, line);
	//cout << "in SFactor" << endl;
    bool status;
	int sign = 0;
	if(t.GetToken() == MINUS )
	{
		sign = -1;
	}
	else if(t.GetToken() == PLUS)
	{
		sign = 1;
	}
	else
		Parser::PushBackToken(t);
		
	status = Factor(in, line, sign, retVal);
    //cout << "SFactor val = " << retVal << endl;
    
    if(sign != 0 && retVal.IsString())
    {
        ParseError(line, "Illegal Operand Type for Sign Operator");
	    return false;	
    }
    
	return status;
}

//Factor ::= IDENT | ICONST | RCONST | SCONST | (Expr) 
bool Factor(istream& in, int& line, int sign, Value & retVal){
	LexItem tok = Parser::GetNextToken(in, line);
	//cout << "in Factor" << endl;
    
	if(tok.GetToken() == IDENT ) {
		string lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(line, "Undefined Variable");
			return false;	
		}

		//Value v = new Value(TempsResults.find(lexeme)->second);
        //cout << lexeme << " " << TempsResults[lexeme] << endl;
        Value v = TempsResults[lexeme];
        if(v.IsErr())
        {
            ParseError(line, "Undefined Variable");
            return false;
        }
        
        if(v.IsInt() || v.IsReal())
        {
            if(sign == -1)
            {
                retVal = v * -1;
                //cout << retVal << endl;
                return true;
            }
            else
                retVal = v;
        }

		retVal = v;
		return true;
	}
	else if(tok.GetToken() == ICONST){
		Value v(stoi(tok.GetLexeme()));
        if(sign == -1)
            retVal = v * -1;
        else
            retVal = v;
        //cout << "integer = " << retVal << endl;
		return true;
	}
    else if(tok.GetToken() == RCONST){
        Value v(stof(tok.GetLexeme()));
		retVal = v;
        if(sign == -1)
            retVal = retVal * -1;
        //cout << "real = " << retVal << endl;
		return true;
    }
    else if(tok.GetToken() == SCONST) {
        Value v(tok.GetLexeme());
		retVal = v;
        //cout << "string = " << retVal << endl;
		return true;
    }
	else if(tok.GetToken() == LPAREN ) {
		bool ex = Expr(in, line, retVal);
		if( !ex ) {
			ParseError(line, "Missing expression after (");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN )
			return ex;
		else 
		{
			//Parser::PushBackToken(tok);
			ParseError(line, "Missing ) after expression");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	return false;
}

//IfStmt ::= IF ( LogicExpr ) THEN Stmt [ELSE Stmt]
bool IfStmt(istream& in, int& line) {
	bool ex=false, status ; 
	LexItem t;
    //cout << "in IfStmt" << endl;
	
	t = Parser::GetNextToken(in, line);
	if(t.GetToken() != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	Value v;
	ex = LogicExpr(in, line, v);
	if( !ex ) {
       // cout << "Value of logic: " << v;
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t.GetToken() != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	/*t = Parser::GetNextToken(in, line);
	if(t.GetToken() != THEN)
	{
		ParseError(line, "If-Stmt Syntax Error");
		return false;
	}*/
    
   // cout << "logic is " << v << endl;
    
    if(v.IsBool())
    {
        if(v.GetBool()){ //If statment logic is true
            t = Parser::GetNextToken(in, line);
            if(t.GetToken() != THEN)
            {
                ParseError(line, "If-Stmt Syntax Error");
                return false;
            }

            status = Stmt(in, line);
            if(!status)
            {
                ParseError(line, "Missing Statement for If-Stmt Then-Part");
                return false;
            }

            t = Parser::GetNextToken(in, line);
            while(t.GetToken() != SEMICOL)
                t = Parser::GetNextToken(in, line);

            //Parser::PushBackToken(t);
        }
        else{ //skip if, go to possible else
            t = Parser::GetNextToken(in, line);
            while(t != ELSE && t != SEMICOL)
                t = Parser::GetNextToken(in, line);
            //Parser::PushBackToken(t);

            if(t.GetToken() == ELSE ) {

                status = Stmt(in, line);
                if(!status)
                {
                    ParseError(line, "Missing Statement for If-Stmt Else-Part");
                    return false;
                }

                //cout << "in IFStmt status of Stmt true" << endl;
                return true;
            }
            //Parser::PushBackToken(t);
        }
        
    }  
    
    
	Parser::PushBackToken(t);
	return true;
}

//LogicExpr ::= Expr (= | > | <) Expr 
bool LogicExpr(istream& in, int& line, Value & retVal){
	Value val1, val2;
	//cout << "in LogicExpr" << endl;
	bool t1 = Expr(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	//cout << "val1 " << val1 << endl;
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	
	if (tok.GetToken() == GTHAN)
	{
		t1 = Expr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing expression after relational operator");
			return false;
		}
        //cout << "val2 = " << val2 << endl;
        //cout << val1.IsInt() << " " << val2.IsInt() << endl;
        //cout << (val1 > val2) << endl;
        retVal = (val1 > val2);
        //cout << "logic = " << retVal << endl;
		return true;
	}
    else if (tok.GetToken() == EQUAL)
	{
		t1 = Expr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing expression after relational operator");
			return false;
		}
        //cout << "val2 = " << val2 << endl;
		retVal = (val1 == val2);
        if(retVal.IsErr()){
            ParseError(line, "Illegal Equal");
            return false;
        }
        //cout << "logic = " << retVal << endl;
		return true;
	}
    else if (tok.GetToken() == LTHAN)
	{
		t1 = Expr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing expression after relational operator");
			return false;
		}
       // cout << "val2 = " << val2 << endl;
		retVal = (val1 < val2);
        //cout << "logic = " << retVal << endl;
		return true;
	}
	else{
		//Parser::PushBackToken(tok);
		ParseError(line, "Missing First Expression");
		return false;
	}
}