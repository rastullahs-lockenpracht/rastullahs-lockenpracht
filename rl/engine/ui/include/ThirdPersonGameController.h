#ifndef __ThirdPersonGameController_H__
#define __ThirdPersonGameController_H__

#include "UiPrerequisites.h"
#include "SynchronizedTask.h"

#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreSceneNode.h>

namespace rl {

    class InputManager;

    /**
     *  @todo auf Actor umstellen statt Ogre::Entity
     *  @todo Kollision
     *  @todo Nachziehen
     */
    class _RlUiExport ThirdPersonGameController : public SynchronizedTask
    {
    public:
        ThirdPersonGameController(Ogre::Camera* camera, Ogre::Entity* hero,
            const Ogre::Vector3& pos = Ogre::Vector3::ZERO);
        virtual ~ThirdPersonGameController();

        void run(Real elapsedTime);

    private:
        Ogre::SceneNode* mControlNode;
        Ogre::SceneNode* mLookAtNode;
        Ogre::SceneNode* mCameraNode;
        Ogre::SceneNode* mHeroNode;

        InputManager* mInputManager;

        Ogre::Camera* mCamera;
        Ogre::Entity* mHero;
        Ogre::Entity* mLooki;

        Ogre::Real mMoveScale;
        Ogre::Real mRotScale;

        Ogre::Real mMoveSpeed;
        Ogre::Real mRotSpeed;

        Ogre::Real mYaw;
        Ogre::Real mPitch;
        Ogre::Real mCameraZ;
        Ogre::Vector3 mTranslation;

        Ogre::Real mCameraDistance;

        bool mWalk;
        bool mWasWalking;

        void calculateScalingFactors(Ogre::Real timePassed);
        void processMouse();
        void processKeys();
    };

}
#endif
