#include <string>
#include <iostream>
#include "SoundManager.h"
#include "SndResource.h"

using namespace std;
using namespace Ogre;

namespace rl {
/**
 * @author JoSch
 * @date 07-23-2004
 */
SndResource::SndResource(const String &name):
    Resource()
{
    mName = name;
    alGenSources(1, &mSource);
    check();
    setGain(1.0);
    setPosition(Vector3(0.0, 0.0, 0.0));
    setVelocity(Vector3(0.0, 0.0, 0.0));
    setDirection(Vector3(0.0, 0.0, 0.0));
}

/**
 * @author JoSch
 * @date 07-24-2004
 */
SndResource::~SndResource()
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
const Vector3 SndResource::getDirection() const throw (RuntimeException)
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
void SndResource::setDirection (const Vector3& direction) throw (RuntimeException)
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
const Vector3 SndResource::getPosition() const throw (RuntimeException)
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
void SndResource::setPosition(const Vector3& position) throw (RuntimeException)
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
const Vector3 SndResource::getVelocity() const throw (RuntimeException)
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
void SndResource::setVelocity(const Vector3& velocity) throw (RuntimeException)
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
const ALfloat SndResource::getGain() const throw (RuntimeException)
{
    ALfloat result;
    alGetSourcef(mSource, AL_GAIN, &result);
    check();
    return result;
}

/**
 * @param gain Die neue Lautstarke der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
void SndResource::setGain(const ALfloat gain) throw (RuntimeException)
{
    alSourcef(mSource, AL_GAIN, gain);
    check();
}

/** Entlaedt die Soundquelle und setzt die Werte.
 * @author JoSch
 * @date 07-23-2004
 */
void SndResource::unload()
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
 * @date 07-23-2004
 */
void SndResource::play() throw (RuntimeException)
{
    alSourcePlay(mSource);
    check();
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void SndResource::pause() throw (RuntimeException)
{
    alSourcePause(mSource);
    check();
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void SndResource::stop() throw (RuntimeException)
{
    alSourceStop(mSource);
    check();
}

/**
 * @author JoSch
 * @date 07-23-2004
 */
void SndResource::rewind() throw (RuntimeException)
{
    alSourceRewind(mSource);
    check();
}

/**
 * @return Der momentane Zustand der Soundquelle.
 * @author JoSch
 * @date 07-23-2004
 */
const ALenum SndResource::getState() const throw (RuntimeException)
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
void SndResource::check() const throw (RuntimeException)
{
    ALenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        Throw(RuntimeException, string((char*)alGetString(error)));
    }
}

}
