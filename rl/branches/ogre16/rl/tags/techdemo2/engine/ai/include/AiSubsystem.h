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

	class _RlAiExport AiSubsystem 
		: public Ogre::Singleton<AiSubsystem>,
		  public SceneChangeListener
	{
	public:
		static AiSubsystem & getSingleton(void);
        static AiSubsystem * getSingletonPtr(void);

		AiSubsystem(void);
		virtual ~AiSubsystem(void);
		
		AiWorld* getWorld();

		void onAfterSceneLoaded();
        void onBeforeClearScene();
	private:
		void initialize();
		AgentManager* mAgentManager;
		AiWorld* mWorld;
	};

	inline AiWorld* AiSubsystem::getWorld()
	{
		return mWorld;
	}
}
#endif
