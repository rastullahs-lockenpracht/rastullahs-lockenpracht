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

