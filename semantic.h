#include <deque>
#include "table_symbols.h"
#include "error.h"
using namespace std;

extern symbol *currScope;						// current scope (with symbol table)
extern deque<DataType> funcArgTypes;					// arg types to function declaration
extern deque<symbol*> funcArgs;					// args to function definition
extern deque<symbol*> callArgs;					// args to function call
extern deque<symbol*> scope;					// nested scopes (global scope in front)
extern int tmpID;								// id number of temporary variable

void parseEnd(deque<symbol*> *table);

void printSTable(deque<symbol*> *table);
bool checkID(string *name);
bool checkFunc(string *name);

void addID(string *name);
void addTypes(DataType type);

void addFuncDecl(string *name, DataType type);
void addArgType(DataType type);
void addArg(string *name, DataType type);
void addFuncDefin(string *name, DataType type);

void addAssign(string *name, symbol *expr);

symbol* addTmpID(string *name);
symbol* addTmpInt(int ival);
symbol* addTmpChar(char cval);
symbol* addTmpString(string *sval);

symbol* addExpr(symbol *oper1, symbol *oper2, int op);
symbol* addExprNeg(symbol *oper);

symbol* addConvert(symbol *expr, DataType type);
symbol* addFuncCall(string *name);
void addCallArg(symbol *arg);

void addReturn(symbol *expr);

void addScope(symbol *newScope);
void leaveScope();

void addIf(symbol *expr);
void addElse();
void addWhile(symbol *expr);

void addLocalVars(symbol *s);
