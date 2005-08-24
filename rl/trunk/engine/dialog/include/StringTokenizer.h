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
