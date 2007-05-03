/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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
#ifndef __AiSubsystem_H__
#define __AiSubsystem_H__

#include <OgreSingleton.h>
#include "AiPrerequisites.h"
#include "World.h"

namespace rl
{
	class Agent;
	class AgentManager;
	class AiWorld;
	class Landmark;
	class LandmarkPath;
	class WayPointGraphManager;


	/** Central core of AI
	 * Handles creation of all AI related object Managers.
	 */
	class _RlAiExport AiSubsystem 
		: public Ogre::Singleton<AiSubsystem>,
		  public SceneChangeListener
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
		virtual void onAfterSceneLoaded();

		/** Trigger function triggered before scene load.
		 */
        virtual void onBeforeClearScene();

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

	private:
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
	};

	inline AiWorld* AiSubsystem::getWorld()
	{
		return mWorld;
	}
}
#endif
