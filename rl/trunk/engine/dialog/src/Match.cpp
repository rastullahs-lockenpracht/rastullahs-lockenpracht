/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */


#include "Match.h"


//--	Match.cpp

namespace rl
{
	Match::Match() : node(NULL) { }

	Match::~Match() { node=NULL;}

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
