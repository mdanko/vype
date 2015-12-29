#include <vector>
#include <deque>
#include <string>
using namespace std;

enum OpType
{
	OPLUS, OMINUS, OMUL, ODIV, OMOD, OAND, OOR, OEQ, ONEQ, OLT, OLTE, OGT, OGTE, ONEG
};

enum DataType 
{
	INIT, DVOID, DINT, DCHAR, DSTRING
};

enum SymType 
{
	SID, SFUNC, STEMP, SIF, SELSE, SWHILE, SRETURN
};

typedef struct symbol symbol;

struct func_info
{
	// var_info retval;  		default vals ... ival = 0, cval = '\0', sval = ""
	bool isDefined;
	deque<int> argTypes;
	deque<symbol*> symbols;
};

struct var_info
{
	int ival;
	char cval;
	string sval;
	bool isGlobal;
};


struct symbol
{
	string name;
    int type;		// ID/FUNC
    int dataType;	// int/char/string (id type/func return type)
    var_info var;
    func_info func;
};
