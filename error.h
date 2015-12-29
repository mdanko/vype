#include <string>
using namespace std;

enum ErrorCode 
{ 
	EOK, ELEX, ESYN, ESEM, EGEN, EINT
};

extern ErrorCode error;

void setError(ErrorCode ec, string msg);