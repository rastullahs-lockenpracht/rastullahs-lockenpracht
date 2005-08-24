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

#ifndef __LightObject_H__
#define __LightObject_H__

#include "CorePrerequisites.h"
#include "ActorControlledObject.h"

#include <OgreLight.h>

namespace rl {

    class _RlCoreExport LightObject : public ActorControlledObject
    {
    public:
		/// Defines the type of light
		enum LightTypes { LT_POINT, LT_DIRECTIONAL, LT_SPOTLIGHT	};

        LightObject(const Ogre::String& name, rl::LightObject::LightTypes type);

        /// Wie ActorControlledObject::getMovableObject()
        /// Nur schon gebrauchsfertig gecastet.
        Ogre::Light* getLight();

        virtual Ogre::String getObjectType();
        
        void setAttenuation(Ogre::Real range, Ogre::Real constant,
            Ogre::Real linear, Ogre::Real quadric);
        void setDiffuseColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue);
        void setDirection(Ogre::Real x, Ogre::Real y, Ogre::Real z);
        void setSpecularColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue);
		void setSpotlightRange( Ogre::Real innerAngle, Ogre::Real outerAngle,
			Ogre::Real falloff = 1.0);
		void setCastShadows( bool cast );
    };
}
#endif
