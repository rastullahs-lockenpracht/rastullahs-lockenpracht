#ifndef __Rl_DialogStringTokenizer_H__
#define __Rl_DialogStringTokenizer_H__

//--	StringTokenizer.h
#include "DialogPrerequisites.h"

/* Used for Graphmaster, unchanged from original jAlice Source*/

#include <string>
#include <sstream>

using namespace std;

namespace rl
{
	class _RlDialogExport StringTokenizer
	{
	public:
		StringTokenizer(const string &data, const char *delimiter): str(data.c_str()), delim(delimiter), opt(false) { }
		~StringTokenizer() { }
	
		bool hasMoreTokens();
		string nextToken();
		string getString();
	private:
		const char *str;
		const char *delim;
		bool opt;
	};
}
#endif
