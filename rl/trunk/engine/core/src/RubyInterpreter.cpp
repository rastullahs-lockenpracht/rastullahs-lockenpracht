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

#include "FixRubyHeaders.h"

#include "RubyInterpreter.h"
#include "ScriptObject.h"
#include "CoreSubsystem.h"
#include "ConfigurationManager.h"

using namespace Ogre;

namespace rl {

RubyInterpreter::RubyInterpreter() : mScriptObjects(), mScriptInstances()
{
    
}

ScriptObject* RubyInterpreter::getScriptObject( const String& instname )
{
	ScriptObjectPtr_Map::const_iterator pSoIter = mScriptObjects.find(instname);

	if( pSoIter != mScriptObjects.end() )
			return pSoIter->second;

	return 0;
}
void RubyInterpreter::initializeInterpreter()
{
	/* 
		Standard-Initialisierung, derzeit ohne Funktion da zur Korrekten Ausgabe in der Console
		eine static method übergeben werden muss, die die Ruby-Standardausgabe handelt
	*/
}

void RubyInterpreter::initializeInterpreter(staticValueMethod func)
{
	//Ruby Initialisieren
	ruby_init();
	
	//Skript-Verzeichnisse der  Dateien duerfen auch in /script liegen
	StringVector modules = CoreSubsystem::getSingleton().getCommonModules();
	modules.push_back(CoreSubsystem::getSingleton().getActiveModule());
	for (StringVector::iterator iter = modules.begin(); iter != modules.end(); iter++)
	{
		//wir suchen die Scripte im modules Verzeichnis relativ zum ModuleRootPath!
		addSearchPath(ConfigurationManager::getSingleton().
			getModulesRootDirectory() + "/modules/" + (*iter) + "/scripts");
		addSearchPath(ConfigurationManager::getSingleton().
			getModulesRootDirectory() + "/modules/" + (*iter) + 
			"/scripts/maps");
	}
	
	ruby_init_loadpath();
	//Skriptname
	ruby_script("Rastullah");
	// Fuer Ruby .dll oder .so dazu laden
	
	loadProtected(&RubyInterpreter::loadDlls, 0, "Ruby error while loading dlls");

	
	//Ersetzt die Standard-Ausgabe von Ruby durch Ausgaben in die Console
	rb_defout = rb_str_new("", 0);
	// Eigentlich nicht mehr notwendig, aber ohne das gibts nen Absturz?!?!
//	rb_define_singleton_method(rb_defout, "write", (VALUE(*)(...))console_write, 1);
	rb_define_singleton_method(rb_defout, "write", func, 1);

    //Define Globals
	loadProtected(&RubyInterpreter::loadGlobals, 0, "Ruby error while loading globals.rb");

	//to Prevent the Ruby GC from deleting
	mRubyObjects = rb_ary_new();
	rb_gc_register_address(&mRubyObjects);
}

void RubyInterpreter::addSearchPath(const String& path)
{
	ruby_incpush(path.c_str());
}

VALUE RubyInterpreter::loadDlls(VALUE val)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	rb_require("RlScript");
#else
    rb_require("libRlScript");
#endif

	return Qnil;
}

VALUE RubyInterpreter::loadGlobals(VALUE val)
{
	rb_require("globals.rb");

	return Qnil;
}

void RubyInterpreter::loadProtected(ProtectedMethod func, VALUE val, const std::string& msg, bool exitOnFail)
{
	int error = 0;
	rb_protect(func, val, &error);
	logRubyErrors("Ruby error while initializing", error);
}

void RubyInterpreter::logRubyErrors(const std::string& intro, int errorcode)
{
	if(errorcode != 0) 
	{
		VALUE info = rb_inspect(ruby_errinfo);
		rb_backtrace();
		if (intro.length() > 0)
			CoreSubsystem::getSingleton().log(intro);
		CoreSubsystem::getSingleton().log(STR2CSTR(info));
	}
}

void RubyInterpreter::setDefOut(staticValueMethod func)
{
	rb_define_singleton_method(rb_defout, "write",func, 1);
}

RubyInterpreter::~RubyInterpreter()
{
	rb_gc_unregister_address(&mRubyObjects);
	ruby_finalize();
}

bool RubyInterpreter::execute(String command)
{
	int status = -1;

	CoreSubsystem::getSingleton().log( "RubyInterpreter: (execute) "+ command );
	rb_eval_string_protect(command.c_str(), &status);

	logRubyErrors("", status);

    if( status )
        rb_eval_string_protect("print $!", &status);

	return false;
}

String RubyInterpreter::val2str(const VALUE rval){
  return STR2CSTR(rb_funcall(rval, rb_intern("to_s"), 0));
}

String RubyInterpreter::strval2str(const VALUE rval){
  return String(RSTRING(rval)->ptr);
}

void RubyInterpreter::registerScriptObject( ScriptObject* obj, const String& instname )
{
	mScriptObjects.insert( ScriptObjectPtr_Pair(instname,obj) );
	createScriptInstance(instname);
}

void RubyInterpreter::unregisterScriptObject( const String& instname )
{
	removeScriptInstance(instname);
	mScriptObjects.erase( instname );
}

void RubyInterpreter::registerRubyObject(VALUE pObject) 
{
	rb_ary_push(mRubyObjects, pObject);
}

void RubyInterpreter::unregisterRubyObject(VALUE pObject) 
{
	rb_ary_delete(mRubyObjects, pObject);
}

// Wrapper
VALUE rb_const_get_wrapper(VALUE data)
{
	VALUE *args = (VALUE *) data;
	return rb_const_get(args[0], args[1]);
}

VALUE rb_funcall_wrapper(VALUE data)
{
	VALUE *args=(VALUE *) data;
	const VALUE argsT=args[3];
	return rb_funcall2(args[0], rb_intern(StringValuePtr(args[1])), (int)NUM2INT(args[2]), &argsT);
}

void RubyInterpreter::setScript( const String& instname, const String& scriptname, const String& classname, int argc, const String args[] )
{
	// unregister old script
	removeScriptInstance(instname);
	createScriptInstance(instname);

	// Includes the file with the class definition
	rb_require( scriptname.c_str() );
	// Converts the String args into ruby VALUES
	VALUE *rArgs = rubyArgs( argc, args );
	// This is need to wrap rb_get_const with rb_protect
	VALUE cgargs[4];
    
    int error = 0;
    cgargs[0] = rb_cObject;
    cgargs[1] = rb_intern(classname.c_str());
    VALUE pClass = rb_protect( (VALUE(*)(VALUE) )rb_const_get_wrapper , (VALUE) &cgargs[0], &error);
	logRubyErrors("RubyInterpreter::setScript", error);

	cgargs[0] = pClass;
	cgargs[1] = rb_str_new2("new");
	cgargs[2] = rb_int_new(argc);
	cgargs[3] = rArgs[0];
	// Generates a new instance of our class
	VALUE pScriptInstance = rb_protect((VALUE(*)(VALUE) )rb_funcall_wrapper,(VALUE)&cgargs[0],&error );
	logRubyErrors("RubyInterpreter::setScript", error);
	// Registers it with the ruby GC, so it won't be deleted
	registerRubyObject( pScriptInstance );
	// Insert it in our map
	mScriptInstances[ instname ] = pScriptInstance;
	// delete args
	delete[] rArgs;
}
	
void RubyInterpreter::callFunction( const String& instname, const String& funcname, int argc, const String args[] )
{
	Value_Map::const_iterator pSoIter = mScriptInstances.find(instname);

	if( pSoIter != mScriptInstances.end() )
	{
		// Converts the String args into ruby VALUES
		VALUE *rArgs = rubyArgs( argc, args );
		int error=0;
		VALUE cgargs[4];
		cgargs[0] = pSoIter->second;
		cgargs[1] = rb_str_new2( funcname.c_str() );
		cgargs[2] = rb_int_new(argc);
		cgargs[3] = rArgs[0];
		// Calls the Function of our script instance
		rb_protect( ( VALUE(*)(VALUE) )rb_funcall_wrapper,(VALUE)&cgargs[0],&error );
		logRubyErrors("RubyInterpreter::callFunction", error);
		// delete args
		delete[] rArgs;
	}
}

int RubyInterpreter::callIntegerFunction( const String& instname, const String& funcname, int argc, const String args[] )
{
	Value_Map::const_iterator pSoIter = mScriptInstances.find(instname);

    int iReturn = 0;

	if( pSoIter != mScriptInstances.end() )
	{
		// Converts the String args into ruby VALUES
		VALUE *rArgs = rubyArgs( argc, args );
		int error=0;
		VALUE cgargs[4];
		cgargs[0] = pSoIter->second;
		cgargs[1] = rb_str_new2( funcname.c_str() );
		cgargs[2] = rb_int_new(argc);
		cgargs[3] = rArgs[0];
		// Calls the Function of our script instance
		VALUE rReturn = rb_protect( ( VALUE(*)(VALUE) )rb_funcall_wrapper,(VALUE)&cgargs[0],&error );
		logRubyErrors("RubyInterpreter::callIntegerFunction", error);

        iReturn = NUM2INT(rReturn);
		// delete args
		delete[] rArgs;
	}

    return iReturn;
}

std::string RubyInterpreter::callStringFunction( const String& instname, const String& funcname, int argc, const String args[] )
{
	Value_Map::const_iterator pSoIter = mScriptInstances.find(instname);

	std::string sReturn = "";

	if( pSoIter != mScriptInstances.end() )
	{
		// Converts the String args into ruby VALUES
		VALUE *rArgs = rubyArgs( argc, args );
		int error=0;
		VALUE cgargs[4];
		cgargs[0] = pSoIter->second;
		cgargs[1] = rb_str_new2( funcname.c_str() );
		cgargs[2] = rb_int_new(argc);
		cgargs[3] = rArgs[0];
	
		// Calls the Function of our script instance
		VALUE rReturn = rb_protect( ( VALUE(*)(VALUE) )rb_funcall_wrapper,(VALUE)&cgargs[0],&error );
		logRubyErrors("RubyInterpreter::setStringFunction", error);
	//	VALUE rReturn = rb_funcall2(pSoIter->second, rb_intern( funcname.c_str() ), argc, rArgs);

        sReturn = STR2CSTR(rReturn);
		// delete args
		delete[] rArgs;
	}
    return sReturn;	
}

VALUE* RubyInterpreter::rubyArgs( int argc, const String args[] )
{
	VALUE *rArgs = new VALUE[argc];

	// Each argument is inserted as a simple rb_str
	// Conversion of arguments in scripts
	if( args != 0 )
		for( int i = 0; i < argc ; i++ )
			rArgs[i] = rb_str_new2(args[i].c_str());

	return rArgs;
}


void RubyInterpreter::createScriptInstance( const String& instname )
{
	VALUE pScriptInstance = 0;
	mScriptInstances.insert( Value_Pair(instname,pScriptInstance) );
}

void RubyInterpreter::removeScriptInstance( const String& instname )
{
	Value_Map::iterator pSoIter = mScriptInstances.find(instname);

	if( pSoIter != mScriptInstances.end() )
	{
		unregisterRubyObject( mScriptInstances[instname] );
		mScriptInstances.erase( pSoIter );
	}
}

}
