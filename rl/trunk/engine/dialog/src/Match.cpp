#include "Match.h"

//--	Match.cpp

namespace rl
{
	Match::Match() : node(NULL) { }

	Match::~Match() { }

	//--	substr(1) accounts for leading space
	void Match::setPattern(const string &pattern, component which) 
	{
		patterns[which] = pattern.substr(1);
	}

	string Match::getPattern(component which) 
	{
		return patterns[which];
	}

	void Match::addStar(const string &star, component which) 
	{
		//--	values added in reverse order due to Graphmaster design
		stars[which].push_back(star);
	}

	string Match::getStar(unsigned int index, component which) 
	{
		//--	only need to check index > size (due to unsigned int)
		vector<string> &vs = stars[which];
		unsigned int size = vs.size();
		//--	++index accounts for default first item (why is it there?)
		if ( ++index > size ) {
			//--	throw an exception?
			return "";
		}
		//--	account for reversed order (see addStar)
		return vs[size - index];
	}

	void Match::setNode(Nodemaster *node) 
	{
		this->node = node;
	}

	Nodemaster *Match::getNode() 
	{
		return node;
	}

	string Match::getPath() 
	{
		const static string separator = " : ";
		return patterns[Context] + separator
			+  patterns[Pattern] + separator
			+  patterns[That] + separator
			+  patterns[Topic];
	}
}
//--	end-of-file
