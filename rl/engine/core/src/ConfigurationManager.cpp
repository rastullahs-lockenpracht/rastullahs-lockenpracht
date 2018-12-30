/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h" //precompiled header

/**
 * \file ConfigurationManager.h
 *
 * Implementation of the ConfigurationManager subsystem
 */

#include "ConfigurationManager.h"
#include "CoreSubsystem.h"

#include <iostream>

template <> rl::ConfigurationManager* Ogre::Singleton<rl::ConfigurationManager>::msSingleton = 0;

namespace rl
{
    ConfigurationManager::ConfigurationManager()
    {
        // Filenames for log and configuration files
        mExecutablePath = "./";
        mRastullahCfgFile = "rastullah.conf";
        mOgreLogFile = "ogre.log";
        mCeguiLogFile = "cegui.log";
        mRastullahLogFile = "rastullah.log";
        mModulesCfgFile = "modules.cfg";

        // Setup general default values
        addSetting("General", "Log Level", "2");
        addSetting("General", "Subtitle Speed", "0.0");
        addSetting("General", "Save Console History", "50");

        // Setup sound default values
        addSetting("Sound", "DefaultMusicVolume", "0.4");
        addSetting("Sound", "DefaultSoundVolume", "0.9");
        addSetting("Sound", "MasterVolume", "1");

        // Setup graphic default values
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        addSetting("Video", "Render System", "Direct3D9 Rendering Subsystem");
#else
        addSetting("Video", "Render System", "OpenGL Rendering Subsystem");
        addSetting("Video", "RTT Preferred Mode", "FBO");
#endif
        addSetting("Video", "Fullscreen", "no");
        addSetting("Video", "FSAA", "0");
        addSetting("Video", "Video Mode", "800 x 600");
        addSetting("Video", "Use Static Geometry", "auto");
        addSetting("Video", "Max Anisotropy", "1");
        addSetting("Video", "Cast Shadows", "yes");
        addSetting("Video", "Shadow Texture Size", "512");

        // Setup input default values
        addSetting("Input", "Mouse Sensitivity", "4");
        addSetting("Input", "Mouse Invert", "no");

        addSetting("Movement keys", "turn_left", "Q");
        addSetting("Movement keys", "turn_right", "E");
        addSetting("Movement keys", "move_left", "A");
        addSetting("Movement keys", "move_right", "D");
        addSetting("Movement keys", "move_forward", "W");
        addSetting("Movement keys", "move_backward", "S");
        addSetting("Movement keys", "jump", "Leertaste");
        addSetting("Movement keys", "run", "Umschalt links");
        addSetting("Movement keys", "sneak", "Strg links");
        addSetting("Movement keys", "run_lock", "< | >");
        addSetting("Movement keys", "rear_view", "T");

        addSetting("Action keys", "quitgame", "Ctrl+X");
        addSetting("Action keys", "closecurrentwindow", "Escape");
        addSetting("Action keys", "toggleconsole", "Tabulator");
        addSetting("Action keys", "toggledebugwindow", "F2");
        addSetting("Action keys", "togglegamelogwindow", "F3");
        addSetting("Action keys", "makescreenshot", "P,Ctrl+P");
        addSetting("Action keys", "togglecollisiondebug", "L,Ctrl+L");
        addSetting("Action keys", "toggledebugvisualisation", "Ctrl+O");
        addSetting("Action keys", "usecurrentobjectdefaultaction", "R");
        addSetting("Action keys", "toggleinventorywindow", "I");
        addSetting("Action keys", "showjournalwindow", "J");
        addSetting("Action keys", "showcharactersheet", "C");
        addSetting("Action keys", "togglecharacterstatewindow", "O");
        addSetting("Action keys", "togglecharacterselectionwindow", "Y");
        addSetting("Action keys", "toggleingameglobalmenu", "F10");
        addSetting("Action keys", "shownextdebugwindowpage", "Ctrl+M");
        addSetting("Action keys", "showobjectactions", "U");
        addSetting("Action keys", "inventoryshowworldobjects", "Strg links");

        addSetting("FreeflightController keys", "back_to_character_movement", "M");
        addSetting("FreeflightController keys", "toggle_camera_collision", "F");

        addSetting("MovementController keys", "freeflight_mode", "M");
        addSetting("MovementController keys", "toggle_view_mode", "F");
        addSetting("MovementController keys", "reset_camera", "0 (Nummernblock)");

        addSetting("Localization", "language", "de");
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

    Ogre::NameValuePairList* ConfigurationManager::getVideoSettings() const
    {
        return new Ogre::NameValuePairList(getSettings("Video"));
    }

    Ogre::NameValuePairList ConfigurationManager::getSettings(const Ogre::String& section) const
    {
        SectionMap::const_iterator it = mSettings.find(section);
        if (it != mSettings.end())
        {
            return it->second;
        }
        else
        {
            return Ogre::NameValuePairList();
        }
    }

    Ogre::StringVector ConfigurationManager::getPluginList() const
    {
        return mPluginList;
    }

    Ogre::StringVector ConfigurationManager::getModuleList() const
    {
        return mModuleList;
    }

    void ConfigurationManager::removeModules(const Ogre::StringVector& modules)
    {
        for (Ogre::StringVector::const_iterator it = modules.begin(), end = modules.end(); it != end; ++it)
        {
            for (Ogre::StringVector::iterator itDel = mModuleList.begin(), endDel = mModuleList.end(); itDel != endDel;
                 ++itDel)
            {
                if (*itDel == *it)
                {
                    mModuleList.erase(itDel);
                    break;
                }
            }
        }
    }

    void ConfigurationManager::loadConfig()
    {
        setRastullahCfgPath();
        // On Linux, we create the .rastullah directory
#if OGRE_PLATFORM != OGRE_PLATFORM_WIN32
        fs::path rastullahCfgDirectory(Ogre::String(::getenv("HOME")) + "/.rastullah");

        if (!fs::exists(rastullahCfgDirectory))
        {
            fs::create_directory(rastullahCfgDirectory);
        }
#endif

        // First check, if we have found any directory
        if (!mRastullahCfgPath.empty())
        {
            ConfigFile* configfile = new ConfigFile();

            // Split the path at the ':' character
            Ogre::StringVector cfg_paths = Ogre::StringUtil::split(mRastullahCfgPath, ":");

            for (unsigned int i = 0; i < cfg_paths.size(); i++)
            {
                try
                {
                    std::cout << "Loading Configuration File " << cfg_paths[i] << mRastullahCfgFile << std::endl;
                    configfile->load(cfg_paths[i] + mRastullahCfgFile, "=", true);
                }
                catch (Ogre::Exception)
                {
                    LOG_ERROR2(Logger::CORE,
                        "Konnte Rastullah-Konfiguration in '" + cfg_paths[i]
                            + "' nicht laden! Defaulteinstellungen werden benutzt.",
                        "Configuration");
                }

                try
                {
                    for (ConfigFile::SectionIterator it = configfile->getSectionIterator(); it.hasMoreElements();
                         it.moveNext())
                    {
                        Ogre::String sectionName = it.peekNextKey();
                        Ogre::NameValuePairList settings = mSettings[sectionName];
                        for (ConfigFile::SettingsIterator it = configfile->getSettingsIterator(sectionName);
                             it.hasMoreElements(); it.moveNext())
                        {
                            settings[it.peekNextKey()] = it.peekNextValue();
                        }
                        mSettings[sectionName] = settings;
                    }
                }
                catch (Ogre::Exception&)
                {
                    // Log it as information
                    LOG_WARNING(Logger::CORE,
                        "Configuration File " + cfg_paths[i] + mRastullahCfgFile + " - Error on parsing.");
                }
            }

            delete configfile;
        }

        mRastullahLogDirectory = findSetting("Paths", "LogDirectory");
        mModulesRootDirectory = findSetting("Paths", "ModulesDirectory");
        mOgrePluginDirectory = findSetting("Paths", "OgrePluginDirectory");

        if (mRastullahLogDirectory.empty())
        {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            mRastullahLogDirectory = "./logs";
#else
            mRastullahLogDirectory = Ogre::String(::getenv("HOME")) + "/.rastullah/logs";
#endif
        }

        if (mModulesRootDirectory.empty())
        {
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
            mModulesRootDirectory = RL_MODULEDIR;
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            mModulesRootDirectory = mExecutablePath + "/../Resources/modules";
#else
            mModulesRootDirectory = "./modules";
#endif
        }

        if (mOgrePluginDirectory.empty())
        {
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
            mOgrePluginDirectory = OGRE_PLUGINDIR;
#else
            mOgrePluginDirectory = ".";
#endif
        }

        // Plugin list for OGRE specific to operating system
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        addPlugin("RenderSystem_Direct3D9", mOgrePluginDirectory);
#endif
        addPlugin("RenderSystem_GL", mOgrePluginDirectory);
        addPlugin("Plugin_ParticleFX", mOgrePluginDirectory);
        addPlugin("Plugin_OctreeSceneManager", mOgrePluginDirectory);
        addPlugin("Plugin_CgProgramManager", mOgrePluginDirectory);
    }

    void ConfigurationManager::loadModulesConfig()
    {
        // Load the module list
        ConfigFile* configfile = new ConfigFile();
        configfile->load(fs::path(mModulesRootDirectory + "/" + mModulesCfgFile).generic_string());

        ConfigFile::SettingsIterator it = configfile->getSettingsIterator();
        for (ConfigFile::SettingsIterator it = configfile->getSettingsIterator(); it.hasMoreElements(); it.moveNext())
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
        ConfigFile* cfgfile = OGRE_NEW ConfigFile();

        for (SectionMap::const_iterator it = mSettings.begin(); it != mSettings.end(); ++it)
        {
            cfgfile->addSection(it->first, it->second);
        }

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        cfgfile->save(fs::path("./modules/config/" + mRastullahCfgFile).native_file_string());
#else
        cfgfile->save(Ogre::String(::getenv("HOME")) + "/.rastullah/" + mRastullahCfgFile);
#endif

        OGRE_DELETE cfgfile;
    }

    Logger::LogLevel ConfigurationManager::getLogLevel() const
    {
        return static_cast<Logger::LogLevel>(getIntSetting("General", "Log Level"));
    }

    Ogre::String ConfigurationManager::getStringSetting(const Ogre::String& section, const Ogre::String& key) const
    {
        // Get the value we are looking for
        return findSetting(section, key);
    }

    int ConfigurationManager::getIntSetting(const Ogre::String& section, const Ogre::String& key) const
    {
        // Get the value we are looking for
        return Ogre::StringConverter::parseInt(findSetting(section, key));
    }

    bool ConfigurationManager::getBoolSetting(const Ogre::String& section, const Ogre::String& key) const
    {
        // Get the value we are looking for
        return Ogre::StringConverter::parseBool(findSetting(section, key));
    }

    Ogre::Real ConfigurationManager::getRealSetting(const Ogre::String& section, const Ogre::String& key) const
    {
        // Get the value we are looking for
        return Ogre::StringConverter::parseReal(findSetting(section, key));
    }

    Ogre::String ConfigurationManager::findSetting(const Ogre::String& section, const Ogre::String& key) const
    {
        SectionMap::const_iterator sectionIt = mSettings.find(section);
        if (sectionIt != mSettings.end())
        {
            Ogre::NameValuePairList::const_iterator it = sectionIt->second.find(key);

            if (it != sectionIt->second.end())
            {
                return it->second;
            }
        }
        return "";
    }

    void ConfigurationManager::addSetting(
        const Ogre::String& section, const Ogre::String& key, const Ogre::String& value)
    {
        mSettings[section][key] = value;
    }

    Ogre::String ConfigurationManager::getKeymap() const
    {
        return Ogre::String("config/keymap-german.xml");
    }

    void ConfigurationManager::setExecutable(const Ogre::String& path)
    {
        fs::path exeAbsolute(path);
        mExecutablePath = exeAbsolute.branch_path().string();
        std::cout << "ConfigurationManager "
                  << "Executable is " << path << " " << mExecutablePath;
    }

    const Ogre::String& ConfigurationManager::getExecutablePath() const
    {
        return mExecutablePath;
    }

    void ConfigurationManager::setRastullahCfgPath()
    {
        // First try: Current directory
        addToCfgPath("./");

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        addToCfgPath(mExecutablePath + "/../Resources/modules/config/");
#endif
#if OGRE_PLATFORM != OGRE_PLATFORM_WIN32
        // Check system wide /etc/rastullah directory
        addToCfgPath("/etc/rastullah/");
        // Check home .rastullah directory
        addToCfgPath(Ogre::String(::getenv("HOME")) + "/.rastullah/");
#endif
        addToCfgPath("./modules/config/");
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

            std::cout << "Adding path " << path << " to Rastullah configuration path." << std::endl;
        }
    }

    bool ConfigurationManager::checkForFile(const Ogre::String& filename)
    {
        try
        {
            std::cout << "Checking for " << fs::complete(filename).string() << std::endl;
            return fs::exists(filename);
        }
        catch (fs::filesystem_error&)
        {
            return false;
        }

        return false;
    }

    void ConfigurationManager::addPlugin(const Ogre::String& plugin, const Ogre::String& pluginDir)
    {
        // In Windows Ogre now uses a _d suffix for debug plugins. But client application is
        // responsible for loading the proper vesion.
        Ogre::String pluginSuffix = "";

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        Ogre::String dirSeparator = "\\";
#ifdef _DEBUG
        pluginSuffix = "_d";
#endif
#else
        Ogre::String dirSeparator = "/";
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        mPluginList.push_back(plugin + pluginSuffix);
#else
        mPluginList.push_back(pluginDir + dirSeparator + plugin + pluginSuffix);
#endif
    }
}
