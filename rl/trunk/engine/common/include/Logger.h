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
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "CommonPrerequisites.h"
#include <OgreSingleton.h>
#include <OgreLog.h>
#include <CEGUILogger.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>

namespace fs = boost::filesystem;

namespace rl {
	class _RlCommonExport Logger : public Ogre::Singleton<Logger>
	{
	public:

		static const char* RULES;
		static const char* CORE;
		static const char* DIALOG;
		static const char* AI;
		static const char* UI;
		static const char* MULTIMEDIA;
		static const char* MAIN;
		static const char* SCRIPT;

		enum LogLevel
		{
			LL_DEBUG = 1,
			LL_MESSAGE,
			LL_WARNING,
			LL_ERROR,
			LL_CRITICAL
		};

		static const char* LEVEL_TEXT[6];

		Logger(const Ogre::String& logDirectory, const Ogre::String& ogreLogFile, const Ogre::String& rastullahLogFile);
		virtual ~Logger();

        // Singleton Stuff
        static Logger & getSingleton(void);
        static Logger * getSingletonPtr(void);

		void log(
			const LogLevel level,
			const Ogre::String& component,
			const char* message,
			const Ogre::String& ident = "");

		void log(
			const LogLevel level,
			const Ogre::String& component,
			const Ogre::String& message,
			const Ogre::String& ident = "");

		void log(
			const LogLevel level,
			const Ogre::String& component,
			const CeGuiString& msg,
			const Ogre::String& ident = "");

		void setLogDetail(const LogLevel level);
        const LogLevel& getLogDetail();
		const CEGUI::LoggingLevel getCeGuiLogDetail() const;

		bool isErrorPresent() const;
		void resetErrorState();
		const Ogre::String& getErrorLog() const;
	private:
		void log(const LogLevel level, const Ogre::String& msg );

		Ogre::Log* mLog;
		LogLevel mLogLevel;
		Ogre::String mErrorBuffer;
		bool mErrorPresent;
	};
}

#define RL_LOG2(level, component, msg, ident) \
    if (rl::Logger::getSingletonPtr() != NULL && level >= rl::Logger::getSingleton().getLogDetail()) \
		rl::Logger::getSingleton().log(level, component, msg, ident)

#define RL_LOG(level, component, msg) \
    RL_LOG2(level, component, msg, "")

#define RL_LOG_SHORT(level, msg) \
    RL_LOG2(level, "", msg, "")

#define LOG_DEBUG(component, msg) \
    RL_LOG(rl::Logger::LL_DEBUG, component, msg)
#define LOG_DEBUG2(component, msg, ident) \
    RL_LOG2(rl::Logger::LL_DEBUG, component, msg, ident)
#define LOG_DEBUG_SHORT(msg) \
    RL_LOG_SHORT(rl::Logger::LL_DEBUG, msg)

#define LOG_MESSAGE(component, msg) \
    RL_LOG(rl::Logger::LL_MESSAGE, component, msg)
#define LOG_MESSAGE2(component, msg, ident) \
    RL_LOG2(rl::Logger::LL_MESSAGE, component, msg, ident)
#define LOG_MESSAGE_SHORT(msg) \
    RL_LOG_SHORT(rl::Logger::LL_MESSAGE, msg)

#define LOG_WARNING(component, msg) \
    RL_LOG(rl::Logger::LL_WARNING, component, msg)
#define LOG_WARNING2(component, msg, ident) \
    RL_LOG2(rl::Logger::LL_WARNING, component, msg, ident)
#define LOG_WARNING_SHORT(msg) \
    RL_LOG_SHORT(rl::Logger::LL_WARNING, msg)

#define LOG_ERROR(component, msg) \
    RL_LOG(rl::Logger::LL_ERROR, component, msg)
#define LOG_ERROR2(component, msg, ident) \
    RL_LOG2(rl::Logger::LL_ERROR, component, msg, ident)
#define LOG_ERROR_SHORT(msg) \
    RL_LOG_SHORT(rl::Logger::LL_ERROR, msg)

#define LOG_CRITICAL(component, msg) \
    RL_LOG(rl::Logger::LL_CRITICAL, component, msg)
#define LOG_CRITICAL2(component, msg, ident) \
    RL_LOG2(rl::Logger::LL_CRITICAL, component, msg, ident)
#define LOG_CRITICAL_SHORT(msg) \
    RL_LOG_SHORT(rl::Logger::LL_CRITICAL, msg)

#endif
