#include <deque>
#include "table_symbols.h"
using namespace std;

extern deque<symbol*> *sTable;					// current scope symbol table
extern deque<int> funcArgTypes;					// arg types to function declaration
extern deque<symbol*> funcArgs;					// args to function definition
extern deque<symbol*> callArgs;					// args to function call
extern deque<symbol*> scope;					// nested scopes (global scope in front)
extern int tmpID;								// id number of temporary variable

void printSTable(deque<symbol*> *table);

void addID(string *name);
void addTypes(int type);

void addFuncDecl(string *name, int type);
void addArgType(int type);
void addArg(string *name, int type);
void addFuncDefin(string *name, int type);

void addAssign(string *name, symbol *expr);

symbol* addTmpID(string *name);
symbol* addTmpInt(int ival);
symbol* addTmpChar(char cval);
symbol* addTmpString(string *sval);

symbol* addExpr(symbol *oper1, symbol *oper2, int op);
symbol* addExprNeg(symbol *oper);

symbol* addConvert(symbol *expr, int type);
symbol* addFuncCall(string *name);
void addCallArg(symbol *arg);

void addScope(symbol *newScope);
void leaveScope();
