#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "semantic.h"
using namespace std;


deque<symbol*> *sTable;
deque<int> funcArgTypes;
deque<symbol*> funcArgs;
deque<symbol*> callArgs;
deque<symbol*> scope;
int tmpID;

void printSTable(deque<symbol*> *table)
{
	cout << "SYMBOL TABLE" << endl;
	for (deque<symbol*>::iterator id=table->begin(); id!=table->end(); ++id)
	{
		cout << "*** NAME:" << (*id)->name << " TYPE:" <<  (*id)->type << " DATATYPE:" <<  (*id)->dataType << endl;
		if ((*id)->dataType == SFUNC)
			printSTable(&(*id)->func.symbols);
	}
	cout << "END OF TABLE" << endl;
}

/* Identificator */

void addID(string *name)
{
	symbol *s = new symbol;
	s->name = *name;
	s->type = SID;
	s->dataType = INIT;
	cout << "------> adding ID " << *name << " datatype " << s->dataType << endl;
	sTable->push_back(s);
}

void addTypes(int type)
{
	for (deque<symbol*>::iterator id=sTable->begin(); id!=sTable->end(); ++id)
	{
		if ((*id)->dataType == INIT)
		{
			cout << "------> adding Type " << type << " to ID " <<  (*id)->name << endl;
			(*id)->dataType = type;
		}		
	}
}

/* Function */

void addFuncDecl(string *name, int type)
{
	symbol *s = new symbol;
	s->name = *name;
	s->type = SFUNC;
	s->dataType = type;
	s->func.argTypes = funcArgTypes;
	funcArgTypes.clear();
	cout << "------> adding func " << *name << " with Type " << type << endl;
	sTable->push_back(s);
}

void addArgType(int type)
{
	funcArgTypes.push_back(type);
}

void addFuncDefin(string *name, int type)
{	
	deque<symbol*>::iterator id = sTable->begin();

	cout << "addFuncDefin" << endl;
  	while (id!=sTable->end() && !((*id)->name == *name && (*id)->type == SFUNC))
  		++id;

	if (id == sTable->end())
	{
		// error - not declared
		cout << "**************ERROR1*****************" << endl;
	}
	else
	{
		if ((*id)->dataType != type)
		{
			// error - wrong return type
			cout << "**************ERROR2*****************" << (*id)->name << " " << (*id)->dataType <<  " " << *name << " " << type << endl;
		}

		if ((*id)->func.argTypes.size() == funcArgs.size())
		{
			deque<int>::iterator argDecl = (*id)->func.argTypes.begin();
			deque<symbol*>::iterator argDefin = funcArgs.begin();

			while(argDecl != (*id)->func.argTypes.end() && argDefin != funcArgs.end())
			{
				cout << *argDecl << " " << (*argDefin)->dataType << endl;

				if (*argDecl == (*argDefin)->dataType)
				{
					cout << "adding" << endl;

					cout << "------> adding  argument" << (*argDefin)->name << " to function " << (*id)->name << endl;

					symbol *arg = *argDefin;
					(*id)->func.symbols.push_back(arg);
					cout << "added" << endl;
					++argDecl;
					++argDefin;
				}
				else
				{
					// error - wrong types of defined args
					cout << "**************ERROR4*****************" << endl;
					return;
				}
			}
		}
		else
		{
			// error - wrong number of defined args
			cout << "**************ERROR3*****************" << endl;
		}

	}
	symbol *newScope = *id;
	addScope(newScope);

	funcArgs.clear();
}

void addArg(string *name, int type)
{
	symbol *s = new symbol;
	s->name = *name;
	s->type = SID;
	s->dataType = type;
	cout << "------> adding arg " << *name << " with Type " << type << endl;
	funcArgs.push_back(s);
}


/* Assignment */
void addAssign(string *name, symbol* expr)
{
	deque<symbol*>::iterator id = sTable->begin();
	cout << "addAssign" << endl;
  	while (id!=sTable->end() && (*id)->name != *name)
  		++id;

	if (id == sTable->end())
	{
		// error - not declared
		cout << "**************ERROR1*****************" << endl;
	}
	else
	{

	}
}

/* Expression */
symbol* addTmpID(string *name)
{
	deque<symbol*>::iterator id = sTable->begin();

  	while (id!=sTable->end() && (*id)->name != *name)
  		++id;

	if (id == sTable->end())
	{
		// error - not declared
		cout << "**************ERROR1*****************" << endl;
	}
	else
	{

	}

	symbol *sid = *id;

	return sid;
}

symbol* addTmpInt(int ival)
{
	symbol *s = new symbol;
	ostringstream tmpName;
	tmpName << "@" << tmpID++;
	s->name =  tmpName.str();
	s->type = STEMP;
	s->dataType = DINT;
	s->var.ival = ival;

	sTable->push_back(s);

	return s;
}

symbol* addTmpChar(char cval)
{
	symbol *s = new symbol;
	ostringstream tmpName;
	tmpName << "@" << tmpID++;
	s->name =  tmpName.str();
	s->type = STEMP;
	s->dataType = DCHAR;
	s->var.cval = cval;

	sTable->push_back(s);

	return s;
}

symbol* addTmpString(string *sval)
{
	symbol *s = new symbol;
	ostringstream tmpName;
	tmpName << "@" << tmpID++;
	s->name =  tmpName.str();
	s->type = STEMP;
	s->dataType = DSTRING;
	s->var.sval = *sval;

	sTable->push_back(s);

	return s;
}

symbol* addExpr(symbol *expr1, symbol *expr2, int op)
{
	switch(op)
	{
		case OPLUS: 
		case OMINUS:
		case OMUL:
		case ODIV:
		case OMOD: 
		case OAND:
		case OOR:  if (expr1->dataType != DINT || expr2->dataType != DINT)
				   {
				   	 // error 
				   	cout << "**************ERROR1*****************"<< endl;
				   }						
					break;
		case OLT:
		case OLTE:
		case OGT:
		case OGTE:
		case OEQ:  
		case ONEQ: if (expr1->dataType != expr2->dataType)
				   {
				   	 // error
				   	cout << "**************ERROR1*****************" << endl;
				   }	
				   break;
		default: break;
    }

	symbol *s = new symbol;
	ostringstream tmpName;
	tmpName << "@" << tmpID++;
	s->name =  tmpName.str();
	s->type = STEMP;
	s->dataType = expr1->type;
	//s.var.sval = sval;

	sTable->push_back(s);

    return s;
}

symbol* addExprNeg(symbol *expr)
{
	if (expr->type != DINT)
	{
		// error
	}

	symbol *s = new symbol;
	ostringstream tmpName;
	tmpName << "@" << tmpID++;
	s->name =  tmpName.str();
	s->type = STEMP;
	s->dataType = DINT;
	//s.var.sval = sval;

	sTable->push_back(s);

    return s;
}

symbol* addConvert(symbol *expr, int type)
{
	deque<symbol*>::iterator id = sTable->begin();

  	while (id!=sTable->end() && ((*id)->name != expr->name && (*id)->type != STEMP))
  		++id;

	if (id == sTable->end())
	{
		// error - not declared
		cout << "**************ERROR1*****************" << endl;
	}

	symbol *s = new symbol;
	ostringstream tmpName;
	tmpName << "@" << tmpID++;
	s->name = tmpName.str();
	s->type = STEMP;
	s->dataType = type;
	//s.var.sval = sval;

	sTable->push_back(s);

    return s;
}

symbol* addFuncCall(string *name)
{
	deque<symbol*>::iterator id = sTable->begin();

  	while (id!=sTable->end() && ((*id)->name != *name && (*id)->type != SFUNC))
  		++id;

	if (id == sTable->end())
	{
		// error - not declared
		return NULL;
	}

	symbol *sid = *id;

	deque<symbol*>::iterator argDefin = sid->func.symbols.begin();
	deque<symbol*>::iterator argCall = callArgs.begin();

	while(argDefin != (*id)->func.symbols.end() && argCall != funcArgs.end())
	{
		if ((*argDefin)->type == (*argCall)->type)
		{
			++argDefin;
			++argCall;
		}
		else
		{
			// error - wrong types of args
			return NULL;
		}
	}

	symbol *s = new symbol;
	ostringstream tmpName;
	tmpName << "@" << tmpID++;
	s->name =  tmpName.str();
	s->type = STEMP;
	s->dataType = (*id)->type;
	//s.var.sval = sval;

	sTable->push_back(s);


	callArgs.clear();
    return s;
}

void addCallArg(symbol *arg)
{
	deque<symbol*>::iterator id = sTable->begin();

  	while (id!=sTable->end() || ((*id)->name == arg->name))
  		++id;

	if (id == sTable->end())
	{
		// error - not declared
		cout << "**************ERROR1*****************" << endl;
	}

	callArgs.push_back(*id);
}

void addScope(symbol *newScope)
{
	scope.push_back(newScope);
	sTable = &scope.back()->func.symbols;
}

void leaveScope()
{
	deque <symbol*> *table;
	table = &scope.back()->func.symbols;

	scope.pop_back();
	sTable = &scope.back()->func.symbols;

	printSTable(table);
}