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

#include "RulesSubsystem.h"
#include "ActionManager.h"
#include "DsaManager.h"
#include "DsaDataLoader.h"
#include "Log.h"

template <>
rl::RulesSubsystem* Singleton<rl::RulesSubsystem> ::ms_Singleton = 0;
namespace rl
{
    RulesSubsystem& RulesSubsystem::getSingleton(void)
    {
        return Singleton<RulesSubsystem>::getSingleton();
    }

    RulesSubsystem* RulesSubsystem::getSingletonPtr(void)
    {
        return Singleton<RulesSubsystem>::getSingletonPtr();
    }

    RulesSubsystem::RulesSubsystem()
    {
		log(Ogre::LML_TRIVIAL, "Start");
        //Zufallsgenerator initialisieren
        srand(static_cast<unsigned int>(time(NULL)));

		
        //Singletons erzeugen
        new ActionManager(); 
		log(Ogre::LML_TRIVIAL, "ActionManager erzeugt");
        new DsaManager();
		log(Ogre::LML_TRIVIAL, "DsaManager erzeugt");

		//Daten laden
		DsaDataLoader::loadData("basis.xdi");
		log(Ogre::LML_TRIVIAL, "Basisdaten geladen");
		
		log(Ogre::LML_TRIVIAL, "Erzeugen abgeschlossen");		
    }
	
	void RulesSubsystem::log(Ogre::LogMessageLevel level, const Ogre::String& msg, const Ogre::String& ident )
	{
		Logger::getSingleton().log(level, "Rules", msg, ident);
	}

	void RulesSubsystem::log(Ogre::LogMessageLevel level, const char* msg, const Ogre::String& ident )
	{
		log(level, Ogre::String(msg), ident);
	}

	void RulesSubsystem::log(Ogre::LogMessageLevel level, const CeGuiString& msg, const Ogre::String& ident )
	{
		log(level, Ogre::String(msg.c_str()), ident);
	}

    RulesSubsystem::~RulesSubsystem()
    {
        delete DsaManager::getSingletonPtr();
    }
}
