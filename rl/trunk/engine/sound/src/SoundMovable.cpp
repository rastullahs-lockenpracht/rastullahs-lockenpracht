/* SoundMovable.cpp - Diese Klassse kapselt einen Sound.
 * (C) 2004. Team Pantheon. www.team-pantheon.de
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
#include "SoundMovable.h"
#include "SoundManager.h"
#include "SoundResource.h"

using namespace std;
using namespace Ogre;

namespace rl {
 
/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 03-12-2005
 */
SoundMovable::SoundMovable(const String &name):
        MovableObject()
{
    mName = name;
    mSound = SoundResourcePtr(dynamic_cast<SoundResource*>
        (SoundManager::getSingleton().getByName(name).getPointer()));
}
 
/**
 * @author JoSch
 * @date 03-12-2005
 */
SoundMovable::~SoundMovable()
{
}

/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Namen
 */
const String& SoundMovable::getName() const
{
    return mName;
}


/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& SoundMovable::getMovableType() const
{
    return "SoundMovable";
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
void SoundMovable::_notifyCurrentCamera(Camera *cam)
{
    // Brauchen wir nicht zu wissen.
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
const AxisAlignedBox& SoundMovable::getBoundingBox() const 
{
    return AxisAlignedBox();
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
Real SoundMovable::getBoundingRadius (void) const
{
    return 0.0;
}

/**
 * @author JoSch
 * @date 03-11-2005
 */
void SoundMovable::_updateRenderQueue(RenderQueue *queue)
{
    // Brauchen wir nicht
}

/**
 * @author JoSch
 * @date 03-12-2005
 */
SoundResourcePtr SoundMovable::getSound() const
{
    return mSound;
}

}
