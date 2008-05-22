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
	ScriptObject::ScriptObject( const Ogre::String& name );
	/** Default Destructor, unregisters with Interpreter */
	ScriptObject::~ScriptObject( );

	/** Sets the script-instance for this ScriptObject
		@param scriptname The File in which the class is defined
		@param classname The Class of the Instance
		@param argc The number of parameters
		@param args The parameters of the constructor, supports only String
	*/
	void setScript( const Ogre::String& scriptname,
	    const Ogre::String& classname, int argc,
	    const CeGuiString args[] = 0 );

	/** Calls a Function of the script-instance of this Object
		@param funcname The Name of the Funktion
		@param argc The number of parameters
		@param args The parameters of the function, supports only String
	*/
	void callFunction( const Ogre::String& funcname, int argc,
	    const CeGuiString args[] = 0 ); 

    /** Calls a Function of the script-instance of this Object
		@param funcname The Name of the Funktion
		@param argc The number of parameters
		@param args The parameters of the function, supports only String
        @return The VALUE Ruby returns, parsed to an Integer, 0 if no Integer was returned
	*/
    int callIntegerFunction( const Ogre::String& funcname,
        int argc, const CeGuiString args[] = 0 ); 

private:
	/** The Name */
	Ogre::String mName;
};

}
#endif
