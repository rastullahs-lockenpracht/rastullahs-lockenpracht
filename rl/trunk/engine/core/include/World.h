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

#ifndef __World_H__
#define __World_H__

#include "CorePrerequisites.h"
#include <OgreSceneManager.h>

namespace rl {

    class Actor;

    class _RlCoreExport World
    {
    public:
        enum FogMode { FOG_NONE, FOG_EXP, FOG_EXP2, FOG_LINEAR  };

        World() : mSceneMgr(0), mCamera(0), mActiveActor(0) {}

        /**@todo Klaeren, wer für Zerstoerung von mCamera und mSceneMgr
        *verantwortlich ist. */
        virtual ~World() {}

        virtual Ogre::SceneManager* getSceneManager(void) const;
        virtual void setSceneManager(Ogre::SceneManager* SceneMgr);

        virtual Ogre::Camera* getActiveCamera(void) const { return mCamera; };

        Actor* getActiveActor() const;
        void setActiveActor(Actor* actor);

        virtual Ogre::Vector3 getStartPoint() const;

        virtual void clearScene(void) = 0;
        virtual void loadScene(const Ogre::String& levelName) = 0;

        //Enables / disables a 'sky plane' i.e.
        virtual void setSkyPlane(bool enable, const Ogre::Plane &plane,
            const Ogre::String &materialName,
            Ogre::Real scale=20000, Ogre::Real tiling=10,
            bool drawFirst=true, Ogre::Real bow=0);

        //Enables / disables a 'sky box' i.e.
        virtual void setSkyBox(bool enable, const Ogre::String &materialName,
            Ogre::Real distance=100000, bool drawFirst=true );

        //Enables / disables a 'sky dome' i.e.
        virtual void setSkyDome(bool enable, const Ogre::String &materialName,
            Ogre::Real curvature=10, Ogre::Real tiling=8,
            Ogre::Real distance=80000,
            bool drawFirst=true );

        //Sets the fogging mode applied to the scene.
        virtual void setFog(FogMode mode=FOG_NONE,
            const Ogre::ColourValue &colour=Ogre::ColourValue::White,
            Ogre::Real expDensity=0.001,
            Ogre::Real linearStart=0.0, Ogre::Real linearEnd=1.0);

        //Returns the fog mode for the scene.
        virtual FogMode getFogMode (void) const;

        //Returns the fog colour for the scene.
        virtual const Ogre::ColourValue & getFogColour (void) const;

        //Returns the fog start distance for the scene.
        virtual Ogre::Real getFogStart (void) const;

        //Returns the fog end distance for the scene.
        virtual Ogre::Real getFogEnd (void) const;

        virtual void setAmbientLight(Ogre::Real r, Ogre::Real g, Ogre::Real b);

		virtual void setCastShadows(bool enabled);

    protected:
        World(Ogre::SceneType sceneType);

        virtual void initializeDefaultCamera(void) = 0;	

        Ogre::SceneManager* mSceneMgr;
        Ogre::Camera* mCamera;

        Actor* mActiveActor;
    };

}
#endif
