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

#include "ParticleSystemObject.h"
#include "Actor.h"
#include "OgreParticleSystemManager.h"

using namespace Ogre;

namespace rl {
	ParticleSystemObject::ParticleSystemObject(const String& name, const String& partSys)
	{
		ParticleSystem* part = ParticleSystemManager::getSingleton().createSystem(name,partSys);
        
		mMovableObject = part;
	}

	ParticleSystem* ParticleSystemObject::getParticleSystem()
	{
		return static_cast<ParticleSystem*>(mMovableObject);
	}

	String ParticleSystemObject::getObjectType()
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
