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

#include "LightObject.h"
#include "Actor.h"
#include "CoreSubsystem.h"
#include "World.h"

using namespace Ogre;

namespace rl {
	LightObject::LightObject(const String& name, rl::LightObject::LightTypes type)
    {
        Light* light = CoreSubsystem::getSingleton().getWorld()->
            getSceneManager()->createLight(name);
		light->setType(Ogre::Light::LightTypes(type) );
        mMovableObject = light;
    }

    Light* LightObject::getLight() const
    {
        return static_cast<Light*>(mMovableObject);
    }

    String LightObject::getObjectType() const
    {
        return "LightObject";
    }

    void LightObject::setAttenuation( Real range, Real constant, Real linear,
        Real quadric)
    {
        getLight()->setAttenuation(range, constant, linear, quadric);
    }

    void LightObject::setDiffuseColour(Real red, Real green, Real blue)
    {
        getLight()->setDiffuseColour(red,green,blue);
    }

    void LightObject::setDiffuseColour(const Ogre::ColourValue& colour)
    {
        getLight()->setDiffuseColour(colour);
    }

    Ogre::ColourValue LightObject::getDiffuseColour() const
    {
        return getLight()->getDiffuseColour();
    }

    void LightObject::setDirection( Real x, Real y, Real z)
    {
        getLight()->setDirection(x,y,z);
    }

    void LightObject::setDirection(const Vector3& direction)
    {
        getLight()->setDirection(direction);
    }

    void LightObject::setSpecularColour(const Ogre::ColourValue& colour)
    {
        getLight()->setSpecularColour(colour);
    }

    void LightObject::setSpecularColour(Real red, Real green, Real blue)
    {
        getLight()->setSpecularColour(red,green,blue);
    }

    Ogre::ColourValue LightObject::getSpecularColour() const
    {
        return getLight()->getSpecularColour();
    }

	void LightObject::setSpotlightRange( Real innerAngle, Real outerAngle,
		Real falloff )
	{
		getLight()->setSpotlightRange(Angle(innerAngle), Angle(outerAngle), falloff );
	}

	void LightObject::setCastShadows( bool cast )
	{
		getLight()->setCastShadows( cast );
	}
}
