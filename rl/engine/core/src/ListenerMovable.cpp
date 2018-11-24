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

#include "ListenerMovable.h"

using namespace Ogre;

namespace rl
{

    String ListenerMovable::msMovableType = "ListenerMovable";
    AxisAlignedBox ListenerMovable::msAABox = AxisAlignedBox(-0.5, -0.5, -0.5, 0.5, 0.5, 0.5);

    /**
     * @param name Der Name des Zuhoerers.
     * @author JoSch
     * @date 03-12-2005
     */
    ListenerMovable::ListenerMovable(const String& name)
        : MovableObject()
    {
        mName = name;

        /// Ein paar Standardwerte setzen
        setPosition(Vector3::ZERO);
        setVelocity(Vector3::ZERO);
        setOrientation(Quaternion());
    }

    /**
     * @author JoSch
     * @date 03-12-2005
     */
    ListenerMovable::~ListenerMovable()
    {
    }

    /**
     * @author JoSch
     * @date 03-11-2005
     * @return Den Namen
     */
    const String& ListenerMovable::getName() const
    {
        return mName;
    }

    /**
     * @author JoSch
     * @date 03-11-2005
     * @return Den Objekttypen
     */
    const String& ListenerMovable::getMovableType() const
    {
        return msMovableType;
    }

    /**
     * @author JoSch
     * @date 03-11-2005
     */
    void ListenerMovable::_notifyCurrentCamera(Camera* cam)
    {
        // Brauchen wir nicht zu wissen.
    }

    /**
     * @author JoSch
     * @date 03-11-2005
     */
    const AxisAlignedBox& ListenerMovable::getBoundingBox() const
    {
        return msAABox;
    }

    /**
     * @author JoSch
     * @date 03-11-2005
     */
    Real ListenerMovable::getBoundingRadius(void) const
    {
        return 0.0;
    }

    /**
     * @author JoSch
     * @date 03-11-2005
     */
    void ListenerMovable::_updateRenderQueue(RenderQueue* queue)
    {
        // BListenerrauchen wir nicht
    }

    /**
     * @return Die aktuelle Richtung der Soundquelle (at-Vektor)
     * @author JoSch
     * @date 03-16-2005
     */
    const Quaternion ListenerMovable::getOrientation() const
    {
        return mOrientation;
    }

    /**
     * @param at Die neue Richtung der Soundquelle.
     * @param up Die neue Richtung der Soundquelle.
     * @author JoSch
     * @date 03-16-2005
     */
    void ListenerMovable::setOrientation(const Quaternion& orientation)
    {
        mOrientation = orientation;
    }

    /**
     * @return Die aktuelle Position der Soundquelle
     * @author JoSch
     * @date 03-16-2005
     */
    const Vector3 ListenerMovable::getPosition() const
    {
        return mPosition;
    }

    /**
     * @param position Die neue Position der Soundquelle.
     * @author JoSch
     * @date 03-16-2005
     */
    void ListenerMovable::setPosition(const Vector3& position)
    {
        mPosition = position;
    }

    /**
     * @return Die aktuelle Geschwindigkeit der Soundquelle
     * @author JoSch
     * @date 03-16-2005
     */
    const Vector3 ListenerMovable::getVelocity() const
    {
        return mVelocity;
    }

    /**
     * @param velocity Die neue Geschwindigkeit der Soundquelle.
     * @author JoSch
     * @date 03-16-2005
     */
    void ListenerMovable::setVelocity(const Vector3& velocity)
    {
        mVelocity = velocity;
    }

    /**
     * @return Die aktuelle Lautstaerke der Soundquelle
     * @author JoSch
     * @date 03-16-2005
     */
    const int ListenerMovable::getGain() const
    {
        return mGain;
    }

    /**
     * @param gain Die neue Lautstarke der Soundquelle.
     * @author JoSch
     * @date 03-16-2005
     */
    void ListenerMovable::setGain(const int gain)
    {
        mGain = gain;
    }

    /**
     * @return TRUE, wenn der aktive Zuhoerer.
     * @author JoSch
     * @date 03-16-2005
     */
    bool ListenerMovable::isActive() const
    {
        return mActive;
    }

    void ListenerMovable::setActive(bool active)
    {
        mActive = active;
    }

    void ListenerMovable::visitRenderables(Renderable::Visitor* visitor, bool debugRenderables)
    {
    }
}
