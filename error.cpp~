#include <string>
#include <iostream>
#include "error.h"
using namespace std;

ErrorCode error = EOK;

void setError(ErrorCode ec, string msg)
{
	if (error == EOK)
		error = ec;
	cout << "ERROR:" << ec<< ":" << msg << "!" << endl;
}