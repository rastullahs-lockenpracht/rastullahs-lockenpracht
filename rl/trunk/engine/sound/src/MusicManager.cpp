/* Copyright 2004 Jochen Schaefer */

#include "MusicManager.h"
#include "MusicResource.h"
#include <iostream>
#include <boost/thread/xtime.hpp>

using namespace Ogre;
using namespace std;
using namespace boost;

/** Das Singleton
 * @author JoSch
 * @date 04-12-2004
 */
template<> rl::MusicManager* Singleton<rl::MusicManager>::ms_Singleton = 0;

namespace rl {


/**
 * @return Eine Referenz auf das Singleton-Objekt.
 * @author JoSch
 * @date 04-12-2004
 */
MusicManager& MusicManager::getSingleton(void)
{
    return Singleton<MusicManager>::getSingleton();
}

/**
 * @return Ein Zeiger auf das Singleton-Objekt.
 * @author JoSch
 * @date 04-12-2004
 */
MusicManager* MusicManager::getSingletonPtr(void)
{
    return Singleton<MusicManager>::getSingletonPtr();
}

/**
 * @author JoSch
 * @date 04-12-2004
 * @date 06-17-2004
 */
MusicManager::MusicManager() : ResManager(),
    mSource(0),
    mLooping(false),
    mAuto(false),
    mShouldPlay(false),
    mShouldExit(false),
    mMusicThread()
{ 
    mThread = new thread(mMusicThread);
//    mThread->join();
}

/**
 * @author JoSch
 * @date 04-12-2004
 */
MusicManager::~MusicManager()
{
    if (mSource != 0)
    {
        mSource->stop();
    }
    mSource = 0;
    mShouldExit = true;
    delete mThread;
    
}

/**
 * Gibt die Suchmuster fuer die Extension zurueck.
 * @author JoSch
 * @date 06-18-2004
 */
StringList MusicManager::getExtension()
{
    StringList result;
    result.push_back("*.ogg");
    
    return result;
}

/**
 * Stoppt den aktuellen Song. Er kann danach wieder
 * fortgesetzt werden.
 * @author JoSch
 * @date 04-12-2004
 */
void MusicManager::stopSong()
{
    if (mSource != 0)
    {
        mSource->stop();
        mShouldPlay = false;
    }
}

/**
 * Spielt den aktuellen Song ab, wenn moeglich.
 * @author JoSch
 * @date 04-12-2004
 */
void MusicManager::playSong()
{
    if (mSource == 0)
    {
        setNextSong();
    }
    // Wenn mSource immer noch NULL ist, dann haben wir nicht ins der Liste.
    if (mSource != 0)
    {
        mSource->play();
        mShouldPlay = true;
    } else {
        mShouldPlay = false;
    }
}


/**
 * @return liefert FALSE, wenn es noch keine Quelle gibt, ansonsten je nachdem ob noch spielt oder nicht.
 * @author JoSch
 * @date 07-16-2004
 */
const bool MusicManager::isSourcePlaying() const
{
    if (mSource == 0)
    {
        return false;
    }
    return mSource->isPlaying(); 
}

/**
 * @return Gibt an, ob der Musikmanager noch spielen will.
 * @author JoSch
 * @date 07-16-2004
 */
const bool MusicManager::isPlaying() const
{
    return mShouldPlay;
}

/**
 * Stoppt den aktuellen Song, hole den naechsten Song.
 * @exception IllegalArgumentException Wird geworfen, wenn ein
 * Fehler auftritt.
 * @author JoSch
 * @date 04-12-2004
 */
void MusicManager::setNextSong()
{
    MusicResource *next = findNextSong();
    // Evtl. spielt noch ein Song.
    if (mSource != 0 )
    {
        mSource->stop();
        mSource->unload();
        mSource = 0;
    }
    RlAssert(mSource == 0, "Fehler beim Stoppen des aktuellen Musikstuecks");
    // Wir setzen jetzt den naechsten Song.
    mSource = next;
    if (mSource != 0) // Was gefunden.
    {
        mSource->load();        
    } else {
       mShouldPlay = false;
    }
}


/**
 * Finde den Nachfolger des Eintrags in der Liste mit dem passenden Namen.
 * @param aSong Name des gesuchten Songs.
 * @return Die gefundene Resource.
 * @author JoSch
 * @date 04-12-2004
 */
MusicResource* MusicManager::findNextSong()
{
    if (mSource != 0)
    {
        ResourceMapIterator cit = getResourceIterator();
        while(cit.hasMoreElements())
        {
            if (cit.peekNextKey() == mSource->getHandle())
            {
                cit.moveNext();
                if (cit.peekNextValue() == 0)
                {
                    // Nichts gefunden.
                    if (mLooping)
                    {
                        return dynamic_cast<MusicResource*>(getResourceIterator().peekNextValue());
                    } else {
                        // Nicht wiederholen.
                        return 0;
                    }
                }
                return dynamic_cast<MusicResource*>(cit.peekNextValue());
            } else {
                cit.moveNext();
            }
        }
    } else { // mSource ist noch nicht gesetzt.
        return dynamic_cast<MusicResource*>(getResourceIterator().peekNextValue());
    }
    return 0;
}

/**
 * @param newGain Neue relative Lautstaerke.
 * @author JoSch
 * @date 04-12-2004
 */
void MusicManager::setGain(ALfloat newGain)
{
    if (mSource != 0)
    {
        mSource->setGain(newGain);
    }
}

/**
 * @return Liefert aktuelle relative Laustaerke.
 * @author JoSch
 * @date 04-12-2004
 */
ALfloat MusicManager::getGain()
{
    if (mSource != 0)
    {
        return mSource->getGain();
    }
    return 0.0;
}

/**
 * @param isAuto Wenn TRUE, dann automatisch  weiterschalten, ansonsten nicht.
 * @author JoSch
 * @date 06-25-2004
 */
void MusicManager::setAuto(bool isAuto)
{
    mAuto = isAuto;
}

/**
 * @return Liefert, ob auf Autopilot.
 * @author JoSch
 * @date 06-25-2004
 */
bool MusicManager::isAuto()
{
    return mAuto;
}

/**
 * @param looping Wenn TRUE, dann automatisch  wiederholen, ansonsten nicht.
 * @author JoSch
 * @date 06-25-2004
 */
void MusicManager::setLooping(bool looping)
{
    mLooping = looping;
}

/**
 * @return Liefert, ob wiederholen.
 * @author JoSch
 * @date 06-25-2004
 */
bool MusicManager::isLooping()
{
    return mLooping;
}

/**
 * Der Konstruktor
 * @author JoSch
 * @date 07-25-2004
 */
MusicManager::MusicThread::MusicThread()
{
}

MusicManager::MusicThread::~MusicThread()
{
}


/**
 * Der Thread ueberprueft, ob das Musikstueck noch lauft und wenn
 * nicht, ob er laufen sollte. Wenn er laufen sollte, wird das naechste
 * Stueck abgespielt.
 * @author JoSch
 * @date 07-25-2004
 */
void MusicManager::MusicThread::operator()()
{
    xtime xt;
    MusicManager *that = MusicManager::getSingletonPtr();
    if (that == 0)
    {
        return;
    }
    while (!that->mShouldExit)
    {
        try {
            if (that->mShouldPlay)
            {
                xtime_get(&xt, TIME_UTC);
                xt.nsec += 1 * 1000;
                thread::sleep(xt);
                // Spielt der Song noch?
                if (!that->isSourcePlaying()) // Nein, spielt nicht
                {
                    if (that->isAuto())
                    {
                        // Automatisch weitermachen.
                        that->setNextSong();
                        if (!that->mShouldPlay)
                        {
                            continue;
                        }
                        that->playSong();
                    } else {
                        that->stopSong();
                    }
                } else { //Doch, spielt noch
                }
            }                     
        } catch(...)
        {
        }
        xtime_get(&xt, TIME_UTC);
        xt.nsec += 1 * 1000;
        thread::sleep(xt);
    }
}

/**
 * @param resName. Der Name der Soundresource, die erzeugt
 * werden soll.
 * @author JoSch
 * @date 06-18-2004
 */
Resource* MusicManager::create(const String& resName)
{
    MusicResource *newSound = new MusicResource(resName);
    return newSound;
}



}
