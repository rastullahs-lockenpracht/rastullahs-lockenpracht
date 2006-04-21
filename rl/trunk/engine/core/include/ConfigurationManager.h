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
 
#ifndef __RL_CONFIGURATIONMANAGER_H__
#define __RL_CONFIGURATIONMANAGER_H__

#include "CorePrerequisites.h"
/**
 * \file ConfigurationManager.h
 * Definiert die abstrakte Basisklasse für das Konfigurationsmanagement.
 */

#include <OgreConfigFile.h>

namespace rl
{
	class _RlCoreExport ConfigurationManager
	{
		public:
            ConfigurationManager();
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
			
			Ogre::String getOgreLogPath();
			Ogre::String getRastullahCfgPath();
			Ogre::String getPluginCfgPath();
			Ogre::String getRastullahLogPath();
			Ogre::String getModulesCfgPath();
			Ogre::String getModuleconfigCfgPath(const Ogre::String& module);
			Ogre::String getModulesRootDirectory();
            Ogre::String getRastullahSystemCfgPath();
            
			Ogre::String getStringSetting(const Ogre::String& name, const Ogre::String& default = Ogre::StringUtil::BLANK);
			Ogre::Real getRealSetting(const Ogre::String& name, Ogre::Real default = 0.0f);
			int getIntSetting(const Ogre::String& name, int default = 0);
			bool getBoolSetting(const Ogre::String& name, bool default = false);

            Ogre::LoggingLevel getLogLevel();

			Ogre::String getEngineVersionString() const;
			Ogre::String getEngineVersionName() const;
			long         getEngineBuildNumber() const;
			const rl::CeGuiString&  getAboutText() const;

			bool shouldUseStaticGeometry( );
			Ogre::String getTextureUnitScheme();
						
		protected:
			Ogre::String mOgreLogPath;
			Ogre::String mRastullahCfgPath;
			Ogre::String mPluginCfgPath;
			Ogre::String mRastullahLogPath;
			Ogre::String mModulesCfgPath;
			Ogre::String mModuleconfigCfgPath;
			Ogre::String mModulesRootDirectory;
            Ogre::String mRastullahSystemCfgPath;

			Ogre::ConfigFile* getSystemConfig();

		private:
            Ogre::ConfigFile* mSystemConfig;
	};
}

#endif
