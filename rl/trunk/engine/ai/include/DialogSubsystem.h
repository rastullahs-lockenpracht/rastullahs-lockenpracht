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
#ifndef __DialogSubsystem_H__
#define __DialogSubsystem_H__

#include "DialogPrerequisites.h"

#include <OgreResource.h>
#include <OgreSingleton.h>

//#include "CommonPrerequisites.h"

namespace MadaBot
{
	template <class S> class AimlCore;
}
namespace rl
{
	class DialogCharacter;
	class ContextInterpreter;

	/// Initialises and manages everything for using dialogs in Rl
	class _RlDialogExport DialogSubsystem : 
	//	public AimlCoreComponent,
		public Ogre::Singleton<DialogSubsystem>
	{
	public:
		DialogSubsystem();
		virtual ~DialogSubsystem();
		
		/**
		 * Get the bot with the given name
		 * @return the bot or NULL if the bot does not exist
		 */
		DialogCharacter* getBot(const CeGuiString& botName);

		ContextInterpreter* getContextInterpreter();

		/**
		 * Load a DialogCharacter from a xml file
		 * If no name is given, the first available bot in the xml file is loaded
		 * @param fileName xml file with the bot definition
		 * @param botName name of the bot to load
		 * @return will return a DialogCharacter...
		 */
		DialogCharacter* loadBot(const CeGuiString& botName, const CeGuiString& fileName = "");
	
        /**
         * Helper function, that retrieves an xml resource from its file name
         * If resource not yet created, it gets first searched in the current
         * adventure module group, then in the default group.
         */
        Ogre::ResourcePtr getXmlResource(const Ogre::String& filename);

		/**
		 * Set the DialogCharacter loaded from ruby
		 */
		void setCurrentDialogCharacter(DialogCharacter* bot);
		DialogCharacter* getCurrentDialogCharacter() const;

	private:
		void initialize();
		typedef std::map<CeGuiString, DialogCharacter*> BotMap;
		typedef MadaBot::AimlCore<CeGuiString> AimlCore; 
		BotMap mBots;
		AimlCore* mCore;
		ContextInterpreter* mContextInterpreter;
		DialogCharacter* mCurrentBot;
	};

/*	inline DialogCharacter* DialogSubsystem::getCurrentBot()
	{
		return mCurrentBot;
	}
*/
	inline ContextInterpreter* DialogSubsystem::getContextInterpreter()
	{
		return mContextInterpreter;
	}
}
#endif
