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

#ifndef __DialogSubsystem_H__
#define __DialogSubsystem_H__

#include <OgreSingleton.h>

#include "CommonPrerequisites.h"
#include "DialogPrerequisites.h"
#include "AimlCoreComponent.h"

namespace rl
{
	class DialogCharacter;
	/// Initialises and manages everything for using dialogs in Rl
	class _RlDialogExport DialogSubsystem : 
		public AimlCoreComponent,
		public Ogre::Singleton<DialogSubsystem>
	{
	public:
		static DialogSubsystem & getSingleton(void);
        static DialogSubsystem * getSingletonPtr(void);

		DialogSubsystem();
		virtual ~DialogSubsystem();
		
		/**
		 *
		 */
		DialogCharacter* getCurrentBot();

		/**
		 * Load a DialogCharacter from a xml file
		 * If no name is given, the first available bot in the xml file is loaded
		 * @param fileName xml file with the bot definition
		 * @param botName name of the bot to load
		 * @return will return a DialogCharacter...
		 */
		DialogCharacter* loadBot(const CeGuiString& fileName, const CeGuiString& botName = "");
	
		/**
		 * Logging
		 */
		void log(const Ogre::LogMessageLevel level, const Ogre::String& msg, const Ogre::String& ident = "");

	private:
		typedef std::map<CeGuiString, DialogCharacter*> BotMap;
		BotMap mBots;

		DialogCharacter* mCurrentBot;
	};
}
#endif
