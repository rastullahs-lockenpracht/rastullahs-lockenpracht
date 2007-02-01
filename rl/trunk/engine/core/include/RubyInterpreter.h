/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __RubyInterpreter_H__
#define __RubyInterpreter_H__

#include "CorePrerequisites.h"

typedef unsigned long VALUE;

namespace rl {

typedef VALUE(*staticValueMethod)(...);
typedef VALUE(*ProtectedMethod)(VALUE);

class _RlCoreExport RubyInterpreter
{
public:
	RubyInterpreter();
	virtual ~RubyInterpreter();

	void initializeInterpreter();
    void finalizeInterpreter();
	void setOutputFunction(staticValueMethod func);

	bool execute(const Ogre::String& command);
	bool executeFile(Ogre::String rubyfile);

	void addSearchPath(const Ogre::String& path);

	static CeGuiString val2ceguistr(const VALUE rval);
private:	
	void logRubyErrors(const std::string& intro, int errorcode);
	void loadProtected(ProtectedMethod func, VALUE args,
	    const std::string& msg, bool exitOnFail = false);
	static VALUE loadDlls(VALUE);
};

}
#endif
