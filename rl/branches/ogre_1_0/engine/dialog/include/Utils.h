/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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
