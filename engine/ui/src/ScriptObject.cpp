#include "ScriptObject.h"
#include "CeConsole.h"
#include "Interpreter.h"
namespace rl {

ScriptObject::ScriptObject( const String& name ) 
{	
    mName = name;
    CeConsole::getSingleton().getInterpreter()->registerScriptObject( this, mName );
}

ScriptObject::~ScriptObject()
{
    CeConsole::getSingleton().getInterpreter()->unregisterScriptObject( mName );
}

void ScriptObject::setScript( const String& scriptname, const String& classname, int argc, const String args[] )
{
	CeConsole::getSingleton().getInterpreter()->setScript( mName, scriptname, classname, argc, args );
}

void ScriptObject::callFunction( const String& funcname, int argc, const String args[] )
{
	CeConsole::getSingleton().getInterpreter()->callFunction( mName, funcname, argc, args );
}

int ScriptObject::callIntegerFunction( const String& funcname, int argc, const String args[] )
{
	return CeConsole::getSingleton().getInterpreter()->callIntegerFunction( mName, funcname, argc, args );
}

}
