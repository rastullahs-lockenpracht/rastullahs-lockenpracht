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

#ifndef __RL_CONFIGURATIONMANAGERWIN32_H__
#define __RL_CONFIGURATIONMANAGERWIN32_H__

/**
 * \file ConfigurationManagerWin32.h
 * Definiert den Windows-Konfigurationsmanager
 */

#include "ConfigurationManager.h"

namespace rl
{
	class _RlCoreExport ConfigurationManagerWin32 : public ConfigurationManager
	{
		public:
			
			~ConfigurationManagerWin32();
			
			/**
 			 * Die Methode gibt einen Pointer auf die Instanz des  
			 * configurationManagerWin32 zurück.
			 *\retval ein Pointer auf die Instanz des configurationManagerWin32
			 */
			static ConfigurationManager* getSingletonPtr();
			
			/**
 			 * Die Methode gibt eine Referenz auf die Instanz des  
			 * configurationManagerWin32 zurück.
			 *\retval eine Referenz auf die Instanz des 
			 *configurationManagerWin32
			 */
			static ConfigurationManager& getSingleton();
			
		private:
			
			ConfigurationManagerWin32();
			
			static ConfigurationManager* instance;
	};
}

#endif
