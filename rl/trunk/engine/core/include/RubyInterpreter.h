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

#ifndef __RubyInterpreter_H__
#define __RubyInterpreter_H__

#include "Interpreter.h"
#include <map>

#include <OgreNoMemoryMacros.h>
#include "FixRubyHeaders.h"
#include <ruby.h>
#include "FixRubyHeaders.h"
#include <OgreMemoryMacros.h>

#include "CommonPrerequisites.h"
#include "CorePrerequisites.h"

namespace rl {

typedef std::map <Ogre::String, VALUE> Value_Map;
typedef std::pair <Ogre::String, VALUE> Value_Pair;
typedef std::map <Ogre::String, ScriptObject*> ScriptObjectPtr_Map;
typedef std::pair <Ogre::String, ScriptObject*> ScriptObjectPtr_Pair;

typedef VALUE(*staticValueMethod)(...);
typedef VALUE(*ProtectedMethod)(VALUE);
class ScriptObject;

class _RlCoreExport RubyInterpreter : public Interpreter
{
public:
	RubyInterpreter();
	virtual ~RubyInterpreter();

	void initializeInterpreter();
	void initializeInterpreter(staticValueMethod func);
	
	bool execute(Ogre::String command);

	static Ogre::String val2str(const VALUE rval);
	static CeGuiString val2ceguistr(const VALUE rval);
	static Ogre::String strval2str(const VALUE rval);

	void setScript( const Ogre::String& instname,
	    const Ogre::String& scriptname, const Ogre::String& classname,
	    int argc, const Ogre::String args[] = 0 );
	void callFunction( const Ogre::String& instname,
	    const Ogre::String& funcname,
	    int argc, const Ogre::String args[] = 0 );
    int callIntegerFunction( const Ogre::String& instname,
        const Ogre::String& funcname, int argc,
        const Ogre::String args[] = 0 );
	std::string callStringFunction( const Ogre::String& instname,
	    const Ogre::String& funcname, int argc,
	    const Ogre::String args[] = 0 );
	void registerScriptObject( ScriptObject* obj,
	    const Ogre::String& instname );
	void unregisterScriptObject( const Ogre::String& instname );

	void addSearchPath(const Ogre::String& path);

	void setDefOut( staticValueMethod func);

	ScriptObject* getScriptObject( const Ogre::String& name );
private:
    VALUE mRubyObjects;
	Value_Map mScriptInstances;
	ScriptObjectPtr_Map mScriptObjects;
	
	void createScriptInstance( const Ogre::String& instname );
	void removeScriptInstance( const Ogre::String& instname );
	void registerRubyObject(VALUE object);
    void unregisterRubyObject(VALUE object);

	void logRubyErrors(const std::string& intro, int errorcode);
	void loadProtected(ProtectedMethod func, VALUE args,
	    const std::string& msg, bool exitOnFail = false);
	static VALUE loadDlls(VALUE);
	static VALUE loadGlobals(VALUE val);

	VALUE* rubyArgs( int argc, const Ogre::String args[] );
};

}
#endif

