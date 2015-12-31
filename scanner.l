%option noyywrap batch
%{
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "parser.tab.h"
using namespace std;

#define TOKEN(t) (yylval.token = t)

extern FILE* yyin;

void blockComment();
void lineComment();
void count();
%}

PRINTABLE [\x20-\x21\x23-\x26\x28-\x5b\x5d-\xff] 
ESCAPE 	  (\\n|\\t|\\\\|\\\"|\\')

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

[ \n\t\r]+                 	;

[a-zA-Z_][a-zA-Z0-9_]*  { yylval.id = new string(yytext); return TID; }
[0-9]+                  { yylval.ival = atoi(yytext); return TINT; }
'(\\|\"|{PRINTABLE}|{ESCAPE})'	{ yylval.cval = yytext[0]; return TCHAR; }
\"({PRINTABLE}|{ESCAPE})*\"		{ yylval.sval = new string(yytext); return TSTRING; }
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

.                      { setError(ELEX, "Read invalid character"); }

%%

void blockComment()
{
	char c, c1;

	while(1)
	{
		while ((c = yyinput()) != '*' && c != 0)
		{
			if (c == EOF)
			{
				setError(ELEX, "Block comment reached eof");
				return;
			}
			putchar(c); 
		}

		if ((c1 = yyinput()) != '/' && c != 0)
		{
			if (c1 == EOF)
				{
					setError(ELEX, "Block comment reached eof");
					return;
				}
			unput(c1);
		}

		if (c != 0)
			putchar(c1);
		}
}

void lineComment()
{
	char c;

	while ((c = yyinput()) != '\n' && c != 0 && c != EOF)
	{		
		putchar(c); 
	}
}
