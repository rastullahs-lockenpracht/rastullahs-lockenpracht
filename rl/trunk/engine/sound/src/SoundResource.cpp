/* SoundResource.cpp - Diese Klassse kapselt einen Sound.
 * (C) 2004. Team Pantheon. www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#include <string>
#include <iostream>
#include "SoundManager.h"
#include "SoundResource.h"
#include "Sleep.h"

using namespace std;
using namespace Ogre;

namespace rl {
/**
 * @author JoSch
 * @date 07-23-2004
 */
SoundResource::SoundResource(const String &name):
    Resource(),
    mFadeInThread(this, true),
    mFadeOutThread(this, false),
    mStreamThread(this)
{
    mName = name;
    alGenSources(1, &mSource);
    check();
    setGain(1.0f);
    setPosition(Vector3(0.0, 0.0, 0.0));
    setVelocity(Vector3(0.0, 0.0, 0.0));
    setDirection(Vector3(0.0, 0.0, 0.0));
}

/**
 * @author JoSch
 * @date 07-24-2004
 */
SoundResource::~SoundResource()
{
    alDeleteBuffers(mBufferCount, mBuffers);
    delete[] mBuffers;
    alDeleteSources(1, &mSource);
}

/**
 * @return Die aktuelle Richtung der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const Vector3 SoundResource::getDirection() const throw (RuntimeException)
{
    Vector3 result;
    alGetSource3f(mSource, AL_DIRECTION,
        &result.x, &result.y, &result.z);
    check();
    return result;
}

/**
 * @param direction Die neue Richtung der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::setDirection (const Vector3& direction) throw (RuntimeException)
{
    alSource3f(mSource, AL_DIRECTION,
        direction[0], direction[1], direction[2]);
    check();
}

/**
 * @return Die aktuelle Position der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const Vector3 SoundResource::getPosition() const throw (RuntimeException)
{
    Vector3 result;
    alGetSource3f(mSource, AL_POSITION,
        &result.x, &result.y, &result.z);
    check();
    return result;
}

/**
 * @param position Die neue Position der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::setPosition(const Vector3& position) throw (RuntimeException)
{
    alSource3f(mSource, AL_POSITION,
        position[0], position[1], position[2]);
    check();
}

/**
 * @return Die aktuelle Geschwindigkeit der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const Vector3 SoundResource::getVelocity() const throw (RuntimeException)
{
    Vector3 result;
    alGetSource3f(mSource, AL_VELOCITY,
        &result.x, &result.y, &result.z);
    check();
    return result;
}

/**
 * @param velocity Die neue Geschwindigkeit der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::setVelocity(const Vector3& velocity) throw (RuntimeException)
{
    alSource3f(mSource, AL_VELOCITY,
        velocity[0], velocity[1], velocity[2]);
    check();
}

/**
 * @return Die aktuelle Lautstaerke der Soundquelle
 * @author JoSch
 * @date 07-23-2004
 */
const ALfloat SoundResource::getGain() const throw (RuntimeException)
{
    mGainMutex.lock();
    ALfloat result;
    mGainMutex.lock();
    alGetSourcef(mSource, AL_GAIN, &result);
    mGainMutex.unlock();
    check();
    return result;
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::setGain(const ALfloat gain) throw (RuntimeException)
{
    mGainMutex.lock();
    alSourcef(mSource, AL_GAIN, gain);
    mGainMutex.unlock();
    check();
}

/** Laedt die Soundquelle und setzt die Werte.
 * @author JoSch
 * @date 10-11-2004
 */
void SoundResource::load()
{
}

/** Entlaedt die Soundquelle und setzt die Werte.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::unload()
{
    if (mIsLoaded)
    {
        stop();
        alSourceUnqueueBuffers(mSource, mBufferCount, mBuffers);
        // Fehler zuruecksetzen.
        alGetError();
    }
    mIsLoaded = false;
}

/**
 * @author JoSch
 * @date 09-17-2004
 */
void SoundResource::fadeIn(unsigned int msec)
{
    if (msec != 0)
    {
        // TODO mFadeInThread.mDuration = msec;
        mFadeInThread.start();
    }
}
/**
 * @author JoSch
 * @date 09-15-2004
 */
void SoundResource::fadeOut(unsigned int msec)
{
    if (msec != 0 )
    {
        // TODO mFadeOutThread.mDuration = msec;
        mFadeOutThread.start();
    }
}
/**
 * @author JoSch
 * @date 09-15-2004
 */
void SoundResource::play(unsigned int msec) throw (RuntimeException)
{
    if (getState() != AL_PLAYING)
    {
        // Abspielen.
        alSourcePlay(mSource);
        check();
        fadeIn(msec);
    }
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::pause() throw (RuntimeException)
{
    alSourcePause(mSource);
    check();
}

/**
 * @author JoSch
 * @date 09-15-2004
 */
void SoundResource::stop(unsigned int msec) throw (RuntimeException)
{
    fadeOut(msec);
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::rewind() throw (RuntimeException)
{
    alSourceRewind(mSource);
    check();
}

/**
 * @return Der momentane Zustand der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
const ALenum SoundResource::getState() const throw (RuntimeException)
{
    ALenum result;
    alGetSourcei(mSource, AL_SOURCE_STATE, &result);    
    check();
    return result;
}

/**
 * @return Ueberprueft, ob Fehler aufgetreten ist, ansonsten Exception.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::check() const throw (RuntimeException)
{
    ALenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        Throw(RuntimeException, string((char*)alGetString(error)));
    }
}

/**
 * @param duration. Die Zeit in msek., die der Fade In insgesamt dauern soll.
 * @param time. Die Zeit in msek., wo der Fade In gerade ist
 * @param gain. Der aktuelle Lautstaerkewert, vom dem ausgegangen wird.
 * @author JoSch
 * @09-15-2004
 */
ALfloat SoundResource::calculateFadeIn(unsigned RL_LONGLONG duration, unsigned RL_LONGLONG time,
                ALfloat gain)
{
    try {
        ALfloat x = (time * 1.0f) / duration;
        return (1.0f - exp(-x)) * gain;
    } catch(...)
    {
        return gain;
    }
}

/**
 * @param duration. Die Zeit in msek., die der Fade Out insgesamt dauern soll.
 * @param time. Die Zeit in msek., wo der Fade Out gerade ist
 * @param gain. Der aktuelle Lautstaerkewert, vom dem ausgegangen wird.
 * @author JoSch
 * @09-15-2004
 */
ALfloat SoundResource::calculateFadeOut(unsigned RL_LONGLONG duration, unsigned RL_LONGLONG time,
                ALfloat gain)
{
    try {
        ALfloat x = (time * 2.0f) / duration;
        return exp(-x) * gain;
    } catch(...)
    {
        return (ALfloat)0.0f;
    }
}

/**
 * Bestimmt, ob der Sound spielen sollte, d.h.
 * ob der Streamthread laufen sollte.
 * Dies hat nicht mit getState von OpenAL zu tun.
 * @return True, wenn der Sound spielen sollte.
 * @author JoSch
 * @date 10-11-2004
 */
const bool SoundResource::isPlaying() const
{
    return true; // TODO
}



/**
 */
SoundResource::FadeThread::FadeThread(SoundResource *that, bool fadeIn)
    : that(that),
      mFadeIn(fadeIn)
{
}

/**
 * Fuehrt das Fade aus
 * @author JoSch
 * @date 09-16-2004
 */
void SoundResource::FadeThread::run()
{
    if (mFadeIn)
    {
        if (mDauer != 0)
        {
            // Alte Lautstaerke speichern.
            ALfloat gain = that->getGain();
            
            that->setGain(0.0f);
            for(unsigned int time = 0; time <= mDauer; time += 10)
            {
                
                // Warten
                msleep(10);
                ALfloat newgain = that->calculateFadeIn(mDauer, time, gain);
                // Lautstaerke hochsetzen.
                that->setGain((newgain > gain)?gain:newgain);
            }
            that->setGain(gain);
            that->check();
        }
    } else {
        if (mDauer != 0)
        {
            
            // Alte Lautstaerke speichern.
            ALfloat gain = that->getGain();
            
            for (unsigned int time = 0; time <= mDauer; time += 10)
            {
                // Warten
                msleep(1);
                ALfloat newgain = that->calculateFadeOut(mDauer, time, gain);
                // Lautstaerke hochsetzen.
                that->setGain((newgain > gain)?gain:newgain);
            }
            that->setGain(0.0f);
            that->check();
            
        }
        // Stoppen.
        alSourceStop(that->mSource);
        that->check();
    }
    
    
}

/**
 * Das Streamen passiert in diesem Thread.
 * @author JoSch
 * @date 10-11-2004
 */
SoundResource::StreamThread::StreamThread(SoundResource *that)
    : that(that)
{
}

/**
 * Fuehrt das Streamen aus
 * @author JoSch
 * @date 10-11-2004
 */
void SoundResource::StreamThread::run()
{
}

}
