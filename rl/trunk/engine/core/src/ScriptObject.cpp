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

#include "ScriptObject.h"
#include "CoreSubsystem.h"
#include "Interpreter.h"
namespace rl {

ScriptObject::ScriptObject( const String& name ) 
{	
    mName = name;
	CoreSubsystem::getSingleton().getInterpreter()->registerScriptObject( this, mName );
 //   Console::getSingleton().getInterpreter()->registerScriptObject( this, mName );
}

ScriptObject::~ScriptObject()
{
	CoreSubsystem::getSingleton().getInterpreter()->unregisterScriptObject( mName );
 //   Console::getSingleton().getInterpreter()->unregisterScriptObject( mName );
}

void ScriptObject::setScript( const String& scriptname, const String& classname, int argc, const String args[] )
{
	CoreSubsystem::getSingleton().getInterpreter()->setScript( mName, scriptname, classname, argc, args );
//	Console::getSingleton().getInterpreter()->setScript( mName, scriptname, classname, argc, args );
}

void ScriptObject::callFunction( const String& funcname, int argc, const String args[] )
{
	CoreSubsystem::getSingleton().getInterpreter()->callFunction( mName, funcname, argc, args );
//	Console::getSingleton().getInterpreter()->callFunction( mName, funcname, argc, args );
}

int ScriptObject::callIntegerFunction( const String& funcname, int argc, const String args[] )
{
	return CoreSubsystem::getSingleton().getInterpreter()->callIntegerFunction( mName, funcname, argc, args );
}

}
