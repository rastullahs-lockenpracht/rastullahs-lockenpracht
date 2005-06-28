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
