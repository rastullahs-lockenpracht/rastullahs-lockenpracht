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

#ifndef __PythonInterpreter_H__
#define __PythonInterpreter_H__

#include "Interpreter.h"
#include <map>

#include <OgreNoMemoryMacros.h>
#include <boost/python.hpp>
#include <OgreMemoryMacros.h>

#include "CorePrerequisites.h"

namespace rl {

    typedef std::map <Ogre::String, ScriptObject*> ScriptObjectPtr_Map;
    typedef std::pair <Ogre::String, ScriptObject*> ScriptObjectPtr_Pair;

    class ScriptObject;

    class _RlCoreExport PythonInterpreter : public Interpreter
    {
    public:
        PythonInterpreter();
        virtual ~PythonInterpreter();

        void initializeInterpreter();

        bool execute(Ogre::String command);

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

        ScriptObject* getScriptObject( const Ogre::String& name );
    private:
        /// Kontext für lokale/globale Variablen in dem alle Skripte
        /// ausgeführt werden sollen.
        boost::python::object mMainNamespace;
        boost::python::object mMainModule;
        boost::python::object mRastullahModule;
        
        ScriptObjectPtr_Map mScriptObjects;
    };

}
#endif

