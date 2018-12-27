/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h"

#include <fmod_errors.h>

#include "Fmod4Driver.h"
#include "Fmod4Listener.h"
#include "Fmod4Sound.h"
#include "Fmod4SoundStitching.h"
#include "Logger.h"
#include "SoundResource.h"

using namespace Ogre;
using namespace std;

namespace rl
{
    // Used by FMOD-Callbacks, which are not C++-functions, but C-functions.
    Ogre::ResourceManager* gSoundResourceManager = NULL;
    Fmod4Driver* gDriver = NULL;

    String Fmod4Driver::NAME = "RlFmod4Driver";

    Fmod4Driver::Fmod4Driver(Ogre::ResourceManager* soundResourceManager)
        : SoundDriver(soundResourceManager)
        , mMasterChannelGroup(NULL)
    {
        gSoundResourceManager = soundResourceManager;
        gDriver = this;
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

        mFmod4System->setFileSystem(Fmod4Driver::open, Fmod4Driver::close, Fmod4Driver::read, Fmod4Driver::seek, -1);

        printData();

        mFmod4System->setDriver(-1);
        // CHECK_FMOD4_ERRORS(mFmod4System->setOutput(FMOD_OUTPUTTYPE_ESD));

        FMOD_RESULT result = mFmod4System->init(MAX_VIRTUAL_CHANNELS, FMOD_INIT_NORMAL, NULL);
        if (result != FMOD_OK)
        {
            LOG_ERROR(
                Logger::CORE, "FMOD error #" + StringConverter::toString(result) + " " + FMOD_ErrorString(result));

            return false;
        }

        mFmod4System->getMasterChannelGroup(&mMasterChannelGroup);

        initializeEaxPresetMap();
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

    Sound* Fmod4Driver::createSoundImpl(SoundResourcePtr res, SoundType type)
    {
        Sound* sound = new Fmod4Sound(this, res, type);
        return sound;
    }

    SoundStitching* Fmod4Driver::createSoundStitchingImpl(unsigned int numSlots, const CeGuiString& name)
    {
        SoundStitching* soundstitching = new Fmod4SoundStitching(numSlots, name, this);
        return soundstitching;
    }

    ListenerMovable* Fmod4Driver::createListener(const Ogre::String& name)
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

#define DEF_PRESET(NAME, DEFINITION)                                                                                   \
    {                                                                                                                  \
        FMOD_REVERB_PROPERTIES prop = DEFINITION;                                                                      \
        mEaxPresetMap.insert(make_pair(NAME, prop));                                                                   \
    }

    void Fmod4Driver::initializeEaxPresetMap()
    {
        DEF_PRESET("Alley", FMOD_PRESET_ALLEY);
        DEF_PRESET("Arena", FMOD_PRESET_ARENA);
        DEF_PRESET("Auditorium", FMOD_PRESET_AUDITORIUM);
        DEF_PRESET("Bathroom", FMOD_PRESET_BATHROOM);
        DEF_PRESET("Carpetted Hallway", FMOD_PRESET_CARPETTEDHALLWAY);
        DEF_PRESET("Cave", FMOD_PRESET_CAVE);
        DEF_PRESET("City", FMOD_PRESET_CITY);
        DEF_PRESET("Concert Hall", FMOD_PRESET_CONCERTHALL);
        DEF_PRESET("Dizzy", FMOD_PRESET_DIZZY);
        DEF_PRESET("Drugged", FMOD_PRESET_DRUGGED);
        DEF_PRESET("Forest", FMOD_PRESET_FOREST);
        DEF_PRESET("Generic", FMOD_PRESET_GENERIC);
        DEF_PRESET("Hallway", FMOD_PRESET_HALLWAY);
        DEF_PRESET("Hangar", FMOD_PRESET_HANGAR);
        DEF_PRESET("Living Room", FMOD_PRESET_LIVINGROOM);
        DEF_PRESET("Mountains", FMOD_PRESET_MOUNTAINS);
        DEF_PRESET("Off", FMOD_PRESET_OFF);
        DEF_PRESET("Padded Cell", FMOD_PRESET_PADDEDCELL);
        DEF_PRESET("Parkinglot", FMOD_PRESET_PARKINGLOT);
        DEF_PRESET("Plain", FMOD_PRESET_PLAIN);
        DEF_PRESET("Psychotic", FMOD_PRESET_PSYCHOTIC);
        DEF_PRESET("Quarry", FMOD_PRESET_QUARRY);
        DEF_PRESET("Room", FMOD_PRESET_ROOM);
        DEF_PRESET("Sewerpipe", FMOD_PRESET_SEWERPIPE);
        DEF_PRESET("Stone Corridor", FMOD_PRESET_STONECORRIDOR);
        DEF_PRESET("Stone Room", FMOD_PRESET_STONEROOM);
        DEF_PRESET("Underwater", FMOD_PRESET_UNDERWATER);
    }
#undef DEF_PRESET

    bool Fmod4Driver::setEaxPreset(const Ogre::String& name)
    {
        EaxPresetMap::iterator it = mEaxPresetMap.find(name);

        if (it != mEaxPresetMap.end())
        {
            FMOD_RESULT result;
            result = mFmod4System->setReverbProperties(&(it->second)); ///@todo seems not to work correctly
            CHECK_FMOD4_ERRORS(result);
        }

        return false;
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
        for (int i = 0; i < numPlugins; i++)
        {
            char pluginName[128];
            unsigned int version;
            FMOD_PLUGINTYPE type = FMOD_PLUGINTYPE_CODEC;
            unsigned int handle;
            FMOD_RESULT result = mFmod4System->getPluginHandle(FMOD_PLUGINTYPE_CODEC, i, &handle);
            CHECK_FMOD4_ERRORS(result);

            result = mFmod4System->getPluginInfo(handle, &type, pluginName, 127, &version);
            CHECK_FMOD4_ERRORS(result);
            LOG_MESSAGE(Logger::MULTIMEDIA,
                String("Fmod4Driver Plugin '") + pluginName + "' Version " + StringConverter::toString(version));
        }
    }

    void Fmod4Driver::printDriverInfo(int driver) const
    {
        int curdriver;
        mFmod4System->getDriver(&curdriver);

        char name[128];
        mFmod4System->getDriverInfo(driver, name, 127, NULL);

        FMOD_CAPS caps;
        int minfreq, maxfreq;
        FMOD_SPEAKERMODE speakermode;
        mFmod4System->getDriverCaps(driver, &caps, &minfreq, &maxfreq, &speakermode);

        LOG_MESSAGE(Logger::MULTIMEDIA,
            String("Fmod4Driver '") + name + "'" + (driver == curdriver ? "(cur)" : "") + ": \n" + "Freq "
                + StringConverter::toString(minfreq) + " - " + StringConverter::toString(maxfreq));

        int hard2d, hard3d, hardtotal;
        mFmod4System->getHardwareChannels(&hard2d, &hard3d, &hardtotal);
        LOG_MESSAGE(Logger::MULTIMEDIA,
            String("Fmod4Driver '") + name + "' Hardware Channels: " + "2D #" + StringConverter::toString(hard2d)
                + ", 3D #" + StringConverter::toString(hard3d) + ", Total #" + StringConverter::toString(hardtotal));

        int softchannels;
        mFmod4System->getSoftwareChannels(&softchannels);
        LOG_MESSAGE(Logger::MULTIMEDIA,
            String("Fmod4Driver '") + name + "' Software Channels: " + "#" + StringConverter::toString(softchannels));

        FMOD_OUTPUTTYPE output;
        mFmod4System->getOutput(&output);
        String outputName;

        if (output == FMOD_OUTPUTTYPE_AUTODETECT)
            outputName = "Autodetect";
        else if (output == FMOD_OUTPUTTYPE_UNKNOWN)
            outputName = "Unknown";
        else if (output == FMOD_OUTPUTTYPE_NOSOUND)
            outputName = "No Sound";
        else if (output == FMOD_OUTPUTTYPE_WAVWRITER)
            outputName = "Wave Writer";
        else if (output == FMOD_OUTPUTTYPE_NOSOUND_NRT)
            outputName = "No Sound non realtime";
        else if (output == FMOD_OUTPUTTYPE_WAVWRITER_NRT)
            outputName = "Wave Writer non realtime";
        else if (output == FMOD_OUTPUTTYPE_DSOUND)
            outputName = "DirectSound";
        else if (output == FMOD_OUTPUTTYPE_WINMM)
            outputName = "Windows Multimedia";
        else if (output == FMOD_OUTPUTTYPE_ASIO)
            outputName = "Low Latency ASIO driver";
        else if (output == FMOD_OUTPUTTYPE_OSS)
            outputName = "Open Sound System (OSS)";
        else if (output == FMOD_OUTPUTTYPE_ALSA)
            outputName = "Advanced Linux Sound Architecture (ALSA)";
        else if (output == FMOD_OUTPUTTYPE_ESD)
            outputName = "Enlightment Sound Daemon (ESD)";
        else if (output == FMOD_OUTPUTTYPE_SOUNDMANAGER)
            outputName = "Macintosh SoundManager";
        else if (output == FMOD_OUTPUTTYPE_COREAUDIO)
            outputName = "Macintosh CoreAudio";

        LOG_MESSAGE(Logger::MULTIMEDIA, String("Fmod4Driver '") + name + "' Output: " + outputName);
    }

    FMOD_RESULT F_CALLBACK Fmod4Driver::open(
        const char* name, int unicode, unsigned int* filesize, void** handle, void** userdata)
    {
        SoundResourcePtr* res = new SoundResourcePtr(gSoundResourceManager->getByName(name));
        LOG_DEBUG(Logger::MULTIMEDIA, "Opened stream " + (*res)->getName());
        (*res)->load();
        *filesize = (*res)->getSize();
        *handle = res;
        return FMOD_OK;
    }

    FMOD_RESULT F_CALLBACK Fmod4Driver::close(void* handle, void* userdata)
    {
        SoundResourcePtr* res = static_cast<SoundResourcePtr*>(handle);
        LOG_DEBUG(Logger::MULTIMEDIA, "Closing stream " + (*res)->getName());
        (*res)->unload();
        delete res;
        return FMOD_OK;
    }

    FMOD_RESULT F_CALLBACK Fmod4Driver::read(
        void* handle, void* buffer, unsigned int sizebytes, unsigned int* bytesread, void* userdata)
    {
        if (handle == NULL)
        {
            return FMOD_ERR_INVALID_PARAM;
        }

        SoundResourcePtr* sound = static_cast<SoundResourcePtr*>(handle);
        if (*sound)
        {
            if (!(*sound)->isLoaded())
            {
                (*sound)->load();
            }

            DataStreamPtr ds = (*sound)->getDataStream();
            if (ds)
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

    FMOD_RESULT F_CALLBACK Fmod4Driver::seek(void* handle, unsigned int pos, void* userdata)
    {
        if (handle == NULL)
        {
            return FMOD_ERR_INVALID_PARAM;
        }

        SoundResourcePtr* sound = static_cast<SoundResourcePtr*>(handle);
        if (*sound)
        {
            DataStreamPtr ds = (*sound)->getDataStream();
            if (ds)
            {
                ds->seek(pos);
                return FMOD_OK;
            }
        }
        return FMOD_ERR_INVALID_PARAM;
    }

    /**
     * This static method is as channel callback, so that we get callbacks
     * from FMOD
     * @author Blakharaz
     * @version 1.0
     * @author JoSch
     * @version 1.1
     * @date 07-03-2007
     * @param _channel The channel for which the callback is registered.
     * @param type Type of the event.
     * @param command A commando
     * @param commanddata1 Data
     * @param commanddata2 Data
     */
    FMOD_RESULT F_CALLBACK Fmod4Driver::channelCallback(
        FMOD_CHANNEL* _channel, FMOD_CHANNEL_CALLBACKTYPE type, void* commanddata1, void* commanddata2)
    {
        /// Extract the Fmod channel and then our Sound object.
        FMOD::Channel* channel = (FMOD::Channel*)_channel;
        RlAssert1(channel != NULL);
        ChannelSoundMap::iterator it = gDriver->mChannelSoundMap.find((FMOD::Channel*)channel);
        if (it != gDriver->mChannelSoundMap.end())
        {
            Fmod4Sound* sound = it->second;
            RlAssert1(sound != NULL);
            switch (type)
            {
            case FMOD_CHANNEL_CALLBACKTYPE_END:
            {
                // We dispatch a stop event
                SoundPlayEvent event(NULL, SoundPlayEvent::STOPEVENT);
                sound->dispatchEvent(&event);
                // If the sound is set for autodestruction,
                // we destroy it now.
                if (sound->isAutoDestroying())
                {
                    gDriver->destroySound(sound);
                    gDriver->mChannelSoundMap.erase(it);
                }
            }
            break;

            case FMOD_CHANNEL_CALLBACKTYPE_SYNCPOINT:
            {
                FMOD::Sound* fmodsound = NULL;
                FMOD_SYNCPOINT* syncpoint = NULL;
                SoundTimingEvent event(NULL, SoundTimingEvent::TIMEEVENT);

                // We get the time point of the sync point and put it in a timing event.
                sound->getFmodChannel()->getCurrentSound(&fmodsound);
                fmodsound->getSyncPoint(*static_cast<int*>(commanddata1), &syncpoint);
                fmodsound->getSyncPointInfo(syncpoint, NULL, 0, &event.mTime, FMOD_TIMEUNIT_MS);
                sound->dispatchEvent(&event);
            }
            break;
            case FMOD_CHANNEL_CALLBACKTYPE_VIRTUALVOICE:
            {
            }
            break;
            }
        }
        return FMOD_OK;
    }

    void Fmod4Driver::setMasterVolume(const Ogre::Real& vol)
    {
        SoundDriver::setMasterVolume(vol);
    }

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

    void Fmod4Driver::_registerChannel(FMOD::Channel* channel, Fmod4Sound* sound)
    {
        FMOD_RESULT res = channel->setCallback(channelCallback);
        CHECK_FMOD4_ERRORS(res);
        mChannelSoundMap.insert(std::make_pair(channel, sound));
    }
}
