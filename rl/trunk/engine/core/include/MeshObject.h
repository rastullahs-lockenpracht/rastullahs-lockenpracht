/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __MeshObject_H__
#define __MeshObject_H__

#include "CorePrerequisites.h"
#include "ActorControlledObject.h"

#include <OgreEntity.h>

namespace rl {

	class RlAnimation;

    class _RlCoreExport MeshObject : public ActorControlledObject
    {
    public:
        MeshObject(const Ogre::String& name, const Ogre::String& meshname);
        
        /// Wie ActorControlledObject::getMovableObject()
        /// Nur schon gebrauchsfertig gecastet.
        Ogre::Entity* getEntity() const;
        
        /// Groesse der Boundingbox
        Ogre::Vector3 getSize();
        Ogre::Vector3 getCenter();
        Ogre::Real getRadius();
        Ogre::Real getHeight();
        
		RlAnimation* getAnimation(const Ogre::String& animName) const;
		RlAnimation* startAnimation(const Ogre::String& animName, 
									Real speed, unsigned int timesToPlay);
        void stopAnimation(const Ogre::String&);
        
        virtual Ogre::String getType();
    private:
        Vector3 mSize;
        void calculateSize();
    };
}
#endif
