#ifndef __ThirdPersonGameController_H__
#define __ThirdPersonGameController_H__

#include "UiPrerequisites.h"
#include "SynchronizedTask.h"

#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreSceneNode.h>

namespace rl {

    class GameActor;

    /**
     *  @todo auf Actor umstellen statt Ogre::Entity
     *  @todo Kollision
     *  @todo Nachziehen
     *  @todo "Pluggablility"
     */
    class _RlUiExport ThirdPersonGameController : public SynchronizedTask
    {
    public:
        /** Massgeblich ist die Position des Actors. Die Camera wird hinter diesen
         *  gesetzt.
         *  @throw NullPointerException falls camera oder hero 0 sind.
         */
        ThirdPersonGameController(Ogre::Camera* camera, GameActor* hero);
        virtual ~ThirdPersonGameController();

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

    private:
        typedef enum {AS_STAND, AS_WALK_FORWARD} AnimationState;
        Ogre::SceneNode* mControlNode;
        Ogre::SceneNode* mLookAtNode;
        Ogre::SceneNode* mCameraNode;
        Ogre::Camera* mCamera;
        GameActor* mActor;

        Ogre::Real mMoveScale;
        Ogre::Real mRotScale;

        Ogre::Real mMoveSpeed;
        Ogre::Real mRotSpeed;

        AnimationState mCurrentAnimationState;
        AnimationState mLastAnimationState;

        void setup();

        void calculateScalingFactors(Ogre::Real timePassed);

        void calculateHeroTranslation(Ogre::Vector3& translation);
        void calculateCameraTranslation(Ogre::Real& cameraZ,
            Ogre::Real& yaw, Ogre::Real& pitch);
        void updateAnimationState(const Ogre::Vector3& translation);
    };

}
#endif
