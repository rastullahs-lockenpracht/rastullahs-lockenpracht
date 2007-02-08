/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#include "CoreSubsystem.h"

template<> rl::ConfigurationManager* Ogre::Singleton<rl::ConfigurationManager>::ms_Singleton = 0;

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

    ConfigurationManager::ConfigurationManager()
    {
        // Filenames for log and configuration files
        mRastullahCfgFile = "rastullah.conf";
        mOgreLogFile = "ogre.log";
        mCeguiLogFile = "cegui.log";
        mRastullahLogFile = "rastullah.log";
        mModulesCfgFile = "modules.cfg";

        // Setup general default values
        mGeneralSettings.insert(make_pair("Log Level", "2"));
        mGeneralSettings.insert(make_pair("Subtitle Speed", "0.0"));

        // Setup sound default values
        mSoundSettings.insert(make_pair("DefaultMusicVolume", "0.4"));
        mSoundSettings.insert(make_pair("DefaultSoundVolume", "0.9"));
        mSoundSettings.insert(make_pair("MasterVolume", "1"));

        // Setup graphic default values
#       if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        mGraphicSettings.insert(make_pair("Render System", "Direct3D9 Rendering Subsystem"));
#       else
        mGraphicSettings.insert(make_pair("Render System","OpenGL Rendering Subsystem"));
        mGraphicSettings.insert(make_pair("RTT Preferred Mode", "FBO"));
#       endif
        mGraphicSettings.insert(make_pair("Fullscreen", "no"));
        mGraphicSettings.insert(make_pair("FSAA", "0"));
        mGraphicSettings.insert(make_pair("Video Mode", "800 x 600"));
        mGraphicSettings.insert(make_pair("Use Static Geometry", "auto"));
        mGraphicSettings.insert(make_pair("Max Anisotropy", "1"));

        // Setup input default values
        mInputSettings.insert(make_pair("Mouse Sensitivity", "4"));
        mInputSettings.insert(make_pair("Mouse Invert", "no"));

        setRastullahCfgPath();
    }

    ConfigurationManager::~ConfigurationManager()
    {
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
        return mModulesRootDirectory  + "/" + mModulesCfgFile;
    }

    Ogre::String ConfigurationManager::getModulesRootDirectory() const
    {
        return mModulesRootDirectory;
    }

    Ogre::String ConfigurationManager::getRastullahLogDirectory() const
    {
        return mRastullahLogDirectory;
    }

    Ogre::String ConfigurationManager::getOgrePluginDirectory() const
    {
        return mOgrePluginDirectory;
    }

    Ogre::NameValuePairList ConfigurationManager::getGraphicSettings() const
    {
        return mGraphicSettings;
    }

    Ogre::NameValuePairList ConfigurationManager::getSoundSettings() const
    {
        return mSoundSettings;
    }

    Ogre::NameValuePairList ConfigurationManager::getInputSettings() const
    {
        return mInputSettings;
    }

    void ConfigurationManager::loadConfig()
    {
        // On Linux, we create the .rastullah directory
#       if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
        fs::path rastullahCfgDirectory(string(::getenv("HOME")) + "/.rastullah",
            fs::portable_posix_name);

        if (!fs::exists(rastullahCfgDirectory))
        {
            fs::create_directory(rastullahCfgDirectory);
        }
#       endif

        // First check, if we have found any directory
        if (!mRastullahCfgPath.empty())
        {
            ConfigFile* configfile = new ConfigFile();
            std::vector<Ogre::String> cfg_paths;

            if (mRastullahCfgPath.find(":") == std::string::npos)
            {
                cfg_paths.push_back(mRastullahCfgPath);
            }
            else
            {
                // Split the path at the ':' character
                cfg_paths = Ogre::StringUtil::split(mRastullahCfgPath, ":");
            }

            for (unsigned int i = 0; i < cfg_paths.size(); i++)
            {
                try
                {
                    std::cout << "Loading Configuration File " << cfg_paths[i]
                        << mRastullahCfgFile << std::endl;
                    configfile->load(cfg_paths[i] + mRastullahCfgFile, "=", true);
                }
                catch (Ogre::Exception)
                {
                    LOG_ERROR2(Logger::CORE,
                         "Konnte Rastullah-Konfiguration in '" + cfg_paths[i] +
                         "' nicht laden! Defaulteinstellungen werden benutzt.","Configuration");
                }

                // Set log, modules and OGRE plugin directory
                mRastullahLogDirectory = configfile->getSetting("LogDirectory", "Paths");
                mModulesRootDirectory = configfile->getSetting("ModulesDirectory", "Paths");
                mOgrePluginDirectory = configfile->getSetting("OgrePluginDirectory", "Paths");

                // Get general settings
                try
                {
                    for (ConfigFile::SettingsIterator it =
                        configfile->getSettingsIterator("General");
                        it.hasMoreElements(); it.moveNext())
                    {
                        addSetting(CS_GENERAL, it.peekNextKey(), it.peekNextValue());
                    }
                }
                catch (Ogre::Exception& oe)
                {
                    // Log it as information
                    LOG_WARNING(Logger::CORE, "Configuration File " + cfg_paths[i] +
                        mRastullahCfgFile +
                        " does not contain a section 'General'. Defaults will be used.");
                }

                // Get video settings
                try
                {
                    for (ConfigFile::SettingsIterator it =
                        configfile->getSettingsIterator("Video");
                        it.hasMoreElements(); it.moveNext())
                    {
                        addSetting(CS_GRAPHICS, it.peekNextKey(), it.peekNextValue());
                    }
                }
                catch (Ogre::Exception& oe)
                {
                    // Log it as information
                    LOG_WARNING(Logger::CORE, "Configuration File " + cfg_paths[i] +
                        mRastullahCfgFile +
                        " does not contain a section 'Video'. Defaults will be used.");
                }

                // Get sound settings
                try
                {
                    for (ConfigFile::SettingsIterator it =
                        configfile->getSettingsIterator("Sound");
                        it.hasMoreElements(); it.moveNext())
                    {
                        addSetting(CS_SOUND, it.peekNextKey(), it.peekNextValue());
                    }
                }
                catch (Ogre::Exception& oe)
                {
                    // Log it as information
                    LOG_WARNING(Logger::CORE, "Configuration File " + cfg_paths[i] +
                        mRastullahCfgFile +
                        " does not contain a section 'Video'. Defaults will be used.");
                }

                // Get input settings
                try
                {
                    for (ConfigFile::SettingsIterator it =
                        configfile->getSettingsIterator("Input");
                        it.hasMoreElements(); it.moveNext())
                    {
                        addSetting(CS_INPUT, it.peekNextKey(), it.peekNextValue());
                    }
                }
                catch (Ogre::Exception& oe)
                {
                    // Log it as information
                    LOG_WARNING(Logger::CORE, "Configuration File " + cfg_paths[i] +
                        mRastullahCfgFile +
                        " does not contain a section 'Input'. Defaults will be used.");
                }
            }

            delete configfile;
        }

        if (mRastullahLogDirectory.empty())
        {
#           if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
            mRastullahLogDirectory = string(::getenv("HOME")) + "/.rastullah/logs";
#           else
            mRastullahLogDirectory = "./logs";
#           endif
        }

        if (mModulesRootDirectory.empty())
        {
#           if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
                mModulesRootDirectory = RL_MODULEDIR;
#           else
                mModulesRootDirectory = "./modules";
#           endif
        }

        if (mOgrePluginDirectory.empty())
        {
#           if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
            mOgrePluginDirectory = OGRE_PLUGINDIR;
#           else
            mOgrePluginDirectory = ".";
#           endif
        }
    }

    void ConfigurationManager::saveConfig()
    {
        ConfigFile* cfgfile = new ConfigFile();

        // Create the path list
        Ogre::NameValuePairList* PathSettings = new Ogre::NameValuePairList();
        PathSettings->insert(make_pair("ModulesDirectory", mModulesRootDirectory));
        PathSettings->insert(make_pair("OgrePluginDirectory", mOgrePluginDirectory));
        PathSettings->insert(make_pair("LogDirectory", mRastullahLogDirectory));

        // Add General settings
        cfgfile->addSection("General", mGeneralSettings);
        // Add Path settings
        cfgfile->addSection("Path", *PathSettings);
        // Add Video settings
        cfgfile->addSection("Video", mGraphicSettings);
        // Add Sound settings
        cfgfile->addSection("Sound", mSoundSettings);
        // Add Input settings
        cfgfile->addSection("Input", mInputSettings);

#       if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        cfgfile->save(fs::path("./modules/config/" + mRastullahCfgFile).native_file_string());
#       else
        cfgfile->save(Ogre::String(::getenv("HOME")) + "/.rastullah/" + mRastullahCfgFile);
#       endif

        // Clean up
        delete PathSettings;
        delete cfgfile;
    }

    Logger::LogLevel ConfigurationManager::getLogLevel()
    {
        return static_cast<Logger::LogLevel>(getIntSetting(ConfigurationManager::CS_GENERAL,
            "Log Level"));
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

    Ogre::String ConfigurationManager::getStringSetting(ConfigurationSection section,
        Ogre::String key)
    {
        // Get the value we are looking for
        return findSetting(section, key);
    }

    int ConfigurationManager::getIntSetting(ConfigurationSection section, Ogre::String key)
    {
        // Get the value we are looking for
        return Ogre::StringConverter::parseInt(findSetting(section, key));
    }

    bool ConfigurationManager::getBoolSetting(ConfigurationSection section, Ogre::String key)
    {
        // Get the value we are looking for
        return Ogre::StringConverter::parseBool(findSetting(section, key));
    }

    Ogre::Real ConfigurationManager::getRealSetting(ConfigurationSection section, Ogre::String key)
    {
        // Get the value we are looking for
        return Ogre::StringConverter::parseReal(findSetting(section, key));
    }

    Ogre::String ConfigurationManager::findSetting(ConfigurationSection section,
        Ogre::String key) const
    {
        Ogre::NameValuePairList::const_iterator it;
        const Ogre::NameValuePairList* searchList;

        // Search for the key in the specified section
        switch (section)
        {
            case CS_GENERAL:
                searchList = &mGeneralSettings;
            break;
            case CS_GRAPHICS:
                searchList = &mGraphicSettings;
            break;
            case CS_SOUND:
                searchList = &mSoundSettings;
            break;
            case CS_INPUT:
                searchList = &mInputSettings;
            break;
            default:

            break;
        }

        it = searchList->find(key);

        if (it == searchList->end())
        {
            return Ogre::String("");
        }
        else
        {
            return it->second;
        }
    }

    void ConfigurationManager::addSetting(ConfigurationSection section, Ogre::String key,
        Ogre::String value)
    {
        Ogre::NameValuePairList::iterator it;
        Ogre::NameValuePairList* addList;

        // Get the section, where the key is inserted
        switch (section)
        {
            case CS_GENERAL:
                addList = &mGeneralSettings;
            break;
            case CS_GRAPHICS:
                addList = &mGraphicSettings;
            break;
            case CS_SOUND:
                addList = &mSoundSettings;
            break;
            case CS_INPUT:
                addList = &mInputSettings;
            break;
            default:

            break;
        }

        // Find out, if key exist
        it = addList->find(key);

        if (it == addList->end())
        {
            // Key does not exist, insert
            addList->insert(make_pair(key, value));
        }
        else
        {
            // Key does exist, update
            it->second = value;
        }
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

        return /* Jahr */          year * 100000 +
               /* Monat */     (month+1) * 1000 +
               /* Tag */           day * 10 +
               /* Sub-Version */     0;
    }

    long ConfigurationManager::getEngineBuildNumber() const
    {
        static long buildNumber = parseDate(__DATE__);
        return buildNumber;
    }


    const CeGuiString& ConfigurationManager::getAboutText() const
    {
        static CeGuiString aboutText =
            "Rastullahs Lockenpracht\n\nCopyright 2003-2006 Team Pantheon\
\n\nBenutzte Bibliotheken: Ogre, fmod, Newton, boost, ...";
        return aboutText;
    }

    Ogre::String ConfigurationManager::getKeymap() const
    {
        return Ogre::String("config/keymap-german.xml");
    }

    void ConfigurationManager::setRastullahCfgPath()
    {
        // First try: Current directory
        addToCfgPath("./");

#       if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
        // Check system wide /etc/rastullah directory
        addToCfgPath("/etc/rastullah/");
        // Check home .rastullah directory
        addToCfgPath(Ogre::String(::getenv("HOME")) + "/.rastullah/");
#       else
        addToCfgPath("./modules/config/");
#       endif
    }

    void ConfigurationManager::addToCfgPath(const std::string& path)
    {
        std::cout << "Checking for " << mRastullahCfgFile << " in " << path << std::endl;

        if (checkForFile(path + mRastullahCfgFile))
        {
            if (mRastullahCfgPath.size() > 0)
            {
                mRastullahCfgPath += ":" + path;
            }
            else
            {
                mRastullahCfgPath = path;
            }

            std::cout << "Adding path " << path << " to Rastullah configuration path."
                << std::endl;
        }
    }

    bool ConfigurationManager::checkForFile(const std::string& filename)
    {
        try {
#           if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            if (fs::exists(filename))
#           else
            if (fs::exists(fs::path(filename, fs::portable_posix_name)))
#           endif
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
