#include "ThirdPersonGameController.h"
#include "CoreSubsystem.h"
#include "World.h"
#include "InputManager.h"
#include "DebugWindow.h"
#include "Exception.h"
#include "GameActor.h"

#include <OgreSceneManager.h>
#include <OgreAxisAlignedBox.h>
#include <OgreStringConverter.h>

using namespace Ogre;

namespace rl {

    ThirdPersonGameController::ThirdPersonGameController(
        Camera* camera, GameActor* actor)
        : mControlNode(0),
        mLookAtNode(0),
        mCamera(camera),
        mActor(actor),
        mMoveScale(0),
        mRotScale(0),
        mMoveSpeed(200.0),
        mRotSpeed(80.0),
        mCurrentAnimationState(AS_STAND),
        mLastAnimationState(AS_STAND)
    {
        if (actor == 0 || camera == 0)
        {
            Throw(NullPointerException, "actor und camera duerfen nicht NULL sein.");
        }

        SceneNode* root = CoreSubsystem::getSingleton().getWorld()->
            getSceneManager()->getRootSceneNode();

        mControlNode = root->createChildSceneNode("ControlNode");

        mLookAtNode = mControlNode->createChildSceneNode("LookAtNode");
        mLookAtNode->pitch(-20);
        
        mCameraNode = mLookAtNode->createChildSceneNode("CameraNode");
        mCameraNode->attachObject(mCamera);
        mCameraNode->translate(Vector3(0, 0, 150), Node::TS_LOCAL);

        setup();
    }

    ThirdPersonGameController::~ThirdPersonGameController()
    {
    }

    void ThirdPersonGameController::run(Real elapsedTime)
    {
        if (!InputManager::getSingleton().isCeguiActive())
        {
            Vector3 translation = Vector3::ZERO;
            Real cameraZ = 0.0;
            Real yaw = 0.0;
            Real pitch = 0.0;

            calculateScalingFactors(elapsedTime);
            calculateCameraTranslation(cameraZ, yaw, pitch);
            calculateHeroTranslation(translation);
            updateAnimationState(translation);

            mControlNode->yaw(yaw);
            mLookAtNode->pitch(pitch);
            mCameraNode->translate(0, 0, cameraZ);

            mControlNode->translate(translation, Node::TS_LOCAL);
        }
    }

    void ThirdPersonGameController::calculateScalingFactors(Real timePassed)
    {
        if (timePassed == 0)
        {
            mMoveScale = 0.5;
            mRotScale = 0.1;
        }
        else
        {
            mMoveScale = mMoveSpeed * timePassed;
            mRotScale = mRotSpeed * timePassed;
        }
    }

    void ThirdPersonGameController::calculateCameraTranslation(
        Ogre::Real& cameraZ, Ogre::Real& yaw, Ogre::Real& pitch)
    {	
        InputManager* im = InputManager::getSingletonPtr();

        cameraZ = -im->getMouseRelativeZ() * 0.05;
        yaw = -im->getMouseRelativeX() * 0.13;
        pitch = -im->getMouseRelativeY() * 0.13;
    }

    void ThirdPersonGameController::calculateHeroTranslation(Ogre::Vector3& translation)
    {
        InputManager* im = InputManager::getSingletonPtr();

        if (im->isKeyDown(KC_UP) || im->isKeyDown(KC_W))
            translation.z = -mMoveScale;

        if (im->isKeyDown(KC_DOWN) || im->isKeyDown(KC_S))
            translation.z = mMoveScale;

        if (im->isKeyDown(KC_RIGHT) || im->isKeyDown(KC_D))
            translation.x = mMoveScale;

        if (im->isKeyDown(KC_LEFT) || im->isKeyDown(KC_A))
            translation.x = -mMoveScale;

        if (im->isKeyDown(KC_PGUP))
            translation.y = mMoveScale;

        if (im->isKeyDown(KC_PGDOWN))
            translation.y = -mMoveScale;

        if (im->isKeyDown(KC_P))
            CoreSubsystem::getSingleton().makeScreenshot("rastullah");

        if (im->isKeyDown(KC_C))
            mCameraNode->lookAt(mControlNode->getWorldPosition(), Node::TS_LOCAL);

        ///@todo das gehoert hier nicht hin.
        if (im->isKeyDown(Ogre::KC_ESCAPE))
        {
            delete &Ogre::Root::getSingleton();
            exit(0);
        }
    }

    void ThirdPersonGameController::updateAnimationState(const Vector3& translation)
    {
        mCurrentAnimationState =
            translation != Vector3::ZERO ? AS_WALK_FORWARD : AS_STAND;

        if (mCurrentAnimationState != mLastAnimationState)
        {
            if (mCurrentAnimationState == AS_WALK_FORWARD)
            {
                mActor->startAnimation("gehloop");
            }
            else
            {
                mActor->stopAnimation("gehloop");
            }
            mLastAnimationState = mCurrentAnimationState;
        }
    }

    GameActor* ThirdPersonGameController::getControlledActor()
    {
        return mActor;
    }

    void ThirdPersonGameController::setControlledActor(GameActor* actor)
    {
        if (actor == 0)
        {
            Throw(NullPointerException, "actor darf nicht NULL sein.");
        }
        mActor = actor;
        setup();
    }

    Ogre::Camera* ThirdPersonGameController::getCamera()
    {
        return mCamera;
    }

    void ThirdPersonGameController::setup()
    {
        if (mActor != 0)
        {
            Vector3 extent = mActor->getExtent();

            SceneNode* root = CoreSubsystem::getSingleton().getWorld()->
                getSceneManager()->getRootSceneNode();
            
            Vector3 pos = mActor->getPosition();

            // ControlNode auf etwa 10% Abstand bezogen auf die Höhe
            // des GameActors bringen.
            pos.y = pos.y + extent.y * 1.2;
            mControlNode->setPosition(pos);
            mControlNode->addChild(mActor->getSceneNode());
            mActor->getSceneNode()->setPosition(Vector3::ZERO);
            mActor->getSceneNode()->translate(
                Vector3(0, -extent.y * 1.2, 0), Node::TS_PARENT);
        }
    }

    void ThirdPersonGameController::setCamera(Ogre::Camera* camera)
    {
        if (camera == 0)
        {
            Throw(NullPointerException, "camera darf nicht NULL sein.");
        }
        mCameraNode->detachObject(mCamera);
        mCameraNode->attachObject(camera);
        mCamera = camera;
    }
}

