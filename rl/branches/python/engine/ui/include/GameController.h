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

#ifndef __GameController_H__
#define __GameController_H__

#include "UiPrerequisites.h"
#include "GameTask.h"

#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreSceneQuery.h>
#undef sleep
#include <OgreOde_Core.h>

namespace rl {

    class Actor;
    class MeshObject;

    /**
     *  @todo Kollision
     *  @todo Nachziehen
     */
    class _RlUiExport GameController : public GameTask,
        public OgreOde::CollisionListener
    {
    public:
        typedef enum {VM_THIRD_PERSON, VM_FIRST_PERSON} ViewMode;
    
        GameController();
        
        /** Massgeblich ist die Position des Actors. Die Camera wird hinter diesen
         *  gesetzt.
         *  @throw NullPointerException falls camera oder hero 0 sind.
         */
        GameController(Actor* camera, Actor* hero);
        virtual ~GameController();

        void run(Real elapsedTime);

        Actor* getControlledActor();

        /** Setzt den Actor, der durch den Benutzer zu steuern ist.
         *  Dabei wird die Camera ueber/hinter den Actor gesetzt.
         *  @throw NullPointerException falls actor 0 ist.
         */
        void setControlledActor(Actor* actor);

        Actor* getCamera();
        
        /// First oder Third person view.
        void setViewMode(ViewMode mode);
        ViewMode getViewMode();
        
        /** Setzt die Camera in einen 30-Grad-Winkel dem Helden auf den Hinterkopf
         *  schauend im aktuellen Abstand vom Helden, wie durch den Spieler bestimmt.
         */
        void resetCamera();

		void toggleDebugOde();

        /**
         *  Callback vom CollisionListener
         */
        virtual bool collision(OgreOde::Contact* contact);

    private:
        typedef enum {AS_STAND, AS_WALK_FORWARD} AnimationState;
        Ogre::SceneManager* mSceneManager;
        Ogre::SceneNode* mControlNode;
        Ogre::SceneNode* mLookAtNode;
        Ogre::SceneNode* mCameraNode;
        Actor* mCameraActor;
        Actor* mActor;
        MeshObject* mActorMesh;

        Ogre::Real mMoveScale;
        Ogre::Real mRotScale;

        Ogre::Real mMoveSpeed;
        Ogre::Real mRotSpeed;
        
        Ogre::Real mFallSpeed;

        OgreOde::World* mOdeWorld;
        OgreOde::Geometry* mOdeActor;
        OgreOde::Geometry* mOdeCamera;
        OgreOde::Geometry* mOdeLevel;

        AnimationState mCurrentAnimationState;
        AnimationState mLastAnimationState;
        
        // Die Soll/Ist-Werte
        Radian mMaxPitch;
        Radian mMinPitch;
        Radian mTargetPitch;

        Real mMaxDistance;
        Real mMinDistance;
        Real mTargetDistance;
        Real mDesiredDistance;
        
        ViewMode mViewMode;
        
        void setup();
            
        Ogre::Vector3 ogrePosToOdePos(const Ogre::Vector3& pos,
            const Ogre::Vector3& extent);

        void calculateScalingFactors(Ogre::Real timePassed);

        void calculateActorTranslation(Ogre::Vector3& translation,
            Ogre::Real& yaw);
        void calculateCameraTranslation();
        
        void updateAnimationState(const Ogre::Vector3& translation);
		
		void updatePickedObject() const;
    };

}
#endif
