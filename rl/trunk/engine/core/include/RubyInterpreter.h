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

#ifndef __RubyInterpreter_H__
#define __RubyInterpreter_H__

#include "Interpreter.h"
#include <map>

#include <OgreNoMemoryMacros.h>

#include "FixRubyHeaders.h"
#include <ruby.h>
#include "FixRubyHeaders.h"
#include <OgreMemoryMacros.h>

#include "CorePrerequisites.h"

namespace rl {

typedef std::map <String, VALUE> Value_Map;
typedef std::pair <String, VALUE> Value_Pair;
typedef std::map <String, ScriptObject*> ScriptObjectPtr_Map;
typedef std::pair <String, ScriptObject*> ScriptObjectPtr_Pair;

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
	
	bool execute(String command);

	static String val2str(const VALUE rval);
	static String strval2str(const VALUE rval);

	void setScript( const String& instname, const String& scriptname, const String& classname, int argc, const String args[] = 0 );
	void callFunction( const String& instname, const String& funcname, int argc, const String args[] = 0 );
    int callIntegerFunction( const String& instname, const String& funcname, int argc, const String args[] = 0 );
	std::string callStringFunction( const String& instname, const String& funcname, int argc, const String args[] = 0 );
	void registerScriptObject( ScriptObject* obj, const String& instname );
	void unregisterScriptObject( const String& instname );

	void addSearchPath(const String& path);

	void setDefOut( staticValueMethod func);

	ScriptObject* getScriptObject( const String& name );
private:
    VALUE mRubyObjects;
	Value_Map mScriptInstances;
	ScriptObjectPtr_Map mScriptObjects;
	
	void createScriptInstance( const String& instname );
	void removeScriptInstance( const String& instname );
	void registerRubyObject(VALUE object);
    void unregisterRubyObject(VALUE object);

	void logRubyErrors(const std::string& intro, int errorcode);
	void loadProtected(ProtectedMethod func, VALUE args, const std::string& msg, bool exitOnFail = false);
	static VALUE loadDlls(VALUE);
	static VALUE loadGlobals(VALUE val);

	VALUE* rubyArgs( int argc, const String args[] );
};

}
#endif

