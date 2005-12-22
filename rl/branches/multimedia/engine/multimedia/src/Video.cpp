/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
#include "Video.h"
#include "Exception.h"

#include <OgreNoMemoryMacros.h>
#include <CEGUISystem.h>
#include <OgreMemoryMacros.h>
#include <OgreExternalTextureSourceManager.h>
#include <OgreCEGUIRenderer.h>

#include "VideoEvents.h"
#include "MultimediaSubsystem.h"

using namespace CEGUI;
using namespace Ogre;

namespace rl
{

/**
 * Konstruktor
 * @param textureName Name der zu benutzenden Textur.
 * @param movieName Name des Videos, das abgespielt werden soll.
 * @author JoSch
 * @date 08-27-2005
 */
Video::Video(CeGuiString textureName, CeGuiString movieName)
  : TheoraMovieMessage(),
    mTextureName(textureName),
    mMovieName(movieName),
    mTexture(0),
    mGUIRenderer(0),
    mVideoControl(0),
    mClip(0)
{
    RlAssert(System::getSingleton().getRenderer() != 0, "No renderer yet");
    mVideoControl = static_cast<TheoraVideoController*>
        (ExternalTextureSourceManager::getSingleton().getExternalTextureSource("ogg_video"));
    if (!mVideoControl)
    {
        Throw(RuntimeException, "Can't get VideoController");
    }
    mGUIRenderer = static_cast<OgreCEGUIRenderer*>(System::getSingleton().getRenderer());
    if (!mGUIRenderer)
    {
        Throw(RuntimeException, "Can't get renderer");
    }
}

/**
 * Destruktor
 * @author JoSch
 * @date 08-27-2005
 */
Video::~Video()
{
    stop();
}

/**
 * Gibt die Breite des Videos zurueck.
 * @return Breite des Videos
 * @author JoSch
 * @date 08-27-2005
 */
unsigned int Video::getWidth() const
{
    return mClip->getVideoDriver()->getWidth();
}

/**
 * Gibt die Hoehe des Videos zurueck.
 * @return Hoehe des Videos
 * @author JoSch
 * @date 08-27-2005
 */
unsigned int Video::getHeight() const
{
    return mClip->getVideoDriver()->getHeight();
}

/**
 * Das Video auffrischen
 * @author JoSch
 * @date 08-27-2005
 */
void Video::update()
{
    if(mClip)
    {
        mClip->blitFrameCheck();
    }
}

/**
 * Das Video starten
 * @author JoSch
 * @date 08-27-2005
 */
void Video::play()
{
    if( mClip )
    {
        destroyCETexture();
        mClip->changePlayMode(TextureEffectPause);
        mVideoControl->destroyAdvancedTexture(mTextureName.c_str());
        // TODO mSoundSystem.destroyAudioClip();
        mClip = 0;
    }
    
    //Sets an input file name - needed by plugin
    mVideoControl->setInputName(mMovieName.c_str());
    //Start paused so we can have audio
    mVideoControl->setPlayMode(TextureEffectPause);
    //! Used for attaching texture to Technique, State, and texture unit layer
    mVideoControl->setTextureTecPassStateLevel(0, 0, 0);

    //Set to true to allow for seeking - highly experimental though ;)
    mVideoControl->setSeekEnabled(false);

    // Grab Our material, then add a new texture unit
    bool bSucceed = true;
    MaterialPtr material;

    try
    {
        material = MaterialManager::getSingleton().getByName(mTextureName.c_str());
        if (material.isNull())
            bSucceed = false;
    }
    catch(...)
    {
        bSucceed = false;
    }

    if (!bSucceed)
    {
        //Looks like material wasn't found, so create a new one here
        material = MaterialManager::getSingleton().create(mTextureName.c_str(), "General");
        if (material.isNull())
        {
            return;         //Some wierd error
        }

        material->createTechnique()->createPass();
    }

    material->getTechnique(0)->getPass(0)->createTextureUnitState();
    
    mVideoControl->createDefinedTexture(mTextureName.c_str(), "General");
    mClip = mVideoControl->getMaterialNameClip(mTextureName.c_str());
    
    //Register, add audio, start playback
    if (!mClip)
    {
        std::string text = "Clip "+ std::string(mMovieName.c_str()) + " not found while trying to play";
        Throw(InvalidArgumentException, text);
    }

    mClip->registerMessageHandler(this);
    // TODO mClip->setAudioDriver(mSoundSystem.getAudioClip());
    mClip->changePlayMode(TextureEffectPlay_ASAP);
    createCETexture();
    if (MultimediaSubsystem::getSingletonPtr())
    {
        MultimediaSubsystem::getSingleton().addVideo(this);
    }
    mPlaying = true;
}

/**
 * Video-Textur erzeugen
 * @author JoSch
 * @date 08-27-2005
 */
void Video::createCETexture()
{
    if (mClip)
    {
        mTexture = mGUIRenderer->createTexture(mClip->getVideoDriver()->getTexture());
    }
}

/**
 * Video-Texttur zerstoeren.
 * @author JoSch
 * @date 08-27-2005
 */
void Video::destroyCETexture()
{
    if (mTexture)
    {
        mGUIRenderer->destroyTexture(mTexture);
    }
    mTexture = 0;
}

/**
 * Das Video stoppen
 * @author JoSch
 * @date 08-27-2005
 */
void Video::stop()
{
    mPlaying = false;
    if( mClip )
    {
        mClip->changePlayMode(TextureEffectPause);
        mVideoControl->destroyAdvancedTexture(mTextureName.c_str());
    }
    MultimediaSubsystem::getSingleton().removeVideo(this);
    // TODO Audio behandenln
}

/**
 * Das Video unterbrechen oder weiterspielen lassen.
 * @param bPause TRUE, wenn unterbrochen werden soll. FALSE, wenn weitergespielt werden soll.
 * @author JoSch
 * @date 08-27-2005
 */
void Video::pause(bool bPause)
{
    mPlaying = !bPause;
    if(mClip)
    {
        if(bPause == false)
        {
            mClip->changePlayMode(TextureEffectPause);
        } else {
            mClip->changePlayMode(TextureEffectPlay_ASAP);
        }
    }
}

/**
 * Wird durch das Video-Plugin aufgerufen, um die Filmlaenge zu setzen.
 * @param discoveredTime Die Filmlaenge.
 * @author JoSch
 * @date 08-27-2005
 */
void Video::discoveredMovieTime(float discoveredTime)
{ 
    mMaxTime = discoveredTime;
}

/**
 * Das Video an einer bestimmten Stelle weiterspielen lassen
 * @param time Die gewuenschte Stelle in Prozent.
 * @author JoSch
 * @date 08-27-2005
 */
void Video::seek(float percentage)
{
    if( mClip )
    {
        mClip->seekToTime(percentage * mMaxTime);
    }
    
}

/**
 * Textur zur�ckgeben
 * @return Die vom Videplugin erzeugte Textur.
 * @author JoSch
 * @date 02-09-2005
 */
CEGUI::Texture* Video::getTexture() const
{
    return mTexture;
}

/**
 * Texturname zur�ckgeben
 * @return Der Name der Textur.
 * @author JoSch
 * @date 02-09-2005
 */
const CeGuiString Video::getTextureName() const
{
    return mTextureName;
}

/**
 * Die Videol�nge zur�ckgeben
 * @return Der L�nge des Videos.
 * @author JoSch
 * @date 02-09-2005
 */
float Video::getMaxTime() const
{
    return mMaxTime;
}

/**
 * Vom Plugin Nachrichten empfangen
 * @return Unused
 * @param message Die erhaltene Nachricht
 * @author JoSch
 * @date 08-27-2005
 */
int Video::messageEvent(PLUGIN_theora_message message)
{
    if (hasListeners())
    {
        VideoPlayEvent *event = new VideoPlayEvent(this);
        event->setReason((int)message);
        mEventCaster.dispatchEvent(event);
    }
    return 0;
}

/**
 * Vom Plugin Nachrichten empfangen
 * @return Unused
 * @param message Die erhaltene Nachricht
 * @author JoSch
 * @date 09-02-2005
 */
void Video::displayedFrame(float vTime, 
                           float aTime,
                           unsigned int frameNumber,
                           unsigned int framesDropped)
{
    if (hasListeners())
    {
        VideoTimingEvent *event = new VideoTimingEvent(this);
        event->mVTime = vTime;
        event->mATime = aTime;
        event->mFrameNumber = frameNumber;
        event->mFramesDropped = framesDropped;
        mEventCaster.dispatchEvent(event);
    }
}

/**
 * Listener einf�gen
 * @param listener Der neue Listener
 * @author JoSch
 * @date 09-02-2005
 */
void Video::addEventListener(VideoEventListener* listener)
{
    if(!mEventCaster.containsListener(listener))
    {        
        mEventCaster.addEventListener(listener);
    }
}

/**
 * Listener eentfernen
 * @param listener Der zu entfernende Listener
 * @author JoSch
 * @date 09-02-2005
 */
void Video::removeEventListener(VideoEventListener* listener)
{
    if(mEventCaster.containsListener(listener))
    { 
        mEventCaster.removeEventListener(listener);
    }
}

/**
 * Alle Listener entfernen
 * @author JoSch
 * @date 09-02-2005
 */
void Video::removeEventListeners()
{
    mEventCaster.removeEventListeners();
}

/**
 * Hört jemand den Events zu?
 * @return TRUE wenn es Listener gibt, FALSE sonst
 * @author JoSch
 * @date 09-02-2005
 */
bool Video::hasListeners( ) const
{
    return mEventCaster.hasEventListeners();
}


/**
 * Spielt der Clip?
 * @return TRUE wenn der Clip spielt, FALSE sonst
 * @author JoSch
 * @date 09-02-2005
 */
bool Video::isPlaying( ) const
{
    return mPlaying;
}

}
