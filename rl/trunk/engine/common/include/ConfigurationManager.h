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
 
#ifndef __RL_CONFIGURATIONMANAGER_H__
#define __RL_CONFIGURATIONMANAGER_H__

/**
 * \file ConfigurationManager.h
 * Definiert die abstrakte Basisklasse für das Konfigurationsmanagement.
 */

#include <string>

namespace rl
{
	class ConfigurationManager
	{
		public:

			~ConfigurationManager();
			
			/**
 			 * Die Methode gibt einen Pointer auf ein spezialisiertes, von 
			 * configurationManager abgeleitetes Objekt zurück welches die 
			 * Pfade und sonstigen wichtigen Konfigurationsmerkmale für die 
			 * aktuelle Plattform beinhaltet.
			 *\retval ein Pointer auf einen spezialisierten 
			 *configurationManager
			 */
			static ConfigurationManager* getSingletonPtr();
			
			/**
 			 * Die Methode gibt eine Referenz auf ein spezialisiertes, von 
			 * configurationManager abgeleitetes Objekt zurück welches die 
			 * Pfade und sonstigen wichtigen Konfigurationsmerkmale für die 
			 * aktuelle Plattform beinhaltet.
			 *\retval eine Referenz auf einen spezialisierten 
			 *configurationManager
			 */
			static ConfigurationManager& getSingleton();
			
			std::string getOgreLogPath();
			
			std::string getRastullahCfgPath();
			
			std::string getPluginCfgPath();
			
			std::string getRlCoreLogPath();
			
			std::string getModulesCfgPath();
			
			std::string getModuleconfigCfgPath(const std::string& module);
			
		protected:
		
			std::string ogreLogPath;
			std::string rastullahCfgPath;
			std::string pluginCfgPath;
			std::string rlCoreLogPath;
			std::string modulesCfgPath;
			std::string moduleconfigCfgPath;
	};
}

#endif
