#ifndef __Rl_DialogUtils_H__
#define __Rl_DialogUtils_H__

//--	Utils.h
#include "DialogPrerequisites.h"

/* Some helper functions, don't know where they should belong to */

#include <string>

using namespace std;
namespace rl
{
	string toUpper(const string &str);
	string toLower(const string &str);
	string trim(const string &str);
	bool checkHeadWS(const string &);
	bool checkTailWS(const string &);
	string normalise(const string &);
}
#endif
