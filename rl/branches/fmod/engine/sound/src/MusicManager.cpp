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
#include "SoundManager.h"

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
MusicManager::MusicManager():
    mSource(0),
    mLooping(false),
    mAuto(false),
    mShouldPlay(false),
    mShouldExit(false),
    mMusicThread(0),
    mMusicFunctor(),
    mPlayList()
{
    mMusicThread = new thread(mMusicFunctor);
}

/**
 * @author JoSch
 * @date 04-12-2004
 */
MusicManager::~MusicManager()
{
    if (!mSource.isNull())
    {
        mSource->stop();
    }
    mSource.setNull();
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
    
    return result;
}

/**
 * Stoppt den aktuellen Song. Er kann danach wieder
 * fortgesetzt werden.
 * @author JoSch
 * @date 04-12-2004
 */
void MusicManager::stopSong(int fade)
{
    if (!mSource.isNull())
    {
        mSource->stop(fade);
    }
    mShouldPlay = false;
}

/**
 * Spielt den aktuellen Song ab, wenn moeglich.
 * @author JoSch
 * @date 04-12-2004
 */
void MusicManager::playSong(int fade)
{
    if (mSource.isNull())
    {
        setNextSong();
    }
    // Wenn mSource immer noch NULL ist, dann haben wir nicht ins der Liste.
    if (!mSource.isNull())
    {
        mSource->play(fade);
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
    if (mSource.isNull())
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
    string name = findNextSong();
    // Evtl. spielt noch ein Song.
    SoundMovablePtr next;
    if (name != "")
    {
        next = SoundMovablePtr(new SoundMovable(
            SoundManager::getSingleton().getByName(name)));
    }
    if (!mSource.isNull() )
    {
        mSource->stop();
        mSource.setNull();
    }
    RlAssert(mSource.isNull(), "Fehler beim Stoppen des aktuellen Musikstuecks");
    // Wir setzen jetzt den naechsten Song.
    mSource = next;
    if (!mSource.isNull()) // Was gefunden.
    {
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
string MusicManager::findNextSong()
{
    if (!mSource.isNull())
    {
        StringList::iterator cit = 
            find(mPlayList.begin(), mPlayList.end(), mSource->getName());
        if (cit == mPlayList.end())
        {
            // Nichts gefunden.
            if (mLooping)
            {
                SoundResourcePtr res = 
                    SoundManager::getSingleton().getResourceIterator().peekNextValue();
                if (!res.isNull())
                    return res->getName();

				return "";
             }
              
			// Nicht wiederholen.
            return "";

         } else {
            cit++;
            if (cit != mPlayList.end())
            {
                return *cit;
            } 

			return "";
         }
    } else { // mSource ist noch nicht gesetzt.
        SoundResourcePtr res = SoundManager::getSingleton().getResourceIterator().peekNextValue();
        if (!res.isNull())
            return res->getName();
        
        return "";
        
    }
    return "";
}

/**
 * @param newGain Neue relative Lautstaerke.
 * @author JoSch
 * @date 04-12-2004
 */
void MusicManager::setGain(float newGain)
{
    if (!mSource.isNull())
    {
        mSource->setGain(newGain);
    }
}

/**
 * @return Liefert aktuelle relative Laustaerke.
 * @author JoSch
 * @date 04-12-2004
 */
float MusicManager::getGain()
{
    if (!mSource.isNull())
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
MusicManager::MusicFunctor::MusicFunctor()
{}


/**
 * Der Thread ueberprueft, ob das Musikstueck noch lauft und wenn
 * nicht, ob er laufen sollte. Wenn er laufen sollte, wird das naechste
 * Stueck abgespielt.
 * @author JoSch
 * @date 07-25-2004
 */
void MusicManager::MusicFunctor::operator()()
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
                xtime_get(&xt, TIME_UTC);
                xt.nsec += 1000;
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
        xtime_get(&xt, TIME_UTC);
        xt.nsec += 1000000;
        thread::sleep(xt);
    } 
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
    SoundManager::ResourceMapIterator it = SoundManager::getSingleton().getResourceIterator();
    while (it.hasMoreElements())
    {
        ResourcePtr res = it.getNext();
		mPlayList.push_back(res->getName());
    }
}

}
