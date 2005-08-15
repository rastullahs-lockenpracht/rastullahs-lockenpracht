/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Perl Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Perl Artistic License for more details.
*
*  You should have received a copy of the Perl Artistic License
*  along with this program; if not you can get it here
*  http://www.perldoc.com/perl5.6/Artistic.html.
*/
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "CommonPrerequisites.h"
#include <OgreSingleton.h>
#include <OgreLog.h>

namespace rl {
	class _RlCommonExport Logger : public Ogre::Singleton<Logger>
	{
	public:
		Logger(const Ogre::String& logPath, const Ogre::String& ogreLogPath);
		virtual ~Logger();

        // Singleton Stuff
        static Logger & getSingleton(void);
        static Logger * getSingletonPtr(void);

		void log(
			const Ogre::LogMessageLevel level, 
			const Ogre::String& component, 
			const Ogre::String& message, 
			const Ogre::String& ident = "");

		void setLogDetail(const Ogre::LoggingLevel level);
	private:
		void log(const Ogre::LogMessageLevel level, const Ogre::String& msg );

		Ogre::Log* mLog;
	};
}

#endif

