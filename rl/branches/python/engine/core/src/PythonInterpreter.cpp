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

#include "PythonInterpreter.h"
#include "ScriptObject.h"
#include "CoreSubsystem.h"

using namespace Ogre;
using namespace boost::python;

namespace rl {

    PythonInterpreter::PythonInterpreter() : mScriptObjects()
    {
        Py_Initialize();
        
        ///XXX Zuweisung ok so?
        mMainModule = object(handle<>(borrowed(PyImport_AddModule("__main__"))));
        mMainNamespace = mMainModule.attr("__dict__");
    }

    PythonInterpreter::~PythonInterpreter()
    {
        Py_Finalize();
    }
    
    ScriptObject* PythonInterpreter::getScriptObject( const String& instname )
    {
        ScriptObjectPtr_Map::const_iterator pSoIter = mScriptObjects.find(instname);

        if( pSoIter != mScriptObjects.end() )
            return pSoIter->second;

        return 0;
    }
    
    void PythonInterpreter::initializeInterpreter()
    {
        //handle<>ignored_import(borrowed(PyImport_ImportModule("sys")));
        execute("import sys\n");
        
        addSearchPath(".");
        //Skript-Verzeichnisse der  Dateien duerfen auch in /script liegen
        StringVector modules = CoreSubsystem::getSingleton().getActiveModules();
        for (StringVector::iterator iter = modules.begin(); iter != modules.end(); iter++)
        {
            addSearchPath("modules/"+(*iter)+"/scripts");
            addSearchPath("modules/"+(*iter)+"/scripts/maps");
        }

//        mRastullahModule(borrowed(PyImport_ImportModule("RlScript")));
        mRastullahModule(borrowed(PyImport_AddModule("rastullah")));

        //Ersetzt die Standard-Ausgabe von Python durch Ausgaben in die Console
        ///@todo Standardausgabe ersetzen.

        //Define Globals
        //handle<>ignored(borrowed(PyImport_ImportModule("globals.py")));
    }

    void PythonInterpreter::addSearchPath(const String& path)
    {
        execute(String("sys.path.append(\"") + path + String("\")"));
    }

    bool PythonInterpreter::execute(String command)
    {
        try
        {
            object result(handle<>(PyRun_String(command.c_str(),
                Py_file_input, mMainModule.ptr(), mMainModule.ptr())));
            return true;
        }
        catch(error_already_set)
        {
            PyErr_Print();
            fflush(stderr);
            return false;
        }
    }

    void PythonInterpreter::registerScriptObject(ScriptObject* obj, const String& instname)
    {
        mScriptObjects.insert(ScriptObjectPtr_Pair(instname,obj));
        //createScriptInstance(instname);
    }

    void PythonInterpreter::unregisterScriptObject(const String& instname)
    {
        //removeScriptInstance(instname);
        mScriptObjects.erase( instname );
    }

    void PythonInterpreter::setScript(const String& instname,
        const String& scriptname, const String& classname,
        int argc, const String args[])
    {
        //// unregister old script
        //removeScriptInstance(instname);
        //createScriptInstance(instname);

        //// Includes the file with the class definition
        //rb_require( scriptname.c_str() );
        //// Converts the String args into ruby VALUES
        //VALUE *rArgs = rubyArgs( argc, args );
        //// This is need to wrap rb_get_const with rb_protect
        //VALUE cgargs[4];

        //int error = 0;
        //cgargs[0] = rb_cObject;
        //cgargs[1] = rb_intern(classname.c_str());
        //VALUE pClass = rb_protect( (VALUE(*)(VALUE) )rb_const_get_wrapper , (VALUE) &cgargs[0], &error);
        //logPythonErrors("PythonInterpreter::setScript", error);

        //cgargs[0] = pClass;
        //cgargs[1] = rb_str_new2("new");
        //cgargs[2] = rb_int_new(argc);
        //cgargs[3] = rArgs[0];
        //// Generates a new instance of our class
        //VALUE pScriptInstance = rb_protect((VALUE(*)(VALUE) )rb_funcall_wrapper,(VALUE)&cgargs[0],&error );
        //logPythonErrors("PythonInterpreter::setScript", error);
        //// Registers it with the ruby GC, so it won't be deleted
        //registerPythonObject( pScriptInstance );
        //// Insert it in our map
        //mScriptInstances[ instname ] = pScriptInstance;
        //// delete args
        //delete[] rArgs;
    }

    void PythonInterpreter::callFunction( const String& instname, const String& funcname, int argc, const String args[] )
    {
        //Value_Map::const_iterator pSoIter = mScriptInstances.find(instname);

        //if( pSoIter != mScriptInstances.end() )
        //{
        //    // Converts the String args into ruby VALUES
        //    VALUE *rArgs = rubyArgs( argc, args );
        //    int error=0;
        //    VALUE cgargs[4];
        //    cgargs[0] = pSoIter->second;
        //    cgargs[1] = rb_str_new2( funcname.c_str() );
        //    cgargs[2] = rb_int_new(argc);
        //    cgargs[3] = rArgs[0];
        //    // Calls the Function of our script instance
        //    rb_protect( ( VALUE(*)(VALUE) )rb_funcall_wrapper,(VALUE)&cgargs[0],&error );
        //    logPythonErrors("PythonInterpreter::callFunction", error);
        //    // delete args
        //    delete[] rArgs;
        //}
    }

    int PythonInterpreter::callIntegerFunction( const String& instname, const String& funcname, int argc, const String args[] )
    {
        //Value_Map::const_iterator pSoIter = mScriptInstances.find(instname);

        int iReturn = 0;

        //if( pSoIter != mScriptInstances.end() )
        //{
        //    // Converts the String args into ruby VALUES
        //    VALUE *rArgs = rubyArgs( argc, args );
        //    int error=0;
        //    VALUE cgargs[4];
        //    cgargs[0] = pSoIter->second;
        //    cgargs[1] = rb_str_new2( funcname.c_str() );
        //    cgargs[2] = rb_int_new(argc);
        //    cgargs[3] = rArgs[0];
        //    // Calls the Function of our script instance
        //    VALUE rReturn = rb_protect( ( VALUE(*)(VALUE) )rb_funcall_wrapper,(VALUE)&cgargs[0],&error );
        //    logPythonErrors("PythonInterpreter::callIntegerFunction", error);

        //    iReturn = NUM2INT(rReturn);
        //    // delete args
        //    delete[] rArgs;
        //}

        return iReturn;
    }

    std::string PythonInterpreter::callStringFunction( const String& instname, const String& funcname, int argc, const String args[] )
    {
        //Value_Map::const_iterator pSoIter = mScriptInstances.find(instname);

        std::string sReturn = "";

        //if( pSoIter != mScriptInstances.end() )
        //{
        //    // Converts the String args into ruby VALUES
        //    VALUE *rArgs = rubyArgs( argc, args );
        //    int error=0;
        //    VALUE cgargs[4];
        //    cgargs[0] = pSoIter->second;
        //    cgargs[1] = rb_str_new2( funcname.c_str() );
        //    cgargs[2] = rb_int_new(argc);
        //    cgargs[3] = rArgs[0];

        //    // Calls the Function of our script instance
        //    VALUE rReturn = rb_protect( ( VALUE(*)(VALUE) )rb_funcall_wrapper,(VALUE)&cgargs[0],&error );
        //    logPythonErrors("PythonInterpreter::setStringFunction", error);
        //    //	VALUE rReturn = rb_funcall2(pSoIter->second, rb_intern( funcname.c_str() ), argc, rArgs);

        //    sReturn = STR2CSTR(rReturn);
        //    // delete args
        //    delete[] rArgs;
        //}
        return sReturn;	
    }
}
