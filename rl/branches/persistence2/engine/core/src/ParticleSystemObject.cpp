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

#include "ParticleSystemObject.h"


#include "Actor.h"
#include "CoreSubsystem.h"
#include "World.h"


using namespace Ogre;

namespace rl {
	ParticleSystemObject::ParticleSystemObject(const String& name, const String& partSys)
	{
		ParticleSystem* part = CoreSubsystem::getSingletonPtr()->getWorld()
				->getSceneManager()->createParticleSystem(name,partSys);

		mMovableObject = part;
	}

	ParticleSystemObject::~ParticleSystemObject()
	{
		CoreSubsystem::getSingletonPtr()->getWorld()
				->getSceneManager()->destroyParticleSystem(getParticleSystem());
	}

	ParticleSystem* ParticleSystemObject::getParticleSystem() const
	{
		return static_cast<ParticleSystem*>(mMovableObject);
	}

	String ParticleSystemObject::getObjectType() const
	{
		return "ParticleSystemObject";
	}

	void ParticleSystemObject::setActive(bool active)
	{
		ParticleSystem* part = getParticleSystem();
		unsigned int numEmitters = part->getNumEmitters();
		for (unsigned int emitterIdx = 0; emitterIdx < numEmitters; emitterIdx++)
		{
			part->getEmitter(emitterIdx)->setEnabled(active);
		}
	}
}
