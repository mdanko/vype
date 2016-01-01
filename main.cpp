#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.tab.h"
#include "error.h"
using namespace std;

int main(int argc, char **argv) 
{
	ifstream inFile;
	ofstream outFile;
	extern FILE* yyin;

	error = EOK;

	if (argc < 2 || argc > 3)
	{
		setError(EINT, "Wrong number of arguments");
		return error;
	}

	if (!(yyin = fopen (argv[1], "r")))
	{
		setError(EINT, "Cannot open input file");
		return error;	
	}

	if (argc == 2)
	{
		outFile.open("out.asm");
	}
	else outFile.open(argv[2]);

	if (!outFile)
	{
		setError(EINT, "Cannot open output file");
		fclose(yyin);
		return error;		
	}

	extern int yydebug;
	extern int yyparse();
	//yydebug = 1;
	tmpID = 0;

	symbol *global = new symbol;
	global->name = "@global";
	scope.push_back(global);
	currScope = scope.front();

	yyparse();

	parseEnd();
	printSTable(&scope.front()->func.symbols);

	fclose(yyin);
	outFile.close();

	return error;
}
