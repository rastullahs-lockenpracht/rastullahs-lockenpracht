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

/**
 * \file ConfigurationManager.h
 *
 * Implementation of the ConfigurationManager subsystem
 */

#include "ConfigurationManager.h"
#include "CoreSubsystem.h"

template<> rl::ConfigurationManager* Ogre::Singleton<rl::ConfigurationManager>::ms_Singleton = 0;

using namespace std;

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
        mInputSettings.insert(make_pair("mov_turn_left", "Q"));
        mInputSettings.insert(make_pair("mov_turn_right", "E"));
        mInputSettings.insert(make_pair("mov_move_left", "A"));
        mInputSettings.insert(make_pair("mov_move_right", "D"));
        mInputSettings.insert(make_pair("mov_move_forward", "W"));
        mInputSettings.insert(make_pair("mov_move_backward", "S"));
        mInputSettings.insert(make_pair("mov_jump", "Leertaste"));
        mInputSettings.insert(make_pair("mov_run", "Umschalt links"));
        mInputSettings.insert(make_pair("mov_sneak", "Strg links"));
        mInputSettings.insert(make_pair("mov_run_lock", "Feststelltaste"));
        mInputSettings.insert(make_pair("act_quitgame", "Ctrl+X"));
        mInputSettings.insert(make_pair("act_closecurrentwindow", "Escape"));
        mInputSettings.insert(make_pair("act_toggleconsole", "Tabulator"));
        mInputSettings.insert(make_pair("act_toggledebugwindow", "F2"));
        mInputSettings.insert(make_pair("act_togglegamelogwindow", "F3"));
        mInputSettings.insert(make_pair("act_toggleviewmode", "F"));
        mInputSettings.insert(make_pair("act_resetcamera", "0 (Nummernblock)"));
        mInputSettings.insert(make_pair("act_makescreenshot", "P,Ctrl+P"));
        mInputSettings.insert(make_pair("act_togglecollisiondebug", "L,Ctrl+L"));
        mInputSettings.insert(make_pair("act_toggledebugvisualisation", "Ctrl+O"));
        mInputSettings.insert(make_pair("act_togglevanitymode", "V"));
        mInputSettings.insert(make_pair("act_usecurrentobjectdefaultaction", "R"));
        mInputSettings.insert(make_pair("act_toggleinventorywindow", "I"));
        mInputSettings.insert(make_pair("act_showjournalwindow", "J"));
        mInputSettings.insert(make_pair("act_showcharactersheet", "C"));
        mInputSettings.insert(make_pair("act_togglecharacterstatewindow", "O"));
        mInputSettings.insert(make_pair("act_toggleingameglobalmenu", "F10"));
        mInputSettings.insert(make_pair("act_togglecharactercontroller", "M"));
        mInputSettings.insert(make_pair("act_shownextdebugwindowpage", "Ctrl+M"));
        mInputSettings.insert(make_pair("act_showobjectactions", "U"));

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

    Ogre::String ConfigurationManager::getModulesRootDirectory() const
    {
        return mModulesRootDirectory;
    }

    Ogre::String ConfigurationManager::getRastullahLogDirectory() const
    {
        return mRastullahLogDirectory;
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

    Ogre::StringVector ConfigurationManager::getPluginList() const
    {
        return mPluginList;
    }

    Ogre::StringVector ConfigurationManager::getModuleList() const
    {
        return mModuleList;
    }

    void ConfigurationManager::loadConfig()
    {
        // On Linux, we create the .rastullah directory
#       if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
        fs::path rastullahCfgDirectory(Ogre::String(::getenv("HOME")) + "/.rastullah",
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

            // Split the path at the ':' character
            std::vector<Ogre::String> cfg_paths = Ogre::StringUtil::split(mRastullahCfgPath, ":");

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
                if (!configfile->getSetting("LogDirectory", "Paths").empty())
                {
                    mRastullahLogDirectory = configfile->getSetting("LogDirectory", "Paths");
                }

                if (!configfile->getSetting("ModulesDirectory", "Paths").empty())
                {
                    mModulesRootDirectory = configfile->getSetting("ModulesDirectory", "Paths");
                }

                if (!configfile->getSetting("OgrePluginDirectory", "Paths").empty())
                {
                    mOgrePluginDirectory = configfile->getSetting("OgrePluginDirectory", "Paths");
                }

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
                catch (Ogre::Exception&)
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
                catch (Ogre::Exception&)
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
                catch (Ogre::Exception&)
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
                catch (Ogre::Exception&)
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
            mRastullahLogDirectory = Ogre::String(::getenv("HOME")) + "/.rastullah/logs";
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

        // Plugin list for OGRE specific to operating system
#       if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        Ogre::String dirSeparator = "\\";
        mPluginList.push_back(mOgrePluginDirectory + dirSeparator + "RenderSystem_Direct3D9");
#       else
        Ogre::String dirSeparator = "/";
#       endif
        mPluginList.push_back(mOgrePluginDirectory + dirSeparator + "RenderSystem_GL");
        mPluginList.push_back(mOgrePluginDirectory + dirSeparator + "Plugin_ParticleFX");
        mPluginList.push_back(mOgrePluginDirectory + dirSeparator + "Plugin_OctreeSceneManager");

        // Load the module list
        ConfigFile* configfile = new ConfigFile();
        configfile->load(fs::path(mModulesRootDirectory + "/" + mModulesCfgFile).native_directory_string());

        ConfigFile::SettingsIterator it = configfile->getSettingsIterator();
        for (ConfigFile::SettingsIterator it = configfile->getSettingsIterator();
             it.hasMoreElements(); it.moveNext())
        {
            // If key is module, we add value to our module list
            if (it.peekNextKey() == "module")
            {
                mModuleList.push_back(it.peekNextValue());
            }
        }

        delete configfile;
    }

    void ConfigurationManager::saveConfig() const
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
        cfgfile->addSection("Paths", *PathSettings);
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

    Logger::LogLevel ConfigurationManager::getLogLevel() const
    {
        return static_cast<Logger::LogLevel>(getIntSetting(ConfigurationManager::CS_GENERAL,
            "Log Level"));
    }

    Ogre::String ConfigurationManager::getStringSetting(const ConfigurationSection& section,
        const Ogre::String& key) const
    {
        // Get the value we are looking for
        return findSetting(section, key);
    }

    int ConfigurationManager::getIntSetting(const ConfigurationSection& section,
        const Ogre::String& key) const
    {
        // Get the value we are looking for
        return Ogre::StringConverter::parseInt(findSetting(section, key));
    }

    bool ConfigurationManager::getBoolSetting(const ConfigurationSection& section,
        const Ogre::String& key) const
    {
        // Get the value we are looking for
        return Ogre::StringConverter::parseBool(findSetting(section, key));
    }

    Ogre::Real ConfigurationManager::getRealSetting(const ConfigurationSection& section,
        const Ogre::String& key) const
    {
        // Get the value we are looking for
        return Ogre::StringConverter::parseReal(findSetting(section, key));
    }

    Ogre::String ConfigurationManager::findSetting(const ConfigurationSection& section,
        const Ogre::String& key) const
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

    void ConfigurationManager::addSetting(const ConfigurationSection& section,
        const Ogre::String& key, const Ogre::String& value)
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

    void ConfigurationManager::addToCfgPath(const Ogre::String& path)
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

    bool ConfigurationManager::checkForFile(const Ogre::String& filename)
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
        catch (fs::filesystem_error&) {
            return false;
        }

        return false;
    }
}
