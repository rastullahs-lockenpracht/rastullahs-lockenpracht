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
	
		void setPattern(const CeGuiString &pattern, component which);
		CeGuiString getPattern(component which);
	
		void addStar(const CeGuiString &star, component which);
		CeGuiString getStar(unsigned int index, component which);
	
		void setNode(Nodemaster *node);
		Nodemaster *getNode();
	
		CeGuiString getPath();
	private:
		//--	size derived from enum 'component'
		CeGuiString patterns[4];
		vector<CeGuiString> stars[4];
		Nodemaster *node;
	};
}
#endif
