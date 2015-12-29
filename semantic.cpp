#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "semantic.h"
using namespace std;


symbol *currScope;
deque<int> funcArgTypes;
deque<symbol*> funcArgs;
deque<symbol*> callArgs;
deque<symbol*> scope;
int tmpID;

void parseEnd(deque<symbol*> *table)
{
	bool isMain = false;
	for (deque<symbol*>::iterator id=table->begin(); id!=table->end(); ++id)
	{
		if ((*id)->type == SFUNC)
		{
			if (!(*id)->func.isDefined)
			{
				// error - declared func not defined
			}
			else 
			{
				if ((*id)->name == string("main") && (*id)->dataType == DINT && (*id)->func.argTypes.size() == 0)
					isMain = true;

				parseEnd(&(*id)->func.symbols);
			}
		}
	}

	if (isMain)
		cout << "MAIN" << endl;

}

void printSTable(deque<symbol*> *table)
{
	cout << "SYMBOL TABLE" << endl;
	for (deque<symbol*>::iterator id=table->begin(); id!=table->end(); ++id)
	{
		if ((*id)->type == SID)
			cout << "*** NAME:" << (*id)->name << " TYPE:" <<  (*id)->type << " DATATYPE:" <<  (*id)->dataType << " GLOBAL:" << (*id)->var.isGlobal << endl;
		else
			cout << "*** NAME:" << (*id)->name << " TYPE:" <<  (*id)->type << " DATATYPE:" <<  (*id)->dataType << endl;
		if ((*id)->type == SFUNC || (*id)->type == SIF || (*id)->type == SELSE || (*id)->type == SWHILE)
			printSTable(&(*id)->func.symbols);

	}
	cout << "END OF TABLE" << endl;
}

bool checkID(string *name)
{
	// check local vars
	for (deque<symbol*>::iterator id=currScope->func.symbols.begin(); id!=currScope->func.symbols.end(); ++id)
	{
		if ((*id)->type == SID && (*id)->name == *name)
		{
			// error - symbol already exists
			return false;
		}		
	}	

	// check global vars
	deque<symbol*> *table;
	table = &scope.front()->func.symbols;
	for (deque<symbol*>::iterator id=table->begin(); id!=table->end(); ++id)
	{
		if ((*id)->type == SID && (*id)->name == *name)
		{
			// error - symbol already exists
			return false;
		}		
	}	
	return true;
}

bool checkFunc(string *name)
{
	deque<symbol*> *table;
	table = &scope.front()->func.symbols;

	for (deque<symbol*>::iterator id=table->begin(); id!=table->end(); ++id)
	{
		if ((*id)->type == SFUNC && (*id)->name == *name && (*id)->func.isDefined)
		{
			// error - symbol already exists
			return false;
		}		
	}	
	return true;	
}

/* Identificator */

void addID(string *name)
{
	if (checkID(name) && checkFunc(name))
	{
		symbol *s = new symbol;
		s->name = *name;
		s->type = SID;
		s->dataType = INIT;

		if (currScope == scope.front())
			s->var.isGlobal = true;
		else s->var.isGlobal = false;

		currScope->func.symbols.push_back(s);		
	}
	else
	{
		// error - wrong name
	}
}

void addTypes(int type)
{
	for (deque<symbol*>::iterator id=currScope->func.symbols.begin(); id!=currScope->func.symbols.end(); ++id)
	{
		if ((*id)->dataType == INIT)
		{
			(*id)->dataType = type;
		}		
	}
}

/* Function */

void addFuncDecl(string *name, int type)
{
	if (checkID(name) && checkFunc(name))
	{
		symbol *s = new symbol;
		s->name = *name;
		s->type = SFUNC;
		s->dataType = type;
		s->func.argTypes = funcArgTypes;
		s->func.isDefined = false;
		funcArgTypes.clear();
		currScope->func.symbols.push_back(s);
	}
	else
	{
		// error - wrong name
	}
}

void addArgType(int type)
{
	funcArgTypes.push_back(type);
}

void addFuncDefin(string *name, int type)
{	
	if (!(checkID(name) && checkFunc(name)))
	{
		// error - wrong name
	}

	deque<symbol*>::iterator id = currScope->func.symbols.begin();

  	while (id!=currScope->func.symbols.end() && !((*id)->name == *name && (*id)->type == SFUNC))
  		++id;


	if (id == currScope->func.symbols.end())
	{ // function is not declared
		deque<symbol*>::iterator arg = funcArgs.begin();
		while (arg!=funcArgs.end())
  		{
  			funcArgTypes.push_back((*arg)->dataType);
  			++arg;  			
  		}

		addFuncDecl(name, type);
		*id = currScope->func.symbols.back();
	}

	if ((*id)->func.isDefined)
	{
		// error - already defined
	}

	if ((*id)->dataType != type)
	{
		// error - wrong return type
	}

	if ((*id)->func.argTypes.size() == funcArgs.size())
	{// check arguments
		deque<int>::iterator argDecl = (*id)->func.argTypes.begin();
		deque<symbol*>::iterator argDefin = funcArgs.begin();
		while((!(*id)->func.isDefined || argDecl != (*id)->func.argTypes.end()) && argDefin != funcArgs.end())
		{
			if (!(*id)->func.isDefined || *argDecl == (*argDefin)->dataType)
			{
				
				symbol *arg = *argDefin;
				(*id)->func.symbols.push_back(arg);
				++argDecl;
				++argDefin;
			}
			else
			{
				// error - wrong types of defined args
			}
		}
	}
	else
	{
		// error - wrong number of defined args
	}

	(*id)->func.isDefined = true;
	
	symbol *newScope = *id;
	addScope(newScope);

	funcArgs.clear();
}

void addArg(string *name, int type)
{
	if (!checkFunc(name))
	{
		// error - wrong name
	}

	symbol *s = new symbol;
	s->name = *name;
	s->type = SID;
	s->dataType = type;
	funcArgs.push_back(s);
}


/* Assignment */
void addAssign(string *name, symbol* expr)
{
	deque<symbol*>::iterator id = currScope->func.symbols.begin();
  	while (id!=currScope->func.symbols.end() && (*id)->name != *name)
  	{
  		++id;
  	 }	

	if (id == currScope->func.symbols.end())
	{
		// error - not declared
	}

	if ((*id)->dataType != expr->dataType)
	{
		// error - wrong assignment type
	}

	// add new value from expr to id

}

/* Expression */
symbol* addTmpID(string *name)
{
	deque<symbol*>::iterator id = currScope->func.symbols.begin();

  	while (id!=currScope->func.symbols.end() && (*id)->name != *name)
  		++id;

	if (id == currScope->func.symbols.end())
	{
		// error - not declared
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
	tmpName << "@INT" << tmpID++;
	s->name =  tmpName.str();
	s->type = STEMP;
	s->dataType = DINT;
	s->var.ival = ival;

	currScope->func.symbols.push_back(s);

	return s;
}

symbol* addTmpChar(char cval)
{
	symbol *s = new symbol;
	ostringstream tmpName;
	tmpName << "@CHAR" << tmpID++;
	s->name =  tmpName.str();
	s->type = STEMP;
	s->dataType = DCHAR;
	s->var.cval = cval;

	currScope->func.symbols.push_back(s);

	return s;
}

symbol* addTmpString(string *sval)
{
	symbol *s = new symbol;
	ostringstream tmpName;
	tmpName << "@STR" << tmpID++;
	s->name =  tmpName.str();
	s->type = STEMP;
	s->dataType = DSTRING;
	s->var.sval = *sval;

	currScope->func.symbols.push_back(s);

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
				   }	
				   break;
		default: break;
    }



	symbol *s = new symbol;
	ostringstream tmpName;
	tmpName << "@" << op << "OP" << tmpID++;
	s->name =  tmpName.str();
	s->type = STEMP;
	s->dataType = expr1->dataType;
	//s.var.sval = sval;

	currScope->func.symbols.push_back(s);

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
	tmpName << "@NEG" << tmpID++;
	s->name =  tmpName.str();
	s->type = STEMP;
	s->dataType = DINT;
	//s.var.sval = sval;

	currScope->func.symbols.push_back(s);

    return s;
}

symbol* addConvert(symbol *expr, int type)
{
	if (expr->dataType == type)
		return expr;

	if (!(expr->dataType == DCHAR && type == DSTRING) &&
	    !(expr->dataType == DCHAR && type == DINT) &&
	    !(expr->dataType == DINT && type == DCHAR))
	{
		// error - unsupported conversion
 	}

	symbol *s = new symbol;
	ostringstream tmpName;
	tmpName << "@CONV" << tmpID++;
	s->name = tmpName.str();
	s->type = STEMP;
	s->dataType = type;
	//s.var.sval = sval;

	currScope->func.symbols.push_back(s);

    return s;
}

symbol* addFuncCall(string *name)
{
	deque<symbol*> *table;
	table = &scope.front()->func.symbols;

	deque<symbol*>::iterator id = table->begin();

  	while (id!=table->end() && (*id)->name != *name)
  		++id;

	if (id == table->end())
	{
		// error - not declared
		return NULL;
	}

	if ((*id)->dataType != DVOID)
	{ // func with arguments
		symbol *sid = *id;

		deque<symbol*>::iterator argDefin = sid->func.symbols.begin();
		deque<symbol*>::iterator argCall = callArgs.begin();

		while(argCall != callArgs.end() && argDefin != (*id)->func.symbols.end())
		{
			if ((*argDefin)->dataType == (*argCall)->dataType)
			{
				++argDefin;
				++argCall;
				// pass values
			}
			else
			{
				// error - wrong types of args
				return NULL;
			}
		}
	}

	symbol *s = new symbol;
	ostringstream tmpName;
	tmpName << "@CALL" << (*id)->name << tmpID++;
	s->name =  tmpName.str();
	s->type = STEMP;
	s->dataType = (*id)->dataType;
	//s.var.sval = sval;

	currScope->func.symbols.push_back(s);


	callArgs.clear();

    return s;
}

void addCallArg(symbol *arg)
{
	deque<symbol*>::iterator id = currScope->func.symbols.begin();

  	while (id!=currScope->func.symbols.end() && ((*id)->name != arg->name))
  	{
  		++id;
  	}

	if (id == currScope->func.symbols.end())
	{
		// error - not declared
	}

	callArgs.push_back(*id);
}

void addReturn(symbol *expr)
{
	int type;

	if (!expr)
		type = DVOID;
	else type = expr->dataType;


	if (type != currScope->dataType)
	{
		// error - wrong return type
	}
	// add funcs return value ... retval = expr->var.val
}


void addScope(symbol *newScope)
{
	scope.push_back(newScope);
	currScope = scope.back();
}

void leaveScope()
{
	scope.pop_back();
	currScope = scope.back();
}

void addIf(symbol *expr)
{
	if (expr->dataType != DINT)
	{
		// error - if expr
	}
	else
	{
		symbol *s = new symbol;
		ostringstream tmpName;
		tmpName << "@IF" << tmpID;
		s->name =  tmpName.str();
		s->type = SIF;
		s->dataType = DINT;
		addLocalVars(s);
		
		currScope->func.symbols.push_back(s);

		symbol *s2 = new symbol;
		ostringstream tmpName2;
		tmpName2 << "@ELSE" << tmpID++;
		s2->name =  tmpName2.str();
		s2->type = SELSE;
		s2->dataType = INIT;
		addLocalVars(s2);

		currScope->func.symbols.push_back(s2);

		addScope(s);
	}
}

void addElse()
{
	for (deque<symbol*>::iterator id=currScope->func.symbols.begin(); id!=currScope->func.symbols.end(); ++id)
	{

		if ((*id)->type == SELSE && (*id)->dataType == INIT)
		{	

			(*id)->dataType = DINT;
			
			symbol *newScope = *id;
			addScope(newScope);

			return;
		}
	}
}

void addWhile(symbol *expr)
{
	if (expr->dataType != DINT)
	{
		// error - if expr
	}
	else
	{
		symbol *s = new symbol;
		ostringstream tmpName;
		tmpName << "@WHILE" << tmpID++;
		s->name =  tmpName.str();
		s->type = SIF;
		addLocalVars(s);
		currScope->func.symbols.push_back(s);

		addScope(s);		
	}
}

void addLocalVars(symbol *s)
{
	deque<symbol*>::iterator id = currScope->func.symbols.begin();

  	while (id!=currScope->func.symbols.end())
  	{
  		if ((*id)->type == SID)
  			s->func.symbols.push_back(*id);

  		++id;
  	}
}