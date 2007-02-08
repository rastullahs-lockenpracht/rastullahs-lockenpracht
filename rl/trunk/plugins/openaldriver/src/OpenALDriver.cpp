/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#include "OpenALDriver.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    // OpenAL 1.1 unter Windows
//    extern "C" {
//        #include "al.h"
//        #include "alc.h"
//    }
//#else
//    // OpenAL 1.0 unter Linux
//    extern "C" {
//        #include "AL/al.h"
//        #include "AL/alc.h"
//    }
#endif

#include "OpenALSoundSample.h"
#include "OpenALSoundStream.h"
#include "OpenALListener.h"

namespace rl
{
    Ogre::String rl::OpenALDriver::NAME = "RlOpenALDriver";

    /**
     * Konstruktor
     * @author JoSch
     * @date 12-23-2005
     */
    OpenALDriver::OpenALDriver(Ogre::ResourceManager* soundResourceManager)
    : SoundDriver(soundResourceManager)
    {
    }

    /**
     * Destruktor
     * @author JoSch
     * @date 12-23-2005
     */
    OpenALDriver::~OpenALDriver()
    {
    }

    /**
     * Zeigt an, ob der Treiber funktionsfähig ist.
     * @return Immer TRUE.
     * @author JoSch
     * @date 12-23-2005
     */
    bool OpenALDriver::isDriverAvailable()
    {
        //try {
        //    // Clear error code
        //    alGetError();
        //    ALCdevice *device = alcOpenDevice(0);
        //
        //    if (device == 0)
        //    {
        //        return false;
        //    }
        //
        //    //Create context(s)
        //    ALCcontext *context = alcCreateContext(device, 0);
        //    //Set active context
        //    alcMakeContextCurrent(context);
        //    // Get Error Code
        //    ALenum success = alGetError();
        //    //Disable context
        //    alcMakeContextCurrent(0);
        //    //Release context(s)
        //    alcDestroyContext(context);
        //    //Close device
        //    alcCloseDevice(device);
        //
        //    return (success == AL_NO_ERROR);
        //} catch (...)
        //{
            return false;
        //}
    }

    /**
     * Starte den Treiber und mache alle Initialisierung.
     * @author JoSch
     * @date 12-23-2005
     */
    bool OpenALDriver::initialize()
    {
        return true;
    }

    /**
     * Beeende den Treiber und gib alle Resourcen frei.
     * @author JoSch
     * @date 12-23-2005
     */
    void OpenALDriver::shutdown()
    {
    }

    /**
     * Gibt den Anzeigename des Treiber zurück.
     * @return Anzeigename
     * @author JoSch
     * @date 12-23-2005
     */
    Ogre::String OpenALDriver::getName() const
    {
        return NAME;
    }

    /**
     * Update-Aufgaben erledigen
     * @author JoSch
     * @date 03-06-2006
     */
     void OpenALDriver::update()
     {
         // Erstmal nichts zu tun.
     }

     /**
      * Einen Sound-Stream mit Resource erzeugen
      * @return Der erzeugte Stream
      * @author JoSch
      * @date 03-06-2006
      */
    Sound* OpenALDriver::createStreamImpl(const SoundResourcePtr &res)
    {
         Sound *sound = new OpenALSoundStream(this, res);
         return sound;
    }

    /**
     * Einen Sound-Sample mit Resource erzeugen
     * @return Das erzeugte Sample
     * @author JoSch
     * @date 03-06-2006
     */
    Sound* OpenALDriver::createSampleImpl(const SoundResourcePtr &res)
    {
         Sound *sound = new OpenALSoundSample(this, res);
         return sound;
    }

    /**
     * Einen Soundlistener erzeugen
     * @return Der erzeugte Listener
     * @param name Der Name des Channels.
     * @author JoSch
     * @date 04-04-2006
     */
    ListenerMovable *OpenALDriver::createListener(const Ogre::String &name)
    {
        ListenerMovable *listener = new OpenALListener(name);
        return listener;
    }

    /**
     * Ausdruck einiger Werte des Soundsystems.
     * @author JoSch
     * @date 01-20-2006
     */
    void OpenALDriver::printData() const
    {
    }

    void OpenALDriver::setRolloffFactor(const Ogre::Real& f)
    {
    }

    const Ogre::Real OpenALDriver::getRolloffFactor()
    {
        return 1.0;
    }

}
