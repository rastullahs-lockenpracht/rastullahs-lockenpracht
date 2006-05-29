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
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "CommonPrerequisites.h"
#include <OgreSingleton.h>
#include <OgreLog.h>
#include <CEGUILogger.h>

namespace rl {
	class _RlCommonExport Logger : public Ogre::Singleton<Logger>
	{
	public:

		static const char* RULES;
		static const char* CORE;
		static const char* DIALOG;
		static const char* UI;
		static const char* MULTIMEDIA;
		static const char* MAIN;
		static const char* SCRIPT;

		enum LogLevel
		{
			LL_TRIVIAL = 1,
			LL_MESSAGE,
			LL_NORMAL,
			LL_ERROR,
			LL_CRITICAL
		};

		Logger(const Ogre::String& logPath, const Ogre::String& ogreLogPath);
		virtual ~Logger();

        // Singleton Stuff
        static Logger & getSingleton(void);
        static Logger * getSingletonPtr(void);

		void log(
			const Ogre::String& component, 
			const LogLevel level, 
			const char* message, 
			const Ogre::String& ident = "");

		void log(
			const Ogre::String& component, 
			const LogLevel level, 
			const Ogre::String& message, 
			const Ogre::String& ident = "");

		void log(
			const Ogre::String& component, 
			const LogLevel level, 			
			const CeGuiString& msg, 
			const Ogre::String& ident = "");
	
		void setLogDetail(const LogLevel level);
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

#endif
