/* MusicManager.cpp - Spielt eine Playlist ab.
 * (C) 2003-2005. Team Pantheon. www.team-pantheon.de
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

#include "MusicManager.h"
#include "SoundSubsystem.h"

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
MusicManager::MusicManager() : ResourceManager(),
    mSource(0),
    mLooping(false),
    mAuto(false),
    mShouldPlay(false),
    mShouldExit(false),
    mMusicThread(0),
    mPlayList()
{
    mMusicThread = new MusicThread();
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
    result.push_back("*.wav");
    
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
    SoundResource *next = findNextSong();
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
SoundResource* MusicManager::findNextSong()
{
    if (mSource != 0)
    {
        StringList::iterator cit = 
            find(mPlayList.begin(), mPlayList.end(), mSource->getName());
        if (cit == mPlayList.end())
        {
            // Nichts gefunden.
            if (mLooping)
            {
                SoundResource *temp = dynamic_cast<SoundResource*>(getResourceIterator().peekNextValue());
                if (temp != 0)
                {
                    mPlayList.push_back(temp->getName());
                }
                return temp;
             } else {
                // Nicht wiederholen.
                return 0;
             }
         } else {
            cit++;
        }
    } else { // mSource ist noch nicht gesetzt.
        SoundResource *temp = dynamic_cast<SoundResource*>(getResourceIterator().peekNextValue());
        if (temp != 0)
        {
            mPlayList.push_back(temp->getName());
        }
        return temp;
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
                xtime xt;
                xt.sec = 0;
                xt.nsec = 1000;
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
        xtime xt;
        xt.sec = 0;
        xt.nsec = 1000;
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
    SoundResource *newSound = new SoundResource(resName);
    return newSound;
}

/**
 * @author JoSch
 * @date 01-26-2005
 */
void MusicManager::clearPlayList()
{
    mPlayList.clear();
}

/**
 * @author JoSch
 * @date 01-27-2005
 */
void MusicManager::addPlayList(std::string songName)
{
    mPlayList.push_back(songName);
}

/**
 * @author JoSch
 * @date 01-27-2005
 */
void MusicManager::addPlayList(StringList list)
{
    mPlayList.merge(list);
}

/**
 * @author JoSch
 * @date 01-27-2005
 */
void MusicManager::addSoundsIntoPlayList()
{
    ResourceMapIterator it = getResourceIterator();
    while (it.hasMoreElements())
    {
        Resource* element = it.getNext();
        mPlayList.push_back(element->getName());
    }
}

}
