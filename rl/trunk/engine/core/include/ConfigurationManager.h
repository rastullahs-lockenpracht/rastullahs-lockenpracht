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
			virtual ~ConfigurationManager();
			
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
			
			Ogre::String getOgreLogPath() const;
			Ogre::String getRastullahCfgPath() const;
			Ogre::String getPluginCfgPath() const;
			Ogre::String getRastullahLogPath() const;
			Ogre::String getModulesCfgPath() const;
			Ogre::String getConfigModulePath() const;
			Ogre::String getModulesRootDirectory() const;
            Ogre::String getRastullahSystemCfgPath() const;
			Ogre::String getSoundCfgPath() const;
			Ogre::String getKeymap() const;
			Ogre::String getInputConfigPath() const;
            
			Ogre::String getStringSetting(const Ogre::String& name, const Ogre::String& _default = Ogre::StringUtil::BLANK) const;
			Ogre::Real getRealSetting(const Ogre::String& name, Ogre::Real _default = 0.0f) const;
			int getIntSetting(const Ogre::String& name, int _default = 0) const;
			bool getBoolSetting(const Ogre::String& name, bool _default = false) const;

            Logger::LogLevel getLogLevel() const;

			Ogre::String getEngineVersionString() const;
			Ogre::String getEngineVersionName() const;
			long         getEngineBuildNumber() const;
			const rl::CeGuiString&  getAboutText() const;

			bool shouldUseStaticGeometry() const;
			Ogre::String getTextureUnitScheme() const;

            void loadConfig();
						
		protected:
			Ogre::String mOgreLogPath;
			Ogre::String mRastullahCfgPath;
			Ogre::String mPluginCfgPath;
			Ogre::String mRastullahLogPath;
			Ogre::String mModulesCfgPath;
			Ogre::String mConfigModuleDirectory;
			Ogre::String mModulesRootDirectory;
            Ogre::String mRastullahSystemCfgPath;
			Ogre::String mSoundCfgPath;

        private:
            Ogre::ConfigFile* mSystemConfig;
	};
}

#endif
