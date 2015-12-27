%{
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "semantic.h"
#include "parser.tab.h"
using namespace std;

#define TOKEN(t) (yylval.token = t)

extern "C" int yywrap() { }
extern "C" int yyerror() {  printf("Error\n");   exit(1); }
    extern FILE* yyin;

void blockComment();
void lineComment();
void count();
%}

%%
"/*"					{ blockComment(); }
"//"					{ lineComment(); }

"char"					return TOKEN(TKEY_CHAR);
"else"					return TOKEN(TKEY_ELSE);
"if"					return TOKEN(TKEY_IF);
"int"					return TOKEN(TKEY_INT);
"return"				return TOKEN(TKEY_RETURN);
"string"				return TOKEN(TKEY_STRING);
"void"					return TOKEN(TKEY_VOID);
"while"					return TOKEN(TKEY_WHILE);

"break"					return TOKEN(TRES_BREAK);
"continue"				return TOKEN(TRES_CONT);
"for"					return TOKEN(TRES_FOR);
"short"					return TOKEN(TRES_SHORT);
"unsigned"				return TOKEN(TRES_UNSIGNED);

[ \n\t]                 	;

[a-zA-Z_][a-zA-Z0-9_]*  { yylval.id = new string(yytext); return TID; }
[0-9]+                  { yylval.ival = atoi(yytext); return TINT; }
'(\\.|[^\\'])'			{ yylval.cval = yytext[0]; return TCHAR; }
\"(\\.|[^\\"])*\"		{ yylval.sval = new string(yytext); return TSTRING; }
"="                     return TOKEN(TASS);
"=="                    return TOKEN(TOP_EQ);
"!="                    return TOKEN(TOP_NEQ);
"<"                     return TOKEN(TOP_LT);
"<="                    return TOKEN(TOP_LTE);
">"                     return TOKEN(TOP_GT);
">="                    return TOKEN(TOP_GTE);
"+"                     return TOKEN(TOP_PLUS);
"-"                     return TOKEN(TOP_MINUS);
"*"                     return TOKEN(TOP_MUL);
"/"                     return TOKEN(TOP_DIV);
"%"						return TOKEN(TOP_MOD);
"!"						return TOKEN(TOP_NEG);
"&&"					return TOKEN(TOP_AND);
"||"					return TOKEN(TOP_OR);
"("                     return TOKEN(TLPAR);
")"                     return TOKEN(TRPAR);
"{"                     return TOKEN(TLBRAC);
"}"                     return TOKEN(TRBRAC);
"."                     return TOKEN(TDOT);
","                     return TOKEN(TCOM);
";"						return TOKEN(TSEMICOL);

.                       printf("Unknown token!\n"); yyerror();

%%

/*int main(int argc, char **argv) 
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

	int tok;

	while(1) 
	{
		tok = yylex();
		printf("%d ", tok);
		
		if (tok == TID || tok == TINT || tok == TDOUBLE || tok == TCHAR || tok == TSTRING) cout << *yylval.literal << endl; 
		else cout << yylval.token << endl;
		//printf("\n");
	}

	return 0;
}*/

void blockComment()
{
	char c, c1;

loop:
	while ((c = yyinput()) != '*' && c != 0)
		putchar(c);

	if ((c1 = yyinput()) != '/' && c != 0)
	{
		unput(c1);
		goto loop;
	}

	if (c != 0)
		putchar(c1);
}

void lineComment()
{
	char c;

	while ((c = yyinput()) != '\n' && c != 0)
		putchar(c); 
}

int column = 0;

void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;

	ECHO;
}

