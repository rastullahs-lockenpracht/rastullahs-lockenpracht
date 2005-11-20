/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */


#include "Match.h"


//--	Match.cpp

namespace rl
{
	Match::Match() : node(NULL) { }

	Match::~Match() { node=NULL;}

	//--	substr(1) accounts for leading space
	void Match::setPattern(const CeGuiString &pattern, component which) 
	{
		patterns[which] = pattern.substr(1);
	}

	CeGuiString Match::getPattern(component which) 
	{
		return patterns[which];
	}

	void Match::addStar(const CeGuiString &star, component which) 
	{
		//--	values added in reverse order due to Graphmaster design
		stars[which].push_back(star);
	}

	CeGuiString Match::getStar(unsigned int index, component which) 
	{
		//--	only need to check index > size (due to unsigned int)
		vector<CeGuiString> &vs = stars[which];
		size_t size = vs.size();
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

	CeGuiString Match::getPath() 
	{
		const static string separator = " : ";
		return patterns[Context] + separator
			+  patterns[Pattern] + separator
			+  patterns[That] + separator
			+  patterns[Topic];
	}
}
//--	end-of-file
