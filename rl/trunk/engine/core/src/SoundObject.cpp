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

#include "SoundObject.h"
#include "Actor.h"
#include "SoundResource.h"
#include "SoundManager.h"

using namespace Ogre;

namespace rl {
    SoundObject::SoundObject(const String &name) : ActorControlledObject()
    {
        mMovableObject = dynamic_cast<SoundResource*>(
                SoundManager::getSingleton().getByName(name).getPointer());
    }
    
    SoundObject::~SoundObject()
    {
    }
    
	bool SoundObject::isMeshObject()
	{
		return false;
	}
	
	void SoundObject::_update()
	{
        ActorControlledObject::_update();
        SoundResource *sound = getSound();
        Actor *actor = getActor();
        if (!sound || !actor) // Einer ist Null
        {
            return;
        }
        sound->setPosition(actor->getPosition());
        Vector3 *temp1 = new Vector3();
        Vector3 *temp2 = new Vector3(actor->getPosition());
        Real length = temp2->normalise();
        actor->getOrientation().ToAxes(temp1);
        *temp1 += *temp2;
        *temp1 *= length;
        sound->setDirection(*temp1);
	}
    
    SoundResource* SoundObject::getSound()
    {
        return reinterpret_cast<SoundResource*>(mMovableObject);
    }

    String SoundObject::getObjectType()
    {
        return "SoundObject";
    }
}
