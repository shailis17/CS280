/* Definitions and some functions implementations
 * parse.cpp to be completed - Actual Assignment
 * Programming Assignment 2
 * Spring 2022
*/

#include "parse.h"


map<string, bool> defVar;
map<string, Token> SymTable;

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
        cout << t << " " << t.GetLinenum() << endl;
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
	LexItem tok = Parser::GetNextToken(in, line);
    //cout << tok << " " << tok.GetLinenum() << endl;
    
    if(tok.GetToken() == ERR){
        ParseError(line, "Syntactic error in Declaration Block.");
	    return false;
    }
    else if(tok.GetToken() != IDENT){
		ParseError(line, "Missing Identifier in declaration statement");
        //cout << tok << " " << tok.GetLinenum() << endl;
		return false;
	}
    else{
        if((defVar.find(tok.GetLexeme())->second)){
            ParseError(line, "Variable Redefinition");
            return false;
        }
    }
		
    defVar.insert({tok.GetLexeme(), true});
	
	tok = Parser::GetNextToken(in, line);
    	
	if(tok.GetToken() == COMMA) {
        //cout << tok << tok.GetLinenum() << endl;
		return DeclStmt(in, line);
	}
    	
	if(tok.GetToken() == IDENT){
		ParseError(line, "Unrecognized Input Pattern");
        //cout << tok << " " << tok.GetLinenum() << endl;
		return false;
	}
	
    //tok = Parser::GetNextToken(in, line); 
    
	if(tok.GetToken() != COLON){
		ParseError(line, "Missing colon");
        cout << tok << " " << tok.GetLinenum() << endl;
		return false;
	}
	
    tok = Parser::GetNextToken(in, line);
    if(tok.GetToken() != STRING && tok.GetToken() != INTEGER && tok.GetToken() != REAL){
        ParseError(line, "Incorrect Declaration Type.");
        return false;
    }
    
    return true;

	
}

//ProgBody ::= BEGIN {Stmt;} END
bool ProgBody(istream& in, int& line){
	LexItem t = Parser::GetNextToken(in, line);
	if(t.GetToken() != BEGIN){
		ParseError(line, "Missing BEGIN token");
        cout << t << " " << t.GetLinenum() << endl;
		return false;
	}
	
	bool status = Stmt(in, line);

	while(status){
		t = Parser::GetNextToken(in, line);
        //cout << t << " " << t.GetLinenum() << endl;
		if(t.GetToken() != SEMICOL){
            //Parser::PushBackToken(t);
            //cout << t << " " << t.GetLinenum() << endl;
			ParseError(line - 1, "Missing semicolon in Statement.");
            //cout << t << " " << t.GetLinenum() << endl;
			return false;
		}
		status = Stmt(in, line);
	}

    t = Parser::GetNextToken(in, line);
	if(t.GetToken() != END){
		ParseError(line, "Missing END token");
        cout << t << " " << t.GetLinenum() << endl;
		return false;
	}

	return true;
}



//Stmt is either a WriteLnStmt, ForepeatStmt, IfStmt, or AssigStmt
//Stmt = AssigStmt | IfStmt | WriteStmt | ForStmt 
bool Stmt(istream& in, int& line) {
	bool status;
	//cout << "in ContrlStmt" << endl;
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) 
	{
		case WRITELN:
			status = WriteLnStmt(in, line);
			//cout << "After WriteStmet status: " << (status? true:false) <<endl;
			break;
		case IF:
			status = IfStmt(in, line);
			break;
		case IDENT:
			Parser::PushBackToken(t);
			status = AssignStmt(in, line);
			break;	
		case FOR:
			status = ForStmt(in, line);
			break;		
		default:
			Parser::PushBackToken(t);
			return false;
	}

	return status;
}//End of Stmt


//WriteStmt:= wi, ExpreList 
bool WriteLnStmt(istream& in, int& line) {
	LexItem t;
	//cout << "in WriteStmt" << endl;
	
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
	//Evaluate: print out the list of expressions values

	return ex;
}

//IfStmt ::= IF ( LogicExpr ) THEN Stmt [ELSE Stmt]
bool IfStmt(istream& in, int& line){
	LexItem tok = Parser::GetNextToken(in, line);
	/*if(tok != IF){
		ParseError(line, "Missing IF");
		return false;
	}
	tok = Parser::GetNextToken(in, line);*/
	if(tok.GetToken() != LPAREN){
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	bool logic = LogicExpr(in, line);
	if(!logic){
		ParseError(line, "Missing Logic Expression");
		return false;
	}
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() != RPAREN){
		ParseError(line, "Missing Right Parenthesis");
        cout << tok << " " << tok.GetLinenum() << endl;
		return false;
	}
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() != THEN){
		ParseError(line, "If-Stmt Syntax Error");
        cout << tok << " " << tok.GetLinenum() << endl;
		return false;
	}
	bool stmt = Stmt(in, line);
	if(!stmt){
		ParseError(line, "Missing Statement");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ELSE){
		stmt = Stmt(in, line);
		if(!stmt){
			ParseError(line, "Missing Else Statement");
			return false;
		}
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}

	return true;
}

// LogicExpr ::= Expr (= | > | <) Expr
bool LogicExpr(istream& in, int& line){
	bool status = Expr(in, line);
	if(!status){
		ParseError(line, "Missing First Expression");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == EQUAL || tok.GetToken() == GTHAN || tok.GetToken() == LTHAN){
		status = Expr(in, line);
		if(!status){
			ParseError(line, "Missing First Expression");
			return false;
		}
		else
			return true;
	}
	else{
		Parser::PushBackToken(tok);
		ParseError(line, "Missing First Expression");
		return false;
	}
}

//AssignStmt ::= Var := Expr
bool AssignStmt(istream& in, int& line){
	bool status = Var(in, line);
	if(!status){
		ParseError(line, "Missing Var");
		return false;
	}
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() != ASSOP){
		ParseError(line, "Missing ASSOP operator");
		return false;
	}
	status = Expr(in, line);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	else{
		return true;
	}
}

//Var ::= IDENT
bool Var(istream& in, int& line){
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == IDENT){
        if (!(defVar.find(tok.GetLexeme())->second)){
            ParseError(line, "Undefined Variable");
            return false;
        }
		return true;
    }
	else{
		return false;
	}
}

//ForStmt ::= FOR Var := ICONST (TO | DOWNTO) ICONST DO Stmt
bool ForStmt(istream& in, int& line){
	bool status = Var(in, line);
	if(!status){
		ParseError(line, "Missing Var");
		return false;
	}
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() != ASSOP){
		ParseError(line, "Missing ASSOP operator");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() != ICONST){
		ParseError(line, "Missing ICONST");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() != TO && tok.GetToken() != DOWNTO){
		ParseError(line, "Missing TO/DOWNTO keyword");
        cout << tok << " " << tok.GetLinenum() << endl;
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() != ICONST){
		ParseError(line, "Missing ICONST");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() != DO){
		ParseError(line, "Missing DO");
		return false;
	}

	status = Stmt(in, line);
	return status;
}


//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	//cout << "in ExprList and before calling Expr" << endl;
	status = Expr(in, line);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok.GetToken() == COMMA) {
		//cout << "before calling ExprList" << endl;
		status = ExprList(in, line);
		//cout << "after calling ExprList" << endl;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

//Expr ::= Term {(+|-) Term}
bool Expr(istream& in, int& line)
{
	bool status = Term(in, line);
	if(!status){
		ParseError(line, "Missing Term");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok.GetToken() == PLUS || tok.GetToken() == MINUS) {
		status = Expr(in, line);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
    //cout << tok << " " << tok.GetLinenum() << endl;
	return status;
}

//Term ::= SFactor {( * | / ) SFactor} 
bool Term(istream& in, int& line){
	bool status = SFactor(in, line);
    LexItem tok;
	if(!status){
		ParseError(line, "Missing SFactor");
		return false;
	}
	else{
		while(status){
            tok = Parser::GetNextToken(in, line);
            if (tok.GetToken() == MULT || tok.GetToken() == DIV) {
                status = SFactor(in, line);
            }
            else{
                status = false;
            }
		}
	}
	Parser::PushBackToken(tok);
	return true;
    
    //cout << tok << " " << tok.GetLinenum() << endl;
	//return status;
}

//SFactor ::= [(+ | -)] Factor 
bool SFactor(istream& in, int& line){
	bool status = false;
	LexItem tok = Parser::GetNextToken(in, line);
	
	if(tok.GetToken() == PLUS || tok.GetToken() == MINUS)
	{
		status = Factor(in, line, 0);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		status = Factor(in, line, 0);
	}
    //cout << tok << " " << tok.GetLinenum() << endl;
	return status;
}

//Factor ::= IDENT | ICONST | RCONST | SCONST | (Expr)
bool Factor(istream& in, int& line, int sign)
{
	bool status = false;
	LexItem tok = Parser::GetNextToken(in, line);

	if(tok.GetToken() == IDENT || tok.GetToken() == ICONST || tok.GetToken() == RCONST || tok.GetToken() == SCONST){
		//cout << tok << " " << tok.GetLinenum() << endl;
        if(tok.GetToken() == IDENT){
            if (!(defVar.find(tok.GetLexeme())->second)){
                ParseError(line, "Using Undefined Variable");
                return false;
            }   
        }
        return true;
    }
	else if(tok == LPAREN){
		bool status = Expr(in, line);
		if(!status){
			ParseError(line, "Missing expression in factor");
			return false;
		}
		else{
			LexItem tok = Parser::GetNextToken(in, line);
			if(tok.GetToken() != RPAREN){
				ParseError(line, "Missing closing parenthesis");
				return false;
			}
			else{
				return true;
			}
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
        return false;
	}
	return status;
}