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

#ifndef __GameController_H__
#define __GameController_H__

#include "UiPrerequisites.h"
#include "SynchronizedTask.h"

#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreSceneQuery.h>
#undef sleep
#include <OgreOde.h>

namespace rl {

    class GameActor;

    /**
     *  @todo Kollision
     *  @todo Nachziehen
     */
    class _RlUiExport GameController : public SynchronizedTask,
        public OgreOde::CollisionListener
    {
    public:
        typedef enum {VM_THIRD_PERSON, VM_FIRST_PERSON} ViewMode;
    
        GameController();
        
        /** Massgeblich ist die Position des Actors. Die Camera wird hinter diesen
         *  gesetzt.
         *  @throw NullPointerException falls camera oder hero 0 sind.
         */
        GameController(Ogre::Camera* camera, GameActor* hero);
        virtual ~GameController();

        void run(Real elapsedTime);

        GameActor* getControlledActor();

        /** Setzt den Actor, der durch den Benutzer zu steuern ist.
         *  Dabei wird die Camera ueber/hinter den Actor gesetzt.
         *  @throw NullPointerException falls actor 0 ist.
         */
        void setControlledActor(GameActor* actor);

        Ogre::Camera* getCamera();

        /** Setzt die Camera, durch die man den Actor steuert.
         *  Dabei wird die Camera ueber/hinter den Actor gesetzt.
         *  @throw NullPointerException falls camera 0 ist.
         */
        void setCamera(Ogre::Camera* camera);
        
        /// First oder Third person view.
        void setViewMode(ViewMode mode);
        ViewMode getViewMode();
        
        /** Setzt die Camera in einen 30-Grad-Winkel dem Helden auf den Hinterkopf
         *  schauend im aktuellen Abstand vom Helden, wie durch den Spieler bestimmt.
         */
        void resetCamera();

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
        Ogre::Camera* mCamera;
        GameActor* mActor;

        Ogre::Real mMoveScale;
        Ogre::Real mRotScale;

        Ogre::Real mMoveSpeed;
        Ogre::Real mRotSpeed;
        
        Ogre::Real mFallSpeed;

        OgreOde::World* mOdeWorld;
        OgreOde::Stepper* mOdeStepper;
        OgreOde::CapsuleGeometry* mOdeActor;
        OgreOde::SphereGeometry* mOdeCamera;
        OgreOde::RayGeometry* mOdeActorRay;
        OgreOde::TriangleMeshGeometry* mOdeLevel;

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
        void setupCollisionDetection();

        Ogre::Vector3 ogrePosToOdePos(const Ogre::Vector3& pos,
            const Ogre::Vector3& extent);

        void translate(const Vector3& translation,
            Ogre::Node::TransformSpace ts);
        void setPosition(const Vector3& position);
        
        bool detectCollision(const Ogre::Vector3& translation);
        void calculateScalingFactors(Ogre::Real timePassed);

        void calculateHeroTranslation(Ogre::Vector3& translation,
            Ogre::Real& yaw);
        void calculateCameraTranslation();
        void updateAnimationState(const Ogre::Vector3& translation);
		void updatePickedObject() const;
		void adjustCamera(OgreOde::Contact* contact);
    };

}
#endif
