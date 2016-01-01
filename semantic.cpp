#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "semantic.h"
using namespace std;


symbol *currScope;
deque<DataType> funcArgTypes;
deque<symbol*> funcArgs;
deque<symbol*> callArgs;
deque<symbol*> scope;
int tmpID;
deque<Label*> currLabel;
deque<JumpInst*> currJump;

void parseEnd()
{
	bool isMain = false;
	deque<symbol*> *table;
	table = &scope.front()->func.symbols;

	for (deque<symbol*>::iterator id=table->begin(); id!=table->end(); ++id)
	{
		if ((*id)->type == SFUNC)
		{
			if (!(*id)->func.isDefined)
			{
				setError(ESEM, "Declared function not defined");
			}
			else 
			{
				if ((*id)->name == string("main") && (*id)->dataType == DINT && (*id)->func.argTypes.size() == 0)
					isMain = true;
			}
		}
	}

	if (!isMain)
		setError(ESEM, "Main function not declared");

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
		setError(ESEM, "Variable name already exists");
		// error - wrong name
	}
}

void addTypes(DataType type)
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

void addFuncDecl(string *name, DataType type)
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
		setError(ESEM, "Function name already exists");
		// error - wrong name
	}
}

void addArgType(DataType type)
{
	funcArgTypes.push_back(type);
}

void addFuncDefin(string *name, DataType type)
{	
	if (!(checkID(name) && checkFunc(name)))
	{
		setError(ESEM, "Function name already exists");
		// error - wrong name
	}

	deque<symbol*>::iterator id = currScope->func.symbols.begin();

  	while (id!=currScope->func.symbols.end() && (*id)->name != *name)
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
		setError(ESEM, "Function already defined");
		// error - already defined
	}

	if ((*id)->dataType != type)
	{
		setError(ESEM, "Function invalid return type");
		// error - wrong return type
	}

	if ((*id)->func.argTypes.size() != 0 || (*id)->func.isDefined)
	{
		if ((*id)->func.argTypes.size() == funcArgs.size())
		{// check arguments
			deque<DataType>::iterator argDecl = (*id)->func.argTypes.begin();
			deque<symbol*>::iterator argDefin = funcArgs.begin();

			while ((argDecl != (*id)->func.argTypes.end()) && argDefin != funcArgs.end())
			{ 
				if (*argDecl == (*argDefin)->dataType)
				{ 
					symbol *arg = *argDefin; 
					(*id)->func.symbols.push_back(arg);
					++argDecl;
					++argDefin;
				}
				else
				{
					setError(ESEM, "Function arguments invalid types");
					break;
					// error - wrong types of defined args
				}
			}
		}
		else
		{
			setError(ESEM, "Function arguments invalid count");
			// error - wrong number of defined args
		}
	}
	(*id)->func.isDefined = true;
	
	symbol *newScope = *id;
	addScope(newScope);

	funcArgs.clear();
}

void addArg(string *name, DataType type)
{
	if (!checkFunc(name))
	{
		setError(ESEM, "Argument name already exists");
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
		setError(ESEM, "Variable not declared");
		return;
	}

	if ((*id)->dataType != expr->dataType)
	{
		setError(ESEM, "Expression invalid type");
		// error - wrong assignment type
	}

	// add new value from expr to id
	if ((*id)->dataType == DINT) {
		(*id)->var.ival = expr->var.ival;
	} else if ((*id)->dataType == DCHAR) {
	    (*id)->var.cval = expr->var.cval;
	} else if ((*id)->dataType == DSTRING) {
	    (*id)->var.sval = expr->var.sval;
	}

    AssignmentInst *i = new AssignmentInst();
    i->var = *id;
    i->result = expr;

    currScope->func.instructions.push_back(s);
}

/* Expression */
symbol* addTmpID(string *name)
{
	deque<symbol*>::iterator id = currScope->func.symbols.begin();

  	while (id!=currScope->func.symbols.end() && (*id)->name != *name)
  		++id;

	if (id == currScope->func.symbols.end())
	{
		setError(ESEM, "Variable not declared");
		symbol *sid = new symbol;
		return sid;
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

	LoadInst *i = new LoadInst();
	i->result = s;

	currScope->func.instructions.push_back(i);

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

	LoadInst *i = new LoadInst();
	i->result = s;

	currScope->func.instructions.push_back(i);

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

	LoadInst *i = new LoadInst();
	i->result = s;

	currScope->func.instructions.push_back(i);

	return s;
}

symbol* addExpr(symbol *expr1, symbol *expr2, OpType op)
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
				   	 	setError(ESEM, "Expression invalid type");
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
					 	setError(ESEM, "Expression invalid type");
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

	// create inst
	ExpressionInst *i = new ExpressionInst();
	i->var1 = expr1;
	i->var2 = expr2;
	i->result = s;
	i->op = op;

	currScope->func.instructions.push_back(s);

    return s;
}

symbol* addExprNeg(symbol *expr)
{
	if (expr->dataType != DINT)
	{
		setError(ESEM, "Expression invalid type");
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

	// create inst
	ExpressionInst *i = new ExpressionInst();
	i->var1 = expr;
	i->result = s;
	i->op = ONEG;

	currScope->func.instructions.push_back(s);

    return s;
}

symbol* addConvert(symbol *expr, DataType type)
{
	if (expr->dataType == type)
		return expr;

	if (!(expr->dataType == DCHAR && type == DSTRING) &&
	    !(expr->dataType == DCHAR && type == DINT) &&
	    !(expr->dataType == DINT && type == DCHAR))
	{
		setError(ESEM, "Expression unsupported conversion");
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

	// create inst
	CastInst *i = new CastInst();
	i->var = expr;
	i->result = s;
	i->type = type;

    return s;
}

symbol* addFuncCall(string *name, bool isExpr)
{
	deque<symbol*> *table;
	table = &scope.front()->func.symbols;

	deque<symbol*>::iterator id = table->begin();

  	while (id!=table->end() && (*id)->name != *name)
  		++id;

	if (id == table->end())
	{
		setError(ESEM, "Called function not defined");
		symbol *s = new symbol;
		return s;
	}

	if ((*id)->func.argTypes.size() == callArgs.size())	
	{ // func with arguments
		if ((*id)->func.argTypes.size() != 0)
		{// check arguments

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
					setError(ESEM, "Called function invalid arguments types");
					break;
				}
			}
		}
	}
	else
	{
		setError(ESEM, "Function arguments invalid count");
		// error - wrong number of defined args
	}

	symbol *s = new symbol;
	ostringstream tmpName;
	tmpName << "@CALL" << (*id)->name << tmpID++;
	s->name =  tmpName.str();
	s->type = STEMP;
	s->dataType = (*id)->dataType;
	//s.var.sval = sval;

	if (isExpr)
		currScope->func.symbols.push_back(s);

	symbol *sid = *id;

	CallInst *i = new CallInst();
    i->fce = sid;
    if (isExpr)
    	i->result = expr;
    else i->result = NULL;
  	i->args = callArgs;
  
    inst->push_back(i);  

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
		setError(ESEM, "Variable not declared");
		// error - not declared
	}

	callArgs.push_back(*id);
}

void addReturn(symbol *expr)
{
	DataType type;

	if (!expr)
		type = DVOID;
	else type = expr->dataType;


	if (type != currScope->dataType)
	{
		setError(ESEM, "Return invalid type");
		// error - wrong return type
	}

	ReturnInst *i = new ReturnInst();
    if (!expr)
    	i->result = NULL;
    else i->result = expr;
    currScope->func.instructions.push_back(i);
	
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
		setError(ESEM, "IF expression invalid type");
		// error - if expr
	}

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

	Label *middle = new Label("IF_FALSE_");
    Label *end   = new Label("IF_END_");
  
    JumpFalseInst *jumpif = new JumpFalseInst();
    jumpif->cond = expr;
    jumpif->label = middle;
  
    JumpInst *jump = new JumpInst(); 
    jump->label = end;

    currScope->func.instructions.push_back(jumpif);

    currJump.push_back(jump);
    currLabel.push_back(end);
    currLabel.push_back(middle);
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

		    currScope->func.instructions.push_back(currJump.back());
		    currScope->func.instructions.push_back(currLabel.back());

		    currLabel.pop_back();
		    currJump.pop_back();

			return;
		}
	}
}

void ElseEnd()
{
	currScope->func.instructions.push_back(currLabel.back());

	currLabel.pop_back();

	leaveScope();
}

void addWhile(symbol *expr)
{
	if (expr->dataType != DINT)
	{
		setError(ESEM, "WHILE expression invalid type");
		// error - if expr
	}

	symbol *s = new symbol;
	ostringstream tmpName;
	tmpName << "@WHILE" << tmpID++;
	s->name =  tmpName.str();
	s->type = SIF;
	addLocalVars(s);
	currScope->func.symbols.push_back(s);

	addScope(s);	

	Label *start = new Label("WHILE_");
    Label *end = new Label("WHILE_END_");
  
    JumpFalseInst *jumpif = new JumpFalseInst();
    jumpif->cond = expr;
    jumpif->label = end;
  
    JumpInst *jump = new JumpInst(); 
    jump->label = start;
  

    currScope->func.instructions.push_back(start);
    currScope->func.instructions.push_back(jumpif);	

    currJump.push_back(jump);
    currLabel.push_back(end);
}

void addWhileEnd()
{
	currScope->func.instructions.push_back(currJump.back());
   	currScope->func.instructions.push_back(currLabel.back());   

   	currJump.pop_back();
   	currLabel.pop_back();

   	leaveScope();
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