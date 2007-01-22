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

#include <OgreRenderSystem.h>
#include <OgreResourceGroupManager.h>

#include "CoreSubsystem.h"

using namespace Ogre;

template<> rl::ConfigurationManager* Singleton<rl::ConfigurationManager>::ms_Singleton = 0;

namespace rl
{
	ConfigurationManager* ConfigurationManager::getSingletonPtr()
	{
		if (ms_Singleton == NULL)
		{
			ms_Singleton = new ConfigurationManager();
		}

		return ms_Singleton;
	}

	ConfigurationManager& ConfigurationManager::getSingleton()
	{
		if (ms_Singleton == NULL)
		{
			ms_Singleton = new ConfigurationManager();
		}

		return *ms_Singleton;
	}

    ConfigurationManager::ConfigurationManager() :
        mSystemConfig(NULL)
    {
    	mRastullahCfgFile = "rastullah.conf";
    	mOgreLogFile = "ogre.log";
    	mCeguiLogFile = "cegui.log";
		mModulesCfgFile = "modules.cfg";

    	setRastullahCfgPath();
    }

	ConfigurationManager::~ConfigurationManager()
	{
        delete mSystemConfig;
	}

	Ogre::String ConfigurationManager::getOgreLogFile() const
	{
		return mRastullahLogDirectory + "/" + mOgreLogFile;
	}

	Ogre::String ConfigurationManager::getRastullahLogFile() const
	{
		return mRastullahLogDirectory + "/" + mRastullahLogFile;
	}

	Ogre::String ConfigurationManager::getCeguiLogFile() const
	{
		return mRastullahLogDirectory + "/" + mCeguiLogFile;
	}

	Ogre::String ConfigurationManager::getModulesCfgFile() const
	{
		return mModulesCfgDirectory  + "/" + mModulesCfgFile;
	}

	Ogre::String ConfigurationManager::getModulesRootDirectory() const
	{
		return mModulesRootDirectory;
	}

	Ogre::String ConfigurationManager::getRastullahLogDirectory() const
	{
		return mRastullahLogDirectory;
	}

	/**
	 * @todo Remove, after other subsystems were changed to
	 *       new ConfigurationManager style
	 */
	Ogre::String ConfigurationManager::getRastullahCfgPath() const
	{
		return mRastullahOgreCfgPath;
	}

	Ogre::String ConfigurationManager::getPluginCfgFile() const
	{
		return mPluginCfgFile;
	}

	Ogre::String ConfigurationManager::getOgreLogPath() const
	{
		return mOgreLogPath;
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

    Ogre::String ConfigurationManager::getRastullahSystemCfgPath() const
    {
        return mRastullahSystemCfgPath;
    }

    Ogre::String ConfigurationManager::getSoundCfgPath() const
    {
        return mSoundCfgPath;
    }
    /// end remove

    void ConfigurationManager::loadConfig()
    {
        if(mSystemConfig == NULL)
        {
        	// On Linux, we create the .rastullah directory
#           if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
			fs::path rastullahCfgDirectory(string(::getenv("HOME")) + "/.rastullah", fs::portable_posix_name);

			if (!fs::exists(rastullahCfgDirectory))
			{
				fs::create_directory(rastullahCfgDirectory);
			}

			mRastullahOgreCfgPath = string(::getenv("HOME")) + "/.rastullah/rastullah_ogre.cfg";
#           else
			mRastullahOgreCfgPath = "./rastullah_ogre.cfg";
#           endif

            mSystemConfig = new Ogre::ConfigFile();
            std::vector<String> cfg_paths;

			if (mRastullahCfgPath.find(":") == std::string::npos)
			{
				cfg_paths.push_back(mRastullahCfgPath);
			}
			else
			{
				// Split the path at the ':' character
				cfg_paths = StringUtil::split(mRastullahCfgPath, ":");
			}

			for (int i = 0; i < cfg_paths.size(); i++)
			{
				try
				{
					std::cout << "Loading Configuration File " << cfg_paths[i] << std::endl;
					mSystemConfig->load(cfg_paths[i] + mRastullahCfgFile, "=", true);
				}
				catch (Ogre::Exception)
				{
					LOG_ERROR2(Logger::CORE,
						"Konnte Rastullah-Konfiguration in '" + cfg_paths[i] +
						"' nicht laden! Defaulteinstellungen werden benutzt.","Configuration");
				}

				// Set log and modules directory and plugin cfg file name
				mRastullahLogDirectory = mSystemConfig->getSetting("LogDirectory", "Rastullah");

				if (mRastullahLogDirectory.empty())
				{
#                   if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
					mRastullahLogDirectory = string(::getenv("HOME")) + "/.rastullah/logs";
#                   else
					mRastullahLogDirectory = "./logs";
#                   endif
				}

				mModulesRootDirectory = mSystemConfig->getSetting("ModulesDirectory", "Rastullah");
				mPluginCfgFile = mSystemConfig->getSetting("PluginCfg", "Rastullah");
			}
        }

		mModulesCfgDirectory = mModulesRootDirectory + "/modules/config";

		/**
		 * @todo Remove, after other subsystems were changed to
		 *       new ConfigurationManager style
		 */
		mRastullahCfgPath = "./rastullah_ogre.cfg";
		mRastullahSystemCfgPath = mModulesCfgDirectory + "/rastullah_system.cfg";
		mSoundCfgPath = mModulesCfgDirectory + "/sound.cfg";
		mPluginCfgPath = mModulesCfgDirectory + "/" + mPluginCfgFile;

		mModulesCfgPath = mModulesRootDirectory + "/modules/modules.cfg";
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

    void ConfigurationManager::setRastullahCfgPath()
    {
    	string path = "./";

    	// First try: Current directory
    	if (checkForFile(path + mRastullahCfgFile))
    	{
			addToCfgPath(path);
    	}

#		if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		// Check system wide /etc/rastullah directory
		path = "/etc/rastullah/";

    	if (checkForFile(path + mRastullahCfgFile))
    	{
			addToCfgPath(path);
    	}

		// Check home .rastullah directory
    	path = string(::getenv("HOME")) + "/.rastullah/";

    	if (checkForFile(path + mRastullahCfgFile))
    	{
			addToCfgPath(path);
    	}
#		endif
    }

	void ConfigurationManager::addToCfgPath(const std::string& path)
	{
		if (mRastullahCfgPath.size() > 0)
		{
			mRastullahCfgPath += ":" + path;
		}
		else
		{
			mRastullahCfgPath = path;
		}
	}

	bool ConfigurationManager::checkForFile(const std::string& filename)
	{
		try {
			if (fs::exists(filename))
			{
				return true;
			}
		}
		catch (fs::filesystem_error& fe) {
			return false;
		}

		return false;
	}
}
