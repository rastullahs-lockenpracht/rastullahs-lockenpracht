#include "ThirdPersonGameController.h"
#include "CoreSubsystem.h"
#include "World.h"
#include "InputManager.h"
#include "DebugWindow.h"
#include "Exception.h"
//BEGIN nur debugkrams, kommt raus
#include "GameActor.h"
#include "ActorFactory.h"
//END nur debugkrams, kommt raus

#include <OgreSceneManager.h>
#include <OgreAxisAlignedBox.h>
#include <OgreStringConverter.h>

using namespace Ogre;

namespace rl {

    ThirdPersonGameController::ThirdPersonGameController(
        Camera* camera,
        Ogre::Entity* hero,
        const Vector3& pos) : mCameraNode(0),
                              mHeroNode(0),
                              mControlNode(0),
                              mLookAtNode(0),
                              mCamera(camera),
                              mHero(hero),
                              mMoveSpeed(60.0),
                              mRotSpeed(100.0),
                              mInputManager(InputManager::getSingletonPtr()),
                              mWalk(false),
                              mWasWalking(false)
    {
        SceneNode* root = CoreSubsystem::getSingleton().getWorld()->
            getSceneManager()->getRootSceneNode();

        mControlNode = root->createChildSceneNode("ControlNode");
        mControlNode->translate(0, 186.0 / 2.54 , 0);
        mLookAtNode = mControlNode->createChildSceneNode("LookAtNode");
        mLookAtNode->pitch(-20);
        mCameraNode = mLookAtNode->createChildSceneNode("CameraNode");
        mCameraNode->attachObject(camera);
        mCameraNode->translate(Vector3::UNIT_Z * 100.0);

        mHeroNode = mControlNode->createChildSceneNode("HeroNode");
        GameActor* actor = dynamic_cast<GameActor*>(
            ActorFactory::getSingleton().createGameActor("Held","held.mesh"));
        mHeroNode->attachObject(actor->getEntity());
        mHeroNode->translate(0, -186.0 / 2.54, 0);
        mHeroNode->setScale(1.0/2.54, 1.0/2.54, 1.0/2.54);

        mControlNode->translate(Matrix3::IDENTITY, pos, Node::TS_PARENT);
    }

    ThirdPersonGameController::~ThirdPersonGameController()
    {
    }

    void ThirdPersonGameController::run(Real elapsedTime)
    {
        calculateScalingFactors(elapsedTime);

        if (!InputManager::getSingleton().isCeguiActive())
        {
            processKeys();
            processMouse();

            mControlNode->yaw(mYaw);
            mLookAtNode->pitch(mPitch);
            mCameraNode->translate(0, 0, mCameraZ);

            mControlNode->translate(mTranslation, Node::TS_LOCAL);
            if (mWalk != mWasWalking)
            {
                
                GameActor* actor = dynamic_cast<GameActor*>(
                    ActorFactory::getSingletonPtr()->getActor("Held"));
                if (mWalk)
                {
                    actor->startAnimation("gehloop");
                }
                else
                {
                    actor->stopAnimation("gehloop");
                }
                mWasWalking = mWalk;
            }
        }

        DebugWindow::getSingletonPtr()->
            setText(" Sphere: " + StringConverter::toString(mControlNode->getPosition())
            + " Cam: " + StringConverter::toString(mCameraNode->getPosition())
            + " Yaw: " + StringConverter::toString(mYaw)
            + " Trans: " + StringConverter::toString(mTranslation));
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

    void ThirdPersonGameController::processMouse()
    {	
        mYaw = -InputManager::getSingleton().getMouseRelativeX() * 0.13;
        mPitch = -InputManager::getSingleton().getMouseRelativeY() * 0.13;
        mCameraZ = -InputManager::getSingleton().getMouseRelativeZ() * 0.05;
    }

    void ThirdPersonGameController::processKeys()
    {
        mTranslation = Vector3::ZERO;
        mWalk = false;

        if (mInputManager->isKeyDown(KC_UP) || mInputManager->isKeyDown(KC_W))
        {
            mTranslation.z = -mMoveScale;
            mWalk = true;
        }

        if (mInputManager->isKeyDown(KC_DOWN) || mInputManager->isKeyDown(KC_S))
        {
            mTranslation.z = mMoveScale;
            mWalk = true;
        }

        if (mInputManager->isKeyDown(KC_RIGHT) || mInputManager->isKeyDown(KC_D))
        {
            mTranslation.x = mMoveScale;
            mWalk = true;
        }

        if (mInputManager->isKeyDown(KC_LEFT) || mInputManager->isKeyDown(KC_A))
        {
            mTranslation.x = -mMoveScale;
            mWalk = true;
        }

        if (mInputManager->isKeyDown(KC_PGUP))
        {
            mTranslation.y = mMoveScale;
            mWalk = true;
        }

        if (mInputManager->isKeyDown(KC_PGDOWN))
        {
            mTranslation.y = -mMoveScale;
            mWalk = true;
        }

        if (mInputManager->isKeyDown(KC_P))
        {
            CoreSubsystem::getSingleton().makeScreenshot("rastullah");
            mWalk = true;
        }

        if (mInputManager->isKeyDown(KC_C))
        {
            mCameraNode->lookAt(mControlNode->getWorldPosition(), Node::TS_LOCAL);
            mWalk = true;
        }

	    if (mInputManager->isKeyDown(Ogre::KC_ESCAPE))
	    {
		    delete &Ogre::Root::getSingleton();
		    exit(0);
	    }
    }
}
