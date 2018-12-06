/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright © 2000-2003 The OGRE Team
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
LGPL like the rest of the engine.
-----------------------------------------------------------------------------
*/
/*
-----------------------------------------------------------------------------
Filename:    ExampleFrameListener.h
Description: Defines an example frame listener which responds to frame events.
This frame listener just moves a specified camera around based on
keyboard and mouse movements.
Mouse:    Freelook
W or Up:  Forward
S or Down:Backward
A:        Step left
D:        Step right
             PgUp:     Move upwards
             PgDown:   Move downwards
             F:        Toggle frame rate stats on/off
             R:        Render mode
             T:        Cycle texture filtering
                       Bilinear, Trilinear, Anisotropic(8)
             P:        Toggle on/off display of camera position / orientation
-----------------------------------------------------------------------------
*/

#ifndef __ExampleFrameListener_H__
#define __ExampleFrameListener_H__

#include "Ogre.h"
#include "OgreEventListeners.h"
#include "OgreException.h"
#include "OgreKeyEvent.h"
#include "OgreStringConverter.h"

namespace Ogre
{

    class ExampleFrameListener : public FrameListener, public KeyListener
    {
    protected:
        int mSceneDetailIndex;
        Real mMoveSpeed;
        Real mRotateSpeed;

    public:
        // Constructor takes a RenderWindow because it uses that to determine input context
        ExampleFrameListener(
            RenderWindow* win, Camera* cam, bool useBufferedInputKeys = false, bool useBufferedInputMouse = false)
        {
            mUseBufferedInputKeys = useBufferedInputKeys;
            mUseBufferedInputMouse = useBufferedInputMouse;
            mInputTypeSwitchingOn = mUseBufferedInputKeys || mUseBufferedInputMouse;
            mRotateSpeed = 36;
            mMoveSpeed = 100;

            if (mInputTypeSwitchingOn)
            {
                mEventProcessor = new EventProcessor();
                mEventProcessor->initialise(win);
                mEventProcessor->startProcessingEvents();
                mEventProcessor->addKeyListener(this);
                mInputDevice = mEventProcessor->getInputReader();
            }
            else
            {
                mInputDevice = PlatformManager::getSingleton().createInputReader();
                mInputDevice->initialise(win, true, true);
            }

            mCamera = cam;
            mWindow = win;
            mStatsOn = true;
            mNumScreenShots = 0;
            mTimeUntilNextToggle = 0;
            mSceneDetailIndex = 0;
            mMoveScale = 0.0f;
            mRotScale = 0.0f;
            mTranslateVector = Vector3::ZERO;
            mAniso = 1;
            mFiltering = TFO_BILINEAR;
        }
        virtual ~ExampleFrameListener()
        {
            if (mInputTypeSwitchingOn)
            {
                delete mEventProcessor;
            }
            else
            {
                PlatformManager::getSingleton().destroyInputReader(mInputDevice);
            }
        }

        virtual bool processUnbufferedKeyInput(const FrameEvent& evt)
        {
            if (mInputDevice->isKeyDown(KC_A))
            {
                // Move camera left
                mTranslateVector.x = -mMoveScale;
            }

            if (mInputDevice->isKeyDown(KC_D))
            {
                // Move camera RIGHT
                mTranslateVector.x = mMoveScale;
            }

            /* Move camera forward by keypress. */
            if (mInputDevice->isKeyDown(KC_UP) || mInputDevice->isKeyDown(KC_W))
            {
                mTranslateVector.z = -mMoveScale;
            }
            /* Move camera forward by mousewheel. */
            if (mInputDevice->getMouseRelativeZ() > 0)
            {
                mTranslateVector.z = -mMoveScale * 8.0;
            }

            /* Move camera backward by keypress. */
            if (mInputDevice->isKeyDown(KC_DOWN) || mInputDevice->isKeyDown(KC_S))
            {
                mTranslateVector.z = mMoveScale;
            }

            /* Move camera backward by mouse wheel. */
            if (mInputDevice->getMouseRelativeZ() < 0)
            {
                mTranslateVector.z = mMoveScale * 8.0;
            }

            if (mInputDevice->isKeyDown(KC_PGUP))
            {
                // Move camera up
                mTranslateVector.y = mMoveScale;
            }

            if (mInputDevice->isKeyDown(KC_PGDOWN))
            {
                // Move camera down
                mTranslateVector.y = -mMoveScale;
            }

            if (mInputDevice->isKeyDown(KC_RIGHT))
            {
                //		mCamera->yaw(-mRotScale);
            }

            if (mInputDevice->isKeyDown(KC_LEFT))
            {
                //		mCamera->yaw(mRotScale);
            }

            if (mInputDevice->isKeyDown(KC_ESCAPE))
            {
                return false;
            }

            // see if switching is on, and you want to toggle
            if (mInputTypeSwitchingOn && mInputDevice->isKeyDown(KC_M) && mTimeUntilNextToggle <= 0)
            {
                switchMouseMode();
                mTimeUntilNextToggle = 1;
            }

            if (mInputTypeSwitchingOn && mInputDevice->isKeyDown(KC_K) && mTimeUntilNextToggle <= 0)
            {
                // must be going from immediate keyboard to buffered keyboard
                switchKeyMode();
                mTimeUntilNextToggle = 1;
            }
            if (mInputDevice->isKeyDown(KC_F) && mTimeUntilNextToggle <= 0)
            {
                mStatsOn = !mStatsOn;
                mTimeUntilNextToggle = 1;
            }
            if (mInputDevice->isKeyDown(KC_T) && mTimeUntilNextToggle <= 0)
            {
                switch (mFiltering)
                {
                case TFO_BILINEAR:
                    mFiltering = TFO_TRILINEAR;
                    mAniso = 1;
                    break;
                case TFO_TRILINEAR:
                    mFiltering = TFO_ANISOTROPIC;
                    mAniso = 8;
                    break;
                case TFO_ANISOTROPIC:
                    mFiltering = TFO_BILINEAR;
                    mAniso = 1;
                    break;
                default:
                    break;
                }
                MaterialManager::getSingleton().setDefaultTextureFiltering(mFiltering);
                MaterialManager::getSingleton().setDefaultAnisotropy(mAniso);

                mTimeUntilNextToggle = 1;
            }

            if (mInputDevice->isKeyDown(KC_SYSRQ) && mTimeUntilNextToggle <= 0)
            {
                char tmp[20];
                sprintf(tmp, "screenshot_%d.png", ++mNumScreenShots);
                mWindow->writeContentsToFile(tmp);
                mTimeUntilNextToggle = 0.5;
                mWindow->setDebugText(String("Wrote ") + tmp);
            }

            if (mInputDevice->isKeyDown(KC_R) && mTimeUntilNextToggle <= 0)
            {
                mSceneDetailIndex = (mSceneDetailIndex + 1) % 3;
                switch (mSceneDetailIndex)
                {
                case 0:
                    mCamera->setPolygonMode(PM_SOLID);
                    break;
                case 1:
                    mCamera->setPolygonMode(PM_WIREFRAME);
                    break;
                case 2:
                    mCamera->setPolygonMode(PM_POINTS);
                    break;
                }
                mTimeUntilNextToggle = 0.5;
            }

            static bool displayCameraDetails = false;
            if (mInputDevice->isKeyDown(KC_P) && mTimeUntilNextToggle <= 0)
            {
                displayCameraDetails = !displayCameraDetails;
                mTimeUntilNextToggle = 0.5;
                if (!displayCameraDetails)
                    mWindow->setDebugText("");
            }
            if (displayCameraDetails)
            {
                // Print camera details
                mWindow->setDebugText("P: " + StringConverter::toString(mCamera->getDerivedPosition()) + " "
                    + "O: " + StringConverter::toString(mCamera->getDerivedOrientation()));
            }

            // Return true to continue rendering
            return true;
        }

        bool processUnbufferedMouseInput(const FrameEvent& evt)
        {
            /* Rotation factors, may not be used if the second mouse button is pressed. */

            /* If the second mouse button is pressed, then the mouse movement results in
               sliding the camera, otherwise we rotate. */
            if (mInputDevice->getMouseButton(1))
            {
                mTranslateVector.x += mInputDevice->getMouseRelativeX() * 0.13;
                mTranslateVector.y -= mInputDevice->getMouseRelativeY() * 0.13;
            }
            else
            {
                mRotX = -mInputDevice->getMouseRelativeX() * 0.13;
                mRotY = -mInputDevice->getMouseRelativeY() * 0.13;
            }

            return true;
        }

        void moveCamera()
        {

            // Make all the changes to the camera
            // Note that YAW direction is around a fixed axis (freelook style) rather than a natural YAW (e.g. airplane)
            //			mCamera->yaw(mRotX);
            //			mCamera->pitch(mRotY);
            mCamera->moveRelative(mTranslateVector);
        }

        // Override frameStarted event to process that (don't care about frameEnded)
        bool frameStarted(const FrameEvent& evt)
        {
            if (mWindow->isClosed())
                return false;

            if (!mInputTypeSwitchingOn)
            {
                mInputDevice->capture();
            }

            if (!mUseBufferedInputMouse || !mUseBufferedInputKeys)
            {
                // one of the input modes is immediate, so setup what is needed for immediate mouse/key movement
                if (mTimeUntilNextToggle >= 0)
                    mTimeUntilNextToggle -= evt.timeSinceLastFrame;

                // If this is the first frame, pick a speed
                if (evt.timeSinceLastFrame == 0)
                {
                    mMoveScale = 1;
                    mRotScale = 0.1;
                }
                // Otherwise scale movement units by time passed since last frame
                else
                {
                    // Move about 100 units per second,
                    mMoveScale = mMoveSpeed * evt.timeSinceLastFrame;
                    // Take about 10 seconds for full rotation
                    mRotScale = mRotateSpeed * evt.timeSinceLastFrame;
                }
                mRotX = 0;
                mRotY = 0;
                mTranslateVector = Vector3::ZERO;
            }

            if (mUseBufferedInputKeys)
            {
                // no need to do any processing here, it is handled by event processor and
                // you get the results as KeyEvents
            }
            else
            {
                if (processUnbufferedKeyInput(evt) == false)
                {
                    return false;
                }
            }
            if (mUseBufferedInputMouse)
            {
                // no need to do any processing here, it is handled by event processor and
                // you get the results as MouseEvents
            }
            else
            {
                if (processUnbufferedMouseInput(evt) == false)
                {
                    return false;
                }
            }

            if (!mUseBufferedInputMouse || !mUseBufferedInputKeys)
            {
                // one of the input modes is immediate, so update the movement vector

                moveCamera();
            }

            return true;
        }

        bool frameEnded(const FrameEvent& evt)
        {
            return true;
        }

        void switchMouseMode()
        {
            mUseBufferedInputMouse = !mUseBufferedInputMouse;
            mInputDevice->setBufferedInput(mUseBufferedInputKeys, mUseBufferedInputMouse);
        }
        void switchKeyMode()
        {
            mUseBufferedInputKeys = !mUseBufferedInputKeys;
            mInputDevice->setBufferedInput(mUseBufferedInputKeys, mUseBufferedInputMouse);
        }

        void keyClicked(KeyEvent* e)
        {
            if (e->getKeyChar() == 'm')
            {
                switchMouseMode();
            }
            else if (e->getKeyChar() == 'k')
            {

                switchKeyMode();
            }
        }
        void keyPressed(KeyEvent* e)
        {
        }
        void keyReleased(KeyEvent* e)
        {
        }

    protected:
        EventProcessor* mEventProcessor;
        InputReader* mInputDevice;
        Camera* mCamera;

        Vector3 mTranslateVector;
        RenderWindow* mWindow;
        bool mStatsOn;
        bool mUseBufferedInputKeys, mUseBufferedInputMouse, mInputTypeSwitchingOn;
        unsigned int mNumScreenShots;
        float mMoveScale;
        float mRotScale;
        // just to stop toggles flipping too fast
        Real mTimeUntilNextToggle;
        float mRotX, mRotY;
        TextureFilterOptions mFiltering;
        int mAniso;
    };
}
#endif
