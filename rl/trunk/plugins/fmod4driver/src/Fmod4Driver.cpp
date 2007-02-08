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
#include "Fmod4Driver.h"

#include "Fmod4Sound.h"
#include "Fmod4Listener.h"
#include "Logger.h"
#include "SoundResource.h"

using namespace Ogre;

namespace rl
{
    String Fmod4Driver::NAME = "RlFmod4Driver";

    Fmod4Driver::Fmod4Driver(Ogre::ResourceManager *soundResourceManager)
        : SoundDriver(soundResourceManager),
        mMasterChannelGroup(NULL)
    {
    }

    Fmod4Driver::~Fmod4Driver()
    {
    }

    String Fmod4Driver::getName() const
    {
        return NAME;
    }

    bool Fmod4Driver::initialize()
    {
        static int MAX_VIRTUAL_CHANNELS = 100;

        CHECK_FMOD4_ERRORS(FMOD::System_Create(&mFmod4System));

        mFmod4System->setFileSystem(
            Fmod4Driver::open,
            Fmod4Driver::close,
            Fmod4Driver::read,
            Fmod4Driver::seek,
            -1);

        printData();

        mFmod4System->setDriver(-1);
        //CHECK_FMOD4_ERRORS(mFmod4System->setOutput(FMOD_OUTPUTTYPE_ESD));

        FMOD_RESULT result = mFmod4System->init(MAX_VIRTUAL_CHANNELS, FMOD_INIT_NORMAL, NULL); //Alternative: ,Output)
        if (result != FMOD_OK)
        {
            LOG_ERROR(Logger::CORE,
                "FMOD error #"
                + StringConverter::toString(result)
                + " "
                + FMOD_ErrorString(result));

            return false;
        }

        mFmod4System->getMasterChannelGroup(&mMasterChannelGroup);
        return true;
    }

    void Fmod4Driver::shutdown()
    {
        CHECK_FMOD4_ERRORS(mFmod4System->close());
        CHECK_FMOD4_ERRORS(mFmod4System->release());
    }

    bool Fmod4Driver::isDriverAvailable()
    {
        FMOD::System* system;
        FMOD_RESULT result = FMOD::System_Create(&system);
        if (result != FMOD_OK)
        {
            return false;
        }

        system->release();
        return true;
    }

    void Fmod4Driver::update()
    {
        mFmod4System->update();
    }

    /**
      * Einen Sound-Stream mit Resource erzeugen
      * @return Der erzeugte Stream
      * @author JoSch
      * @date 03-06-2006
      */
    Sound *Fmod4Driver::createStreamImpl(const SoundResourcePtr &res)
    {
         Sound *sound = new Fmod4Sound(this, res, Fmod4Sound::STREAM);
         return sound;
    }

    /**
     * Einen Sound-Sample mit Resource erzeugen
     * @return Das erzeugte Sample
     * @author JoSch
     * @date 03-06-2006
     */
    Sound *Fmod4Driver::createSampleImpl(const SoundResourcePtr &res)
    {
        Sound *sound = new Fmod4Sound(this, res, Fmod4Sound::SAMPLE);
         return sound;
    }

    ListenerMovable *Fmod4Driver::createListener(const Ogre::String &name)
    {
        return new Fmod4Listener(name, mFmod4System);
    }

    FMOD::System* Fmod4Driver::_getFmodSystem()
    {
        return mFmod4System;
    }

    void Fmod4Driver::applySettings(const Ogre::NameValuePairList& settings)
    {
        SoundDriver::applySettings(settings);
        mMasterChannelGroup->setVolume(mMasterVolume);
    }

    Ogre::NameValuePairList Fmod4Driver::getSettings() const
    {
        return SoundDriver::getSettings();
    }

    void Fmod4Driver::printData() const
    {
        int numDrivers;
        mFmod4System->getNumDrivers(&numDrivers);

        for (int i = 0; i < numDrivers; i++)
        {
            printDriverInfo(i);
        }

        int numPlugins;
        mFmod4System->getNumPlugins(FMOD_PLUGINTYPE_CODEC, &numPlugins);
        for (int i = 0; i < numDrivers; i++)
        {
            char pluginName[128];
            unsigned int version;

            mFmod4System->getPluginInfo(
                FMOD_PLUGINTYPE_CODEC,
                i,
                pluginName,
                127,
                &version);
            LOG_MESSAGE(Logger::MULTIMEDIA,
                String("Fmod4Driver Plugin '")
                + pluginName
                + "' Version "
                + StringConverter::toString(version));
        }
    }

    void Fmod4Driver::printDriverInfo(int driver) const
    {
        int curdriver;
        mFmod4System->getDriver(&curdriver);

        char name[128];
        mFmod4System->getDriverName(driver, name, 127);

        FMOD_CAPS caps;
        int minfreq, maxfreq;
        FMOD_SPEAKERMODE speakermode;
        mFmod4System->getDriverCaps(driver, &caps, &minfreq, &maxfreq, &speakermode);

        LOG_MESSAGE(Logger::MULTIMEDIA,
            String("Fmod4Driver '")
            + name
            + "'"
            + (driver == curdriver ? "(cur)" : "")
            + ": \n"
            + "Freq " + StringConverter::toString(minfreq)
            + " - " + StringConverter::toString(maxfreq));

        int hard2d, hard3d, hardtotal;
        mFmod4System->getHardwareChannels(&hard2d, &hard3d, &hardtotal);
        LOG_MESSAGE(Logger::MULTIMEDIA,
            String("Fmod4Driver '")
            + name + "' Hardware Channels: "
            + "2D #" + StringConverter::toString(hard2d)
            + ", 3D #" + StringConverter::toString(hard3d)
            + ", Total #" + StringConverter::toString(hardtotal));

        int softchannels;
        mFmod4System->getSoftwareChannels(&softchannels);
        LOG_MESSAGE(Logger::MULTIMEDIA,
            String("Fmod4Driver '")
            + name + "' Software Channels: "
            + "#" + StringConverter::toString(softchannels));

        FMOD_OUTPUTTYPE output;
        mFmod4System->getOutput(&output);
        String outputName;

        if(output == FMOD_OUTPUTTYPE_AUTODETECT)
            outputName = "Autodetect";
        else if(output == FMOD_OUTPUTTYPE_UNKNOWN)
            outputName = "Unknown";
        else if(output == FMOD_OUTPUTTYPE_NOSOUND)
            outputName = "No Sound";
        else if(output == FMOD_OUTPUTTYPE_WAVWRITER)
            outputName = "Wave Writer";
        else if(output == FMOD_OUTPUTTYPE_NOSOUND_NRT)
            outputName = "No Sound non realtime";
        else if(output == FMOD_OUTPUTTYPE_WAVWRITER_NRT)
            outputName = "Wave Writer non realtime";
        else if(output == FMOD_OUTPUTTYPE_DSOUND)
            outputName = "DirectSound";
        else if(output == FMOD_OUTPUTTYPE_WINMM)
            outputName = "Windows Multimedia";
        else if(output == FMOD_OUTPUTTYPE_ASIO)
            outputName = "Low Latency ASIO driver";
        else if(output == FMOD_OUTPUTTYPE_OSS)
            outputName = "Open Sound System (OSS)";
        else if(output == FMOD_OUTPUTTYPE_ALSA)
            outputName = "Advanced Linux Sound Architecture (ALSA)";
        else if(output == FMOD_OUTPUTTYPE_ESD)
            outputName = "Enlightment Sound Daemon (ESD)";
        else if(output == FMOD_OUTPUTTYPE_SOUNDMANAGER)
            outputName = "Macintosh SoundManager";
        else if(output == FMOD_OUTPUTTYPE_COREAUDIO)
            outputName = "Macintosh CoreAudio";

        LOG_MESSAGE(Logger::MULTIMEDIA,
            String("Fmod4Driver '")
            + name + "' Output: " + outputName);
    }

    FMOD_RESULT F_CALLBACK Fmod4Driver::open(
        const char *  name,
        int  unicode,
        unsigned int *  filesize,
        void **  handle,
        void **  userdata)
    {
        SoundResourcePtr* res = new SoundResourcePtr(sSoundResourceManager->getByName(name));
        LOG_DEBUG(Logger::MULTIMEDIA,
            "Opened stream " + (*res)->getName());
        (*res)->load();
        *filesize = (*res)->getSize();
        *handle = res;
        return FMOD_OK;
    }

    FMOD_RESULT F_CALLBACK Fmod4Driver::close(
        void *  handle,
        void *  userdata)
    {
           SoundResourcePtr *res = static_cast<SoundResourcePtr*>(handle);
        LOG_DEBUG(Logger::MULTIMEDIA,
            "Closing stream " + (*res)->getName());
        (*res)->unload();
        delete res;
        return FMOD_OK;
    }

    FMOD_RESULT F_CALLBACK Fmod4Driver::read(
        void *  handle,
        void *  buffer,
        unsigned int  sizebytes,
        unsigned int *  bytesread,
        void *  userdata)
    {
        if (handle == NULL)
        {
            return FMOD_ERR_INVALID_PARAM;
        }

        SoundResourcePtr *sound = static_cast<SoundResourcePtr*>(handle);
        if (!sound->isNull())
        {
            if (!(*sound)->isLoaded())
            {
                (*sound)->load();
            }

            DataStreamPtr ds = (*sound)->getDataStream();
            if (!ds.isNull())
            {
                *bytesread = ds->read(buffer, sizebytes);
                if (*bytesread < sizebytes)
                {
                    return FMOD_ERR_FILE_EOF;
                }
                return FMOD_OK;
            }
        }

        return FMOD_ERR_FILE_EOF;
    }

    FMOD_RESULT F_CALLBACK Fmod4Driver::seek(
        void *  handle,
        unsigned int  pos,
        void *  userdata)
    {
        if (handle == NULL)
        {
            return FMOD_ERR_INVALID_PARAM;
        }

        SoundResourcePtr *sound = static_cast<SoundResourcePtr*>(handle);
        if (!sound->isNull())
        {
            DataStreamPtr ds = (*sound)->getDataStream();
            if (!ds.isNull())
            {
                ds->seek(pos);
                return FMOD_OK;
            }
        }
        return FMOD_ERR_INVALID_PARAM;
    }

    void Fmod4Driver::setMasterVolume(const Ogre::Real& vol)
    {
        SoundDriver::setMasterVolume(vol);
        /**@todo: Lautst�rke setzen, geht m�glicherweise nur �ber channelgroups*/
    }

    /// Setzt den Faktor f, mit der die Lautst�rke nach der Formel 1/(f*Entfernung) abnimmt
    void Fmod4Driver::setRolloffFactor(const Ogre::Real& factor)
    {
        float dopplerScale;
        mFmod4System->get3DSettings(&dopplerScale, NULL, NULL);
        mFmod4System->set3DSettings(dopplerScale, 1.0, factor);
    }

    const Ogre::Real Fmod4Driver::getRolloffFactor()
    {
        float factor;
        mFmod4System->get3DSettings(NULL, NULL, &factor);
        return factor;
    }
}
