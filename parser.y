%{
	#include <iostream>
	#include <fstream>
	#include <string.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include "semantic.h"
	using namespace std;

    extern int yylex();
    extern int yyparse();
    extern FILE* yyin;

    extern int yylineno;
    extern char *yytext;

    void yyerror(const char *s) { printf("%d: %s at %s\n", yylineno, s, yytext); }
%}

%union {
    string *id;
    int ival;
    char cval;
    string *sval;
    int token;

    int dtval;
    symbol *eval;
}

%token <id> TID
%token <ival> TINT
%token <cval> TCHAR
%token <sval> TSTRING
%token TKEY_CHAR TKEY_ELSE TKEY_IF TKEY_INT TKEY_RETURN TKEY_STRING TKEY_VOID TKEY_WHILE
%token <token> TRES_BREAK TRES_CONT TRES_FOR TRES_SHORT TRES_UNSIGNED
%token <token> TASS TLPAR TRPAR TLBRAC TRBRAC TDOT TCOM TSEMICOL TEOF
%token <token> TOP_EQ TOP_NEQ TOP_LT TOP_LTE TOP_GT TOP_GTE TOP_PLUS TOP_MINUS TOP_MUL TOP_DIV TOP_MOD
%token <token> TOP_NEG TOP_AND TOP_OR

%left TOP_OR
%left TOP_AND
%left TOP_EQ TOP_NEQ
%left TOP_LT TOP_LTE TOP_GT TOP_GTE
%left TOP_PLUS TOP_MINUS
%left TOP_MUL TOP_DIV TOP_MOD
%nonassoc TOP_NEG
%nonassoc TLPAR TRPAR

%type <dtval> data_type TKEY_INT TKEY_CHAR TKEY_STRING
%type <eval> expr

%start program
%%

program : stmts
		;

stmts : stmt 
      | stmts stmt
      ;

stmt : func_decl TSEMICOL
	 | func_def
	 | return TSEMICOL
	 | id_decl TSEMICOL
	 | assign TSEMICOL
	 | expr TSEMICOL
	 | if 
	 | while
	 | TSEMICOL
     ;

func_decl : data_type TID TLPAR func_decl_args TRPAR		{ addFuncDecl($2, $1); }
		  | data_type TID TLPAR TKEY_VOID TRPAR				{ addFuncDecl($2, $1); }
		  | TKEY_VOID TID TLPAR func_decl_args TRPAR		{ addFuncDecl($2, DVOID); }
		  | TKEY_VOID TID TLPAR TKEY_VOID TRPAR				{ addFuncDecl($2, DVOID); }
          ;
    
func_decl_args : data_type TCOM func_decl_args				{ addArgType($1); }
			   | data_type									{ addArgType($1); }
         	   ;

func_def : data_type TID TLPAR func_def_args TRPAR 			{ addFuncDefin($2, $1); }
		   block											
		 | data_type TID TLPAR TKEY_VOID TRPAR				{ addFuncDefin($2, $1); }
		   block											
		 | TKEY_VOID TID TLPAR func_def_args TRPAR			{ addFuncDefin($2, DVOID); }
		   block 											
		 | TKEY_VOID TID TLPAR TKEY_VOID TRPAR				{ addFuncDefin($2, DVOID); }
		   block											
		 ;

func_def_args : data_type TID TCOM func_def_args			{ addArg($2, $1); }
		  | data_type TID 									{ addArg($2, $1); }
		  ;

return : TKEY_RETURN expr
	   | TKEY_RETURN
	   ;

block : TLBRAC 
		stmts 
		TRBRAC				{ leaveScope(); }		
      | TLBRAC 
        TRBRAC				{ leaveScope(); }
      ;

id_decl : data_type ids 	{ addTypes($1); }
		;

ids : TID 					{ addID($1); }
	| TID TCOM ids			{ addID($1); }
	;

data_type : TKEY_INT		{ $$ = DINT; }
		  | TKEY_CHAR		{ $$ = DCHAR; }
		  | TKEY_STRING		{ $$ = DSTRING; }
		  ;

assign : TID TASS expr		{ addAssign($1, $3); }
	   ;

expr : expr TOP_PLUS expr	{ $$ = addExpr($1, $3, OPLUS); }
	 | expr TOP_MINUS expr 	{ $$ = addExpr($1, $3, OMINUS); }
	 | expr TOP_MUL expr  	{ $$ = addExpr($1, $3, OMUL); }
	 | expr TOP_DIV expr 	{ $$ = addExpr($1, $3, ODIV); }
	 | expr TOP_MOD expr 	{ $$ = addExpr($1, $3, OMOD); }

	 | expr TOP_AND expr 	{ $$ = addExpr($1, $3, OAND); }
	 | expr TOP_OR expr 	{ $$ = addExpr($1, $3, OOR); }

	 | expr TOP_EQ expr  	{ $$ = addExpr($1, $3, OEQ); }
	 | expr TOP_NEQ expr  	{ $$ = addExpr($1, $3, ONEQ); }
	 | expr TOP_LT expr 	{ $$ = addExpr($1, $3, OLT); }
	 | expr TOP_LTE expr  	{ $$ = addExpr($1, $3, OLTE); }
	 | expr TOP_GT expr  	{ $$ = addExpr($1, $3, OGT); }
	 | expr TOP_GTE expr 	{ $$ = addExpr($1, $3, OGTE); }

	 | TOP_NEG expr 		{ $$ = addExprNeg($2); }

	 | TLPAR data_type TRPAR expr 		{ $$ = addConvert($4, $2);}
	 | TID TLPAR func_call_args TRPAR	{ $$ = addFuncCall($1); }

	 | TID 					{ $$ = addTmpID($1); }
	 | TINT 				{ $$ = addTmpInt($1); }
	 | TCHAR 				{ $$ = addTmpChar($1); }
	 | TSTRING 				{ $$ = addTmpString($1); }

	 | TLPAR expr TRPAR		{ $$ = $2; }
	 ;

func_call_args : expr TCOM func_call_args		{ addCallArg($1); }
			   | expr 							{ addCallArg($1); }
			   ;

if : TKEY_IF TLPAR expr TRPAR block TKEY_ELSE block
   ;

while : TKEY_WHILE TLPAR expr TRPAR block
	  ;


%%
int main(int argc, char **argv) 
{
	ofstream outFile;

	if (argc < 1 || argc >= 3)
	{
		return 1;
	}

	printf("%s %s\n", argv[1], argv[2]);
	yyin = fopen (argv[1], "rw");

	if (argc == 1)
	{
		outFile.open("out.asm");
	}
	else outFile.open(argv[2]);

	extern int yydebug;
	yydebug = 1;
	tmpID = 0;

	symbol *global = new symbol;
	global->name = "@global";
	scope.push_back(global);
	sTable = &scope.front()->func.symbols;

	symbol *s = new symbol;
	s->name = "aaaa";
	s->type = SID;
	s->dataType = INIT;
	sTable->push_back(s);

	yyparse();

	printSTable(&scope.front()->func.symbols);

	return 0;
}