
#include "AudibleActor.h"
#include "SoundManager.h"
#include "Exception.h"
#include "SoundResource.h"

namespace rl {

const String AudibleActor::TYPENAME="AudibleActor";

/*
 * @param name Der Name des Aktors.
 * @author JoSch
 * @date 05-04-2004
 */
AudibleActor::AudibleActor( const String& name ) : Actor(name)
{
    // Nichts zu tun.
}

/*
 * @param name Der Name des Aktors.
 * @param pParentSceneNode Die SceneNode, an den der Aktor gebunden ist.
 * @author JoSch
 * @date 05-04-2004
 */
AudibleActor::AudibleActor( const String& name, SceneNode* pParentSceneNode) 
    : Actor(name,pParentSceneNode)
{
    // Nichts zu tun.
}

/*
 * @author JoSch
 * @date 05-04-2004
 */
AudibleActor::~AudibleActor()
{
    // Wir stoppen alle Sounds und entladen sie.
    SoundNameMap::iterator it;
    for(it = mMap.begin(); it != mMap.end(); it++)
    {
        SoundResource *sr = it->second;
        RlAssert(sr != 0, "SoundResource nicht vorhanden!");
        sr->stop();
        SoundManager::getSingletonPtr()->unload(sr);
    }
}

/*
 * @param name Der Name des hinzuzufuegenden Sounds.
 * @author JoSch
 * @date 07-04-2004
 */
void AudibleActor::addSound(const String &name)
{
    if (getSound(name) == 0)
    {
        mMap[name] = dynamic_cast<SoundResource*>(SoundManager::getSingletonPtr()->getByName(name));
        if (mMap[name] == 0)
        {
            mMap[name] = dynamic_cast<SoundResource*>(SoundManager::getSingletonPtr()->create(name));
            SoundManager::getSingletonPtr()->add(mMap[name]);
        }
    }
}

/*
 * @param name Der Name des zu spielenden Sounds.
 * @author JoSch
 * @date 05-04-2004
 */
void AudibleActor::playSound(const String &name)
{
	//Console::getSingleton().write("Beep " + name + "\n");
    SoundResource *sr = getSound(name);
    if (sr != 0)
    {
        if (!sr->isLoaded())
        {
            SoundManager::getSingletonPtr()->load(sr, 0);
        }
        sr->play();
    }
}

/*
 * @param name Der Name des zu stoppenden Sounds.
 * @author JoSch
 * @date 05-04-2004
 */
void AudibleActor::stopSound(const String &name)
{
    SoundResource *sr = const_cast<SoundResource*>(getSound(name));
    if (sr != 0)
    {
        sr->stop();
    }
}

/*
 * @param name Der Name des zu loeschenden Sounds.
 * @author JoSch
 * @date 05-04-2004
 */
void AudibleActor::deleteSound(const String &name)
{
    SoundNameMap::iterator it = mMap.find(name);
    if (it != mMap.end())
    {
        it->second->stop();
        SoundManager::getSingletonPtr()->unload(it->second);
        mMap.erase(it);
    }
}

/*
 * @param name Der Name des zuruecklieferten Sounds.
 * @return Liefert den Sound, NULL, wenn nicht vorhanden.
 * @author JoSch
 * @date 05-04-2004
 */
const SoundResource* AudibleActor::getSound(const String &name) const
{
    SoundNameMap::const_iterator it = mMap.find(name);
    if (it != mMap.end())
    {
        return it->second;
    } else {
        return 0;
    }
}

/*
 * @param name Der Name des zuruecklieferten Sounds.
 * @return Liefert den Sound, NULL, wenn nicht vorhanden.
 * @author JoSch
 * @date 05-04-2004
 */
SoundResource* AudibleActor::getSound(const String &name)
{
    SoundNameMap::const_iterator it = mMap.find(name);
    if (it != mMap.end())
    {
        return it->second;
    } else {
        return 0;
    }
}

/**
 * @param velocity Die neue Geschwindigkeit.
 * @author JoSch
 * @date 05-04-2004
 */
void AudibleActor::setVelocity(const Vector3& velocity)
{
    mVelocity = velocity;
    SoundNameMap::iterator it;
    for(it = mMap.begin(); it != mMap.end(); it++)
    {
        SoundResource *sr = it->second;
        RlAssert(sr != 0, "SoundResource nicht vorhanden");
        sr->setVelocity(Vector3(velocity[0], velocity[1], velocity[2]));
    }
}

bool AudibleActor::isSimpleOgreActor()
{
    return false;
}

/**
 * @return Die aktuelle Geschwindigkeit
 * @author JoSch
 * @date 05-04-2004
 */
const Vector3& AudibleActor::getVelocity() const
{
    return mVelocity;
}

/**
 * @param x, y, z Die neue Position des Sounds.
 * @author JoSch
 * @date 05-04-2004
 */
void AudibleActor::setPosition(Real x, Real y, Real z)
{
    Actor::setPosition(x, y, z);
    SoundNameMap::iterator it;
    for(it = mMap.begin(); it != mMap.end(); it++)
    {
        SoundResource *sr = it->second;
        RlAssert(sr != 0, "SoundResource nicht vorhanden");
        sr->setPosition(Vector3(x, y, z));
    }
}

/**
 * @param orientation Die neue Orientierung.
 * @author JoSch
 * @date 05-04-2004
 */
void AudibleActor::setOrientation(const Quaternion& orientation)
{
    Actor::setOrientation(orientation);
    SoundNameMap::iterator it;
    for(it = mMap.begin(); it != mMap.end(); it++)
    {
        SoundResource *sr = it->second;
        RlAssert(sr != 0, "SoundResource nicht vorhanden");
        sr->setDirection(Vector3(orientation.x, orientation.y, orientation.z));
    }
}

}
