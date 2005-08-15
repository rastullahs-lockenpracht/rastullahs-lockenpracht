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

#ifndef __RL_CONFIGURATIONMANAGERMAC_H__
#define __RL_CONFIGURATIONMANAGERMAC_H__

/**
 * \file ConfigurationManagerMac.h
 * Definiert den Windows-Konfigurationsmanager
 */

#include "ConfigurationManager.h"

namespace rl
{
	class ConfigurationManagerMac : public ConfigurationManager
	{
		public:
			
			~ConfigurationManagerMac();
			
			/**
 			 * Die Methode gibt einen Pointer auf die Instanz des  
			 * configurationManagerMac zurück.
			 *\retval ein Pointer auf die Instanz des configurationManagerMac
			 */
			static ConfigurationManager* getSingletonPtr();
			
			/**
 			 * Die Methode gibt eine Referenz auf die Instanz des  
			 * configurationManagerMac zurück.
			 *\retval eine Referenz auf die Instanz des configurationManagerMac
			 */
			static ConfigurationManager& getSingleton();
			
		private:
			
			ConfigurationManagerMac();
			
			static ConfigurationManager* sInstance;
	};
}

#endif
