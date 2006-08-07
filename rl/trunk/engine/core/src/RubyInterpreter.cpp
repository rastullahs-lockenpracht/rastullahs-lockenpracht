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


#include "RubyInterpreter.h"

#include <OgreNoMemoryMacros.h>
#include "FixRubyHeaders.h"
#include <ruby.h>
#include "FixRubyHeaders.h"
#include <OgreMemoryMacros.h>

#include "CoreSubsystem.h"
#include "ConfigurationManager.h"
#include "ContentModule.h"

using namespace Ogre;

namespace rl {

RubyInterpreter::RubyInterpreter()
{
    
}

RubyInterpreter::~RubyInterpreter()
{

}

void RubyInterpreter::initializeInterpreter()
{
    #if defined(NT)
        static int dummyargc(0);
        static char** vec;
        NtInitialize(&dummyargc, &vec);
    #endif

    //Ruby Initialisieren
    ruby_init();

    // UTF 8 aktivieren
    execute( "$KCODE = 'u'" );

    //Skript-Verzeichnisse der  Dateien duerfen auch in /script liegen
    ModuleMap modules = CoreSubsystem::getSingleton().getAllModules();
    for (ModuleMap::iterator iter = modules.begin(); iter != modules.end(); iter++)
    {
        ContentModule* mod = (*iter).second;
        //wir suchen die Scripte im modules Verzeichnis relativ zum ModuleRootPath!
        addSearchPath(mod->getDirectory() + "/conf");
        addSearchPath(mod->getDirectory() + "/scripts");
        addSearchPath(mod->getDirectory() + "/scripts/maps");
    }
    
    ruby_init_loadpath();
    //Skriptname
    ruby_script("Rastullah");
    // Fuer Ruby .dll oder .so dazu laden
    
    loadProtected(&RubyInterpreter::loadDlls, 0, "Ruby error while loading dlls");
}


void RubyInterpreter::finalizeInterpreter()
{
    ruby_finalize();
}

void RubyInterpreter::setOutputFunction(staticValueMethod func)
{
    //Ersetzt die Standard-Ausgabe von Ruby durch Ausgaben in die Console
    rb_defout = rb_str_new("", 0);
    // Eigentlich nicht mehr notwendig, aber ohne das gibts nen Absturz?!?!
    // rb_define_singleton_method(rb_defout, "write", (VALUE(*)(...))console_write, 1);
    rb_define_singleton_method(rb_defout, "write", func, 1);
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
            LOG_ERROR(Logger::CORE, intro);
        LOG_ERROR(Logger::CORE, STR2CSTR(info));
    }
}

bool RubyInterpreter::execute(const String& command)
{
    int status = -1;

    LOG_MESSAGE2(Logger::CORE, command, "RubyInterpreter::execute" );
    rb_eval_string_protect(command.c_str(), &status);

    logRubyErrors("", status);

    if( status )
    {
        rb_eval_string_protect("print $!", &status);
        return false;
    }

    return true;
}

bool RubyInterpreter::executeFile(String rubyfile)
{
    bool error = execute("load '" + rubyfile + "'");
    if (CoreSubsystem::getSingleton().isInitialized())
    {
        execute("load 'checkerrors.rb'");
    }
    return error;
}

CeGuiString RubyInterpreter::val2ceguistr(const VALUE rval)
{
    return CeGuiString(
        (CEGUI::utf8*)STR2CSTR(
            rb_funcall(
                rval, 
                rb_intern("to_s"), 
                0)));
}

}
