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
 
#include "ConfigurationManager.h"
#include "ConfigurationManagerLinux.h"
#include "ConfigurationManagerWin32.h"
#include "ConfigurationManagerMac.h"

#include <OgreRenderSystem.h>
#include <OgreResourceGroupManager.h>

#include "CoreSubsystem.h"

using namespace Ogre;

namespace rl
{
	ConfigurationManager* ConfigurationManager::getSingletonPtr()
	{
#       if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		    return ConfigurationManagerLinux::getSingletonPtr();
#       elif OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		    return ConfigurationManagerWin32::getSingletonPtr();
#       else
		    return ConfigurationManagerMac::getSingletonPtr();
#       endif
	}
	
	ConfigurationManager& ConfigurationManager::getSingleton()
	{
#       if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		    return ConfigurationManagerLinux::getSingleton();
#       elif OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		    return ConfigurationManagerWin32::getSingleton();
#       else
		    return ConfigurationManagerMac::getSingleton();
#       endif
	}
	
    ConfigurationManager::ConfigurationManager() :
        mSystemConfig(NULL)
    {
    }

	ConfigurationManager::~ConfigurationManager()
	{
        delete mSystemConfig;
	}
	
	Ogre::String ConfigurationManager::getOgreLogPath() const
	{
		return mOgreLogPath;
	}
	
	Ogre::String ConfigurationManager::getRastullahCfgPath() const
	{
		return mRastullahCfgPath;
	}
	
	Ogre::String ConfigurationManager::getPluginCfgPath() const
	{
		return mPluginCfgPath;
	}
	
	Ogre::String ConfigurationManager::getRastullahLogPath() const
	{
		return mRastullahLogPath;
	}
	
	Ogre::String ConfigurationManager::getModulesCfgPath() const
	{
		return mModulesCfgPath;
	}
	
	Ogre::String ConfigurationManager::getModulesRootDirectory() const
	{
		return mModulesRootDirectory;
	}

    Ogre::String ConfigurationManager::getRastullahSystemCfgPath() const
    {
        return mRastullahSystemCfgPath;
    }

    Ogre::String ConfigurationManager::getSoundCfgPath() const
    {
        return mSoundCfgPath;
    }

    void ConfigurationManager::loadConfig()
    {
        if( mSystemConfig == NULL )
        {
            mSystemConfig = new Ogre::ConfigFile();

            try
            {            
                mSystemConfig->load( mRastullahSystemCfgPath, "=", true );
            }           
            catch ( Ogre::Exception  ) 
            {
                LOG_ERROR2(Logger::CORE, 
                    "Konnte Rastullah-System-Konfiguration aus '" + mRastullahSystemCfgPath + 
                    "' nicht laden! Defaulteinstellungen werden benutzt.","Configuration");
            }
        }        
    }

    bool ConfigurationManager::shouldUseStaticGeometry() const
    {
        Ogre::String mode = mSystemConfig->getSetting( "use_static_geometry" );
        
        if( mode == Ogre::StringUtil::BLANK )
            mode = "auto";
        
        if( mode.compare("yes") == 0 )
        {
            return true;
        }
        else if( mode.compare("no") == 0 )
        {
            return false;
        }        
        else
        {
            // Überprüfen ob der Renderer VertexBuffer unterstützt
            return Ogre::Root::getSingleton().getRenderSystem()
                ->getCapabilities()->hasCapability( Ogre::RSC_VBO );
        }
        
    }

	Ogre::String ConfigurationManager::getTextureUnitScheme() const
	{
		int numTu = Ogre::Root::getSingleton().getRenderSystem()
			->getCapabilities()->getNumTextureUnits();

		Ogre::String scheme;

		if (numTu >= 6)
			scheme = "tu6";
		else if (numTu >= 2)
			scheme = "tu2";
		else 
			scheme = "tu1";

		LOG_MESSAGE(Logger::CORE, 
			"Found "+Ogre::StringConverter::toString(numTu) + " Texture Units, using "+
			"alphablending scheme "+scheme);
		return scheme;
	}

	Logger::LogLevel ConfigurationManager::getLogLevel() const
	{
		int loglevel = Ogre::StringConverter::parseInt(mSystemConfig->getSetting("log_level"));
		return static_cast<Logger::LogLevel>(loglevel);
	}

	Ogre::String ConfigurationManager::getEngineVersionString() const
	{
		static Ogre::String version = "0.2.0";
		return version;
	}

	Ogre::String ConfigurationManager::getEngineVersionName() const
	{
		static Ogre::String version = "Internal Build";
		return version;
	}

	static const Ogre::String sMonths[] = 
			{"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
             "Sep", "Oct", "Nov", "Dec"};

	/// __DATE__ sieht ca. so aus : Nov 08 2005
	long parseDate(char* date)
	{   
		Ogre::String dateStr = Ogre::String(date);
		Ogre::String monthStr = dateStr.substr(0,3);
		int day = Ogre::StringConverter::parseInt( dateStr.substr(4,2) );
		int year = Ogre::StringConverter::parseInt( dateStr.substr(7,4) );
		int month = 0;

		while( month < 12 && monthStr.compare(sMonths[month]) != 0 )
			month++;

		return /* Jahr */		  year * 100000 +
			   /* Monat */	 (month+1) * 1000 + 
			   /* Tag */		   day * 10 + 
			   /* Sub-Version */	 0;	
	}

	long ConfigurationManager::getEngineBuildNumber() const
	{
		static long buildNumber = parseDate(__DATE__);
		return buildNumber;
	}


	const CeGuiString& ConfigurationManager::getAboutText() const
	{
		static CeGuiString aboutText = 
			"Rastullahs Lockenpracht\n\nCopyright 2003-2006 Team Pantheon\n\nBenutzte Bibliotheken: Ogre, fmod, Newton, boost, ...";
		return aboutText;
	}

	String ConfigurationManager::getStringSetting(const String& name, const String& defaultv) const
	{
		String rval = mSystemConfig->getSetting(name);
		if (rval == StringUtil::BLANK) rval = defaultv;
		return rval;
	}

	Real ConfigurationManager::getRealSetting(const String& name, Real defaultv) const
	{
		String val = getStringSetting(name);
		if (val == StringUtil::BLANK) return defaultv;
		else return StringConverter::parseReal(val);
	}

	int ConfigurationManager::getIntSetting(const String& name, int defaultv) const
	{
		String val = getStringSetting(name);
		if (val == StringUtil::BLANK) return defaultv;
		else return StringConverter::parseInt(val);
	}

	bool ConfigurationManager::getBoolSetting(const String& name, bool defaultv) const
	{
		String val = getStringSetting(name);
		if (val == StringUtil::BLANK) return defaultv;
		else return StringConverter::parseBool(val);
	}

	String ConfigurationManager::getInputConfigPath() const
	{
		return mConfigModuleDirectory+"/rastullah_input.cfg";
	}

	String ConfigurationManager::getKeymap() const
	{
		return getStringSetting("Keymap", "keymap-german.xml");
	}

    String ConfigurationManager::getConfigModulePath() const
    {
        return mConfigModuleDirectory;
    }
}
