#ifndef __Rl_DialogGraphMaster_H__
#define __Rl_DialogGraphMaster_H__

//--	Graphmaster.h
#include "Match.h"
#include "DialogPrerequisites.h"

#include <string>



using namespace std;


namespace rl
{
	class Nodemaster;

	class _RlDialogExport Graphmaster
	{
	public:
		Graphmaster();
		~Graphmaster();
	
		Nodemaster *add(const string &context, const string &pattern, const string &that, const string &topic,const string &templateValue);
		Match *match(const string &context, const string &pattern, const string &that, const string &topic);
	private:
		Match *match(Nodemaster *node, Nodemaster *parent, component which, const string &input, const string &star, const string &path);
	
		Nodemaster *root;
	};
}
#endif
