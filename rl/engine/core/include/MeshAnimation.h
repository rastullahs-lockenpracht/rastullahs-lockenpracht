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

#ifndef __MeshAnimation_H__
#define __MeshAnimation_H__

#include "CorePrerequisites.h"

#include "BaseAnimation.h"
#include <map>
#include <set>

namespace Ogre
{
    class Animation;
}

namespace rl
{

    class MeshObject;

    /**
        Diese Klasse ermoeglicht eine einfache Steuerung von Animationseinstellungen
        und ist die Basisklasse erweiterter Animationen.
        @remarks Instanzen werden ueber den AnimationManager erzeugt
        @see AnimationManager
    */
    class _RlCoreExport MeshAnimation : public BaseAnimation
    {
    public:
        /**	Der Basiskonstruktor, fuer MeshObject, die einen AnimationState mitbringen
            @param animState	AnimationState, intern
            @param speed		Geschwindigkeit, auch negativ
            @param timesToPlay	Abspielanzahl, 0 = unendlich
            @mesh mesh  The MeshObjekt whose Animation is played
            @remarks	Die Animation beginnt sofort zu spielen, bei negativer
                        Geschwindigkeit beginnt sie mit dem letzten Frame. Konstruktor
                        sollte nicht direkt aufgerufen werden, sondern vom AnimationManager.
        */
        MeshAnimation(Ogre::AnimationState* animState, MeshObject* mesh, Ogre::Real speed = 1.0,
            unsigned int timesToPlay = 0, bool paused = false);
        /// Virtueller Destruktor
        virtual ~MeshAnimation();

        MeshObject* getMeshObject();

        virtual Ogre::Real getWeight(void) const;
        virtual void setWeight(Ogre::Real weight);

        virtual void resetTimesPlayed();
        virtual void setSpeed(Ogre::Real speed);

        virtual void doAddTime(Ogre::Real timePassed);

        Ogre::AnimationState* getAnimationState() const;

    protected:
        virtual void setLoop(bool loop);
        /// Der AnimationState
        Ogre::AnimationState* mAnimState;

    private:
        Ogre::Animation* mAnimation;
        /// Das MeshObject
        MeshObject* mMeshObject;

        /// Setzt den AnimationState
        void setAnimationState(Ogre::AnimationState* animState);
    };
}
#endif
