#ifndef __Rl_DialogMatch_H__
#define __Rl_DialogMatch_H__

#include "DialogPrerequisites.h"
//--	Match.h

#include <string>
#include <vector>

using namespace std;

namespace rl
{
	typedef enum { Context = 0, Pattern, That, Topic } component;

	class Nodemaster;

	class _RlDialogExport Match
	{
	public:
		Match();
		~Match();
	
		void setPattern(const string &pattern, component which);
		string getPattern(component which);
	
		void addStar(const string &star, component which);
		string getStar(unsigned int index, component which);
	
		void setNode(Nodemaster *node);
		Nodemaster *getNode();
	
		string getPath();
	private:
		//--	size derived from enum 'component'
		string patterns[4];
		vector<string> stars[4];
		Nodemaster *node;
	};
}
#endif
