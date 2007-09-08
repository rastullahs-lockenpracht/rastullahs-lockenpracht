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
#ifndef __AiSubsystem_H__
#define __AiSubsystem_H__

#include <OgreSingleton.h>

#include "AiPrerequisites.h"

#include "MessagePump.h"
#include "XmlProcessor.h"
#include "World.h"

namespace MadaBot
{
	template <class S> class AimlCore;
}

namespace rl
{
	class Agent;
	class AgentManager;
	class AiWorld;
	class ContextInterpreter;
	class DialogCharacter;
	class Landmark;
	class LandmarkPath;
	class WayPointGraphManager;


	/** Central core of AI
	 * Handles creation of all AI related object Managers.
	 */
	class _RlAiExport AiSubsystem 
        : public Ogre::Singleton<AiSubsystem>,
        private XmlProcessor
	{
	public:
		//! default constructor
		AiSubsystem();
		/** explicit virtual destructor.
		 * Unregisters Scene listener, makes AgentManager drop all Agents,
		 * unregisters AgentManager from GameLoop,
		 * deletes AgentManager, WayPointGraphManager and AiWorld
		 */
		virtual ~AiSubsystem();
		
		/** Returns the stored world object representing storing obstacles.
		 */
		AiWorld* getWorld();

		/** Trigger function triggered after scene load.
		 */
		virtual bool onAfterSceneLoaded();

		/** Trigger function triggered before scene load.
		 */
        virtual bool onBeforeClearScene();

		/** Creates an named LandmarkPath.
		 * @param name the path's name
		 */
		LandmarkPath* createLandmarkPath(const Ogre::String& name);

		LandmarkPath* getLandmarkPath(const Ogre::String& name) const;

		/** Creates an named Landmark.
		 * @param name the landmark's name
		 */
		Landmark* createLandmark(const Ogre::String& name, const Ogre::Vector3& position);

		Landmark* getLandmark(const Ogre::String& name) const;

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
		typedef std::map<CeGuiString, DialogCharacter*> BotMap;
		typedef MadaBot::AimlCore<CeGuiString> AimlCore; 
		BotMap mBots;
		AimlCore* mCore;
		ContextInterpreter* mContextInterpreter;
		DialogCharacter* mCurrentBot;

        /** Initializes the AI subsystem.
		 * Creates AiWorld and AgentManager, registers a scene listener,
		 * adds the AgentManager to the GameLoop as a task.
		 */
		void initialize();

		/** Removes all registered LandmarkPaths
		 * Clears all internal lists and deallocates the memory of the 
		 * stored objects.
		 */
		void removeAllLandmarkPaths();

		/** Removes all registered Landmarkss
		 * Clears all internal lists and deallocates the memory of the 
		 * stored objects.
		 */
		void removeAllLandmarks();

		//! single AgentManager object
		AgentManager* mAgentManager;
		//! single WayPointGraphManager object
		WayPointGraphManager *mWayPointGraphManager;
		//! AiWorld representation - WIP!!! (object to changes)
		AiWorld* mWorld;

		std::map<Ogre::String, LandmarkPath*> mLandmarkPaths;
		std::map<Ogre::String, Landmark*> mLandmarks;

	    MessagePump::ScopedConnection mSceneLoadedConnection;
	    MessagePump::ScopedConnection mSceneClearingConnection;
	};

	inline ContextInterpreter* AiSubsystem::getContextInterpreter()
	{
		return mContextInterpreter;
	}

    inline AiWorld* AiSubsystem::getWorld()
	{
		return mWorld;
	}
}
#endif
