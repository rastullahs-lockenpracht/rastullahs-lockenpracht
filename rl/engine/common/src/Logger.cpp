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

#include "Logger.h"

#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

using Ogre::LogManager;
using Ogre::Singleton;

template <> rl::Logger* Singleton<rl::Logger>::msSingleton = 0;
const char* rl::Logger::COMMON = "Common";
const char* rl::Logger::RULES = "Rules";
const char* rl::Logger::CORE = "Core";
const char* rl::Logger::DIALOG = "Dialog";
const char* rl::Logger::AI = "Ai";
const char* rl::Logger::UI = "Ui";
const char* rl::Logger::MULTIMEDIA = "Multimedia";
const char* rl::Logger::MAIN = "Main";
const char* rl::Logger::SCRIPT = "Script";

const char* rl::Logger::LEVEL_TEXT[6] = { "(UD)", "(--)", "(--)", "(WW)", "(EE)", "(CC)" };

namespace rl
{

    Logger::Logger(
        const Ogre::String& logDirectory, const Ogre::String& ogreLogFile, const Ogre::String& rastullahLogFile)
        : mLog(0)
        , mLogLevel(LL_ERROR)
        , mErrorBuffer("")
        , mErrorPresent(false)
    {
        if (LogManager::getSingletonPtr() == 0)
        {
            new LogManager();
        }

        // Check, if logPath exists
        fs::path rastullahLogDirectory(logDirectory);
        if (!fs::exists(rastullahLogDirectory))
        {
            fs::create_directory(rastullahLogDirectory);
        }

        // Log fr Ogre
        LogManager::getSingleton().setDefaultLog(LogManager::getSingleton().createLog(ogreLogFile));

        // Log fr RL
        mLog = LogManager::getSingleton().createLog(rastullahLogFile);
    }

    Logger::~Logger()
    {
        delete LogManager::getSingletonPtr();
    }

    void Logger::log(const Logger::LogLevel level, const Ogre::String& component, const Ogre::String& message,
        const Ogre::String& ident)
    {
        if (ident.length() == 0)
            log(level, "[" + component + "] " + message);
        else
            log(level, "[" + component + "::" + ident + "] " + message);
    }

#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
    void Logger::log(const Logger::LogLevel level, const Ogre::String& component, const CeGuiString& message,
        const Ogre::String& ident)
    {
        if (ident.length() == 0)
            log(level, "[" + component + "] " + message.c_str());
        else
            log(level, "[" + component + "::" + ident + "] " + message.c_str());
    }
#endif

    void Logger::log(
        const Logger::LogLevel level, const Ogre::String& component, const char* message, const Ogre::String& ident)
    {
        if (ident.length() == 0)
            log(level, "[" + component + "] " + message);
        else
            log(level, "[" + component + "::" + ident + "] " + message);
    }

    void Logger::log(const Logger::LogLevel level, const Ogre::String& msg)
    {
        if (level >= mLogLevel)
        {
            mLog->logMessage(LEVEL_TEXT[level] + msg, Ogre::LML_TRIVIAL);

            if (level >= Logger::LL_ERROR) // Fehler
            {
                mErrorBuffer.append("\n");
                mErrorBuffer.append(msg);
                mErrorPresent = true;
            }
        }
    }

    void Logger::setLogDetail(const Logger::LogLevel level)
    {
        mLogLevel = level;
        mLog->setLogDetail(Ogre::LL_BOREME);
        LogManager::getSingleton().setLogDetail(getOgreLogDetail());
    }

    const Logger::LogLevel& Logger::getLogDetail()
    {
        return mLogLevel;
    }

    const CEGUI::LoggingLevel Logger::getCeGuiLogDetail() const
    {
        if (mLogLevel > Logger::LL_ERROR)
            return CEGUI::Errors;
        else if (mLogLevel > Logger::LL_WARNING)
            return CEGUI::Standard;
        else
            return CEGUI::Insane;

        return CEGUI::Errors;
    }

    const Ogre::LoggingLevel Logger::getOgreLogDetail() const
    {
        if (mLogLevel > Logger::LL_ERROR)
            return Ogre::LL_LOW;
        else if (mLogLevel > Logger::LL_WARNING)
            return Ogre::LL_NORMAL;
        else
            return Ogre::LL_BOREME;

        return Ogre::LL_LOW;
    }

    bool Logger::isErrorPresent() const
    {
        return mErrorPresent;
    }

    void Logger::resetErrorState()
    {
        mErrorPresent = false;
    }

    const Ogre::String& Logger::getErrorLog() const
    {
        return mErrorBuffer;
    }

    Ogre::Log* Logger::_getLog() const
    {
        return mLog;
    }
}
