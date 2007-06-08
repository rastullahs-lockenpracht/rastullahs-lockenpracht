/*
* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include "DialogCharacterController.h"
#include "Exception.h"

#include "Actor.h"
#include "CameraObject.h"
#include "CommandMapper.h"
#include "ConfigurationManager.h"
#include "CoreSubsystem.h"
#include "DialogCharacter.h"
#include "DialogSubsystem.h"
#include "DialogWindow.h"
#include "InputManager.h"
#include "MeshAnimation.h"
#include "MeshObject.h"
#include "Person.h"
#include "PhysicalThing.h"
#include "Sound.h"
#include "SoundDriver.h"
#include "SoundManager.h"
#include "SoundObject.h"
#include "SubtitleWindow.h"
#include "WindowFactory.h"
#include "WindowManager.h"
#include "World.h"

#include <OgreSceneManager.h>
#include <OgreNewt_Body.h>

using namespace Ogre;

namespace rl {

    DialogCharacterController::DialogCharacterController(CommandMapper* cmdMapper,
        Actor* camera, Person* character)
        : CharacterController(cmdMapper, camera, character, CST_DIALOG),
        mTargetCameraPosition(Vector3::ZERO),
        mTargetCameraDirection(Vector3::UNIT_Z),
        mCurrFadeTextTime(0),
        mTotalFadeTextTime(0),
        mText(),
        mDialogMode(DM_FRONT),
        mTextShown(false),
        mDialogWindow(NULL),
        mSubtitleWindow(NULL),
        mSoundObject(NULL),
        mTalkAnimation(NULL),
        mSubtitleSpeed(1.0f)
    {
        mSubtitleSpeed = ConfigurationManager::getSingleton().getRealSetting(
            "General", "Subtitle Speed");

    }

    DialogCharacterController::~DialogCharacterController()
    {
        delete mSoundObject;
    }

    void DialogCharacterController::pause()
    {
        if(mDialogPartner)
        {
            MeshObject* mesh = dynamic_cast<MeshObject*>(mDialogPartner->getControlledObject());
            if (mesh != NULL)
            {
                mesh->stopAllAnimations();
                if (mesh->hasAnimation("idle"))
                {
                    mTalkAnimation = mesh->startAnimation("idle");
                }
            }
        }
        mCameraActor->getPhysicalThing()->unfreeze();
        mCharacterActor->getPhysicalThing()->unfreeze();
        mDialogWindow->setVisible(false, false);
        mSubtitleWindow->setVisible(false, false);
    }

    void DialogCharacterController::resume()
    {
        mCharacterActor->setVisible(true);

        mCameraActor->getPhysicalThing()->freeze();
        mCharacterActor->getPhysicalThing()->freeze();
        Camera* ogreCam = static_cast<Camera*>(mCameraActor->_getMovableObject());
        ogreCam->setFixedYawAxis(true);
        ogreCam->setPosition(Vector3::ZERO);
        ogreCam->setOrientation(Quaternion::IDENTITY);
        mCameraActor->_getSceneNode()->setFixedYawAxis(true);

        DialogCharacter* bot = DialogSubsystem::getSingleton().getCurrentDialogCharacter();
        bot->setDialogCharacter(mCharacter);
		mDialogPartner = bot->getDialogPartner()->getActor();

        recalculateCamera( mDialogPartner, mCharacterActor );

        mSubtitleWindow = WindowFactory::getSingleton().getSubtitleWindow();

        mDialogWindow = WindowFactory::getSingleton().getDialogWindow();
        mDialogWindow->initialize(this, bot);
		mDialogWindow->start();
    }

    void DialogCharacterController::run(Real elapsedTime)
    {
        if( elapsedTime <= 0.0 )
            return;

        // Aktuellen Abstand zur gewünschten Position berechnen
        Vector3 posDistance = (mTargetCameraPosition - mCameraActor->_getSceneNode()->getPosition());
        // Ist eine Anpassung nötig?
        if( posDistance != Vector3::ZERO )
        {
            // Die Kameraanpassgeschwindigkeit
            Real speed = 1/elapsedTime;
            mCameraActor->setPosition(mTargetCameraPosition);
        }

        mCameraActor->setOrientation( Quaternion::IDENTITY );
        mCameraActor->_getSceneNode()->setDirection( mTargetCameraDirection );
        mCameraActor->_getSceneNode()->_update( true, false );

        // Textanzeigedauer
        if (mCurrFadeTextTime >= 0)
        {
            mCurrFadeTextTime -= elapsedTime;
        }

        if (mTextShown && mCurrFadeTextTime <= 0)
        {
            mCurrFadeTextTime = 0;
            mTotalFadeTextTime = 0;
            mTextShown = false;
               if (mSoundObject)
            {
                mSoundObject->stop();
            }
            mSubtitleWindow->setVisible(false, false);
            mDialogWindow->textFinished();
        }

        LOG_DEBUG(Logger::UI,
                StringConverter::toString(mSubtitleWindow->getWindow()->getEffectiveAlpha())
                + ": "
                + mSubtitleWindow->getText()
                + CeGuiString(mSubtitleWindow->getWindow()->isVisible() ? "vis" : "unvis")
                + ", Time: "
                + StringConverter::toString(mCurrFadeTextTime));
    }

    void DialogCharacterController::recalculateCamera( Actor* speaker, Actor* listener )
    {
        // Position camera at position between char and dialog partner
        Vector3 charEyes = speaker->getWorldPosition();
        // Modify by MeshBounds
        if( speaker->getControlledObject()->isMeshObject() )
        {
            MeshObject* mo = dynamic_cast<MeshObject*>(speaker->getControlledObject());
            Ogre::AxisAlignedBox aab = mo->getDefaultSize();
            Vector3 offset(
                aab.getCenter().x,
                aab.getMaximum().y*0.933,
                aab.getCenter().z );
            charEyes += speaker->getWorldOrientation()*offset;
        }

        Vector3 partEyes = listener->getWorldPosition();
        // Modify by MeshBounds
        if( listener->getControlledObject()->isMeshObject() )
        {
            MeshObject* mo = dynamic_cast<MeshObject*>(listener->getControlledObject());
            Ogre::AxisAlignedBox aab = mo->getDefaultSize();
            Vector3 offset(
                aab.getCenter().x,
                aab.getMaximum().y*0.933,
                aab.getCenter().z );
            partEyes += listener->getWorldOrientation()*offset;
        }

        Vector3 globalCameraPosition = ( charEyes + partEyes ) / 2.0f;

        // Weltkoordinaten in lokale umwandeln
        mTargetCameraPosition =
            -1*(mCameraActor->_getSceneNode()->getParentSceneNode()->getWorldOrientation().Inverse()*
            (mCameraActor->_getSceneNode()->getParentSceneNode()->getWorldPosition() - globalCameraPosition));


        mTargetCameraDirection = ( partEyes - mTargetCameraPosition ).normalisedCopy();
    }

    float DialogCharacterController::getShowTextLength(const CeGuiString& text) const
    {
        return 0.019f * text.length() + // Zeit fürs Text lesen
               0.25f;                   // Fade in
    }

    void DialogCharacterController::response(
        Actor* actor, const CeGuiString& text, const Ogre::String& soundFile)
    {
        if( actor == mDialogPartner )
            recalculateCamera( mCharacterActor, mDialogPartner );
        else
            recalculateCamera( mDialogPartner, mCharacterActor );

        // Ungefähre Lesedauer bestimmen
        float fadeTime = getShowTextLength(text);
        if (soundFile.length() == 0)
        {
            float speed = mSubtitleSpeed;

            if( mSubtitleSpeed == 0.0 )
                speed = 1.0;

            mCurrFadeTextTime = fadeTime*speed;
            mTotalFadeTextTime = fadeTime*speed;
        }
        else
        {
            if( mSoundObject != NULL )
                mSoundObject->getMovableObject()->getParentSceneNode()->detachObject(
                    mSoundObject->getMovableObject() );

            delete mSoundObject;
            mSoundObject = new SoundObject(SoundManager::getSingleton().getActiveDriver()->
                    createSound(soundFile, ST_SAMPLE), soundFile);

            // An Sprecher hängen
            actor->_getSceneNode()->attachObject( mSoundObject->getMovableObject() );
            actor->_getSceneNode()->_update( true, false );
            mSoundObject->_setActor( actor );
            mSoundObject->set3d(true);
            mSoundObject->play();
            mSoundObject->_update();


            mCurrFadeTextTime = std::max(fadeTime*mSubtitleSpeed,mSoundObject->getLength());
            mTotalFadeTextTime = mCurrFadeTextTime;
        }

        MeshObject* mesh = dynamic_cast<MeshObject*>(actor->getControlledObject());
        if (mesh != NULL)
        {
            if (mesh->hasAnimation("reden"))
            {
                mesh->stopAllAnimations();
                mTalkAnimation = mesh->startAnimation("reden");
            }
        }

        LOG_DEBUG(Logger::UI,
            "Response: "
                + actor->getName()
                + " File: '"
                + soundFile
                + "', Text: '" + text + "', Time: "
                + StringConverter::toString(mCurrFadeTextTime));


        mTextShown = true;

        if (mSubtitleWindow != NULL && !text.empty())
        {
            mSubtitleWindow->show(text);
        }
    }

    bool DialogCharacterController::mouseReleased(const OIS::MouseEvent& evt,
        OIS::MouseButtonID id)
    {

        if (mTextShown && (mCurrFadeTextTime + 0.25) < mTotalFadeTextTime)
        {
            mCurrFadeTextTime = -1;

            return true;
        }
        else
        {
            return false;
        }

    }
}
