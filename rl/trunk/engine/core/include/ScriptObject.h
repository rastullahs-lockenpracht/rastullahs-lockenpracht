#ifndef __ScriptObject_H__
#define __ScriptObject_H__

#include "CorePrerequisites.h"

namespace rl {

/** The Script Object
	@brief A simple sripting class for script-callbacks
	@remarks The instance names must be unique, 
	@par The name of the ScriptObject is stored in Interpreter, where also the plattform
		 specific script-functionality resides
	@see Interpreter
*/
class _RlCoreExport ScriptObject
{
public:
	/** Creates a new ScriptObject, registers with Interpreter
		@param name The Name of this ScriptObject, also the reference to the Instance in Interpreter
	*/
	ScriptObject::ScriptObject( const String& name );
	/** Default Destructor, unregisters with Interpreter */
	ScriptObject::~ScriptObject( );

	/** Sets the script-instance for this ScriptObject
		@param scriptname The File in which the class is defined
		@param classname The Class of the Instance
		@param argc The number of parameters
		@param args The parameters of the constructor, supports only String
	*/
	void setScript( const String& scriptname, const String& classname, int argc, const String args[] = 0 );

	/** Calls a Function of the script-instance of this Object
		@param funcname The Name of the Funktion
		@param argc The number of parameters
		@param args The parameters of the function, supports only String
	*/
	void callFunction( const String& funcname, int argc, const String args[] = 0 ); 

    /** Calls a Function of the script-instance of this Object
		@param funcname The Name of the Funktion
		@param argc The number of parameters
		@param args The parameters of the function, supports only String
        @return The VALUE Ruby returns, parsed to an Integer, 0 if no Integer was returned
	*/
    int callIntegerFunction( const String& funcname, int argc, const String args[] = 0 ); 

private:
	/** The Name */
	String mName;
};

}
#endif
