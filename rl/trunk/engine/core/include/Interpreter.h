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

#ifndef __Interpreter_H__
#define __Interpreter_H__

#include <OgreSingleton.h>

#include "CorePrerequisites.h"

namespace rl {

class ScriptObject;

class _RlCoreExport Interpreter 
{
	public:
		virtual ~Interpreter() {};

		virtual bool execute(Ogre::String command) = 0;
		virtual void initializeInterpreter() = 0;

		virtual void setScript( const String& instname, const String& scriptname, const String& classname, int argc, const String args[] = 0 ) = 0;
		virtual void callFunction( const String& instname, const String& funcname, int argc, const String args[] = 0 ) = 0;
		virtual int callIntegerFunction( const String& instname, const String& funcname, int argc, const String args[] = 0 ) = 0;

		virtual ScriptObject* getScriptObject( const String& name ) = 0;

		virtual void registerScriptObject( ScriptObject* obj, const String& instname ) = 0;
		virtual void unregisterScriptObject( const String& instname ) = 0;
};

}
#endif

