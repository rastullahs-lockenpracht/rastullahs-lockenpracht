#ifndef __RubyInterpreter_H__
#define __RubyInterpreter_H__

#include "Interpreter.h"
#include <map>

#include "OgreNoMemoryMacros.h"
#include "ruby.h"
#include "FixRubyHeaders.h"
#include "OgreMemoryMacros.h" 

#include "CorePrerequisites.h"

namespace rl {

typedef std::map <String, VALUE> Value_Map;
typedef std::pair <String, VALUE> Value_Pair;
typedef std::map <String, ScriptObject*> ScriptObjectPtr_Map;
typedef std::pair <String, ScriptObject*> ScriptObjectPtr_Pair;

typedef VALUE(*staticValueMethod)(...);

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

	VALUE* rubyArgs( int argc, const String args[] );
};

}
#endif

