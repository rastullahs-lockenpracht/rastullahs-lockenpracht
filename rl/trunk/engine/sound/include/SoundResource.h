/* SoundResource.h - Diese Klassse kapselt einen Sound.
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

#ifndef SOUNDRESOURCE_H
#define SOUNDRESOURCE_H

#include "SoundPrerequisites.h"
#include <Ogre.h>
#include <OpenThreads/Thread>
#include <OpenThreads/Mutex>

using namespace OpenThreads;

// @TODO: Callbacks für Threads einfuehren.

namespace rl {
 
/**
 * Der Typ der Sounddaten.
 * Momentan nur Wave und OggVorbis.
 */
enum SoundDataType { Wave, OggVorbis };
 
/** Diese Basisklasse kapselt eine OpenAl++-Source fuer
 * den ResourceManager von Ogre
 * @author Josch
 * @date 06-19-2004
 * @date 07-23-2004
 * @date 10-10-2004
 * @version 3.0
 */
class _RlSoundExport SoundResource: public Resource {
    private:
        /// Diese Klasse kapselt das Fade in.
        class FadeThread : public Thread {
            private:
                /// Damit wir wissen, wo wir hinwollen ;-)
                SoundResource *that;
                /// Fade In oder Out
                bool mFadeIn;
                /// Dauer des Fades in msek.
                unsigned long int mDauer;
            public:
                /// Der Konstruktor.
                FadeThread(SoundResource *that, bool fadeIn);
                /// Die Arbeitsroutine.
                void run();

        } mFadeInThread, mFadeOutThread;
       
        /// Streamen der Sounddaten
        class StreamThread : public Thread {
            private:
                /// Damit wir wissen, wo wir hinwollen ;-)
                SoundResource *that;
                /// Die Daten, die wir streamen wollen.
                ALbyte *mData;
                /// Der Typ der Sounddaten.
                SoundDataType mDataType;
            public:
                /// Der Konstruktor.
                StreamThread(SoundResource *that);
                /// Die Arbeitsroutine.
                void run();

        } mStreamThread;
    
    protected:
        /// Die gekapselte Soundquelle
        ALuint mSource;
        /// Die Buffer, die wir benutzen
        ALuint *mBuffers;
        /// Wieviele Buffer werden benutzt.
        const short mBufferCount;
        
        /// Welches Soundformat hat dieser Sound.
        ALsizei mFormat;
        /// Wie gross ist der Sound in Bytes.
        ALsizei mSize;
        /// Wie gross ist ein Sample in Bits.
        ALsizei mBits;
        /// Mit welcher Frequenz wurde gesampelt.
        ALsizei mFrequency;
        /// Loopen?
        ALboolean mLoop;
        /// Unsere Daten von Ogres ResourceManager.
        DataChunk *mData;
        /// Die Art des Sounds.
        SoundDataType mSoundDataType;

        /// Ueberpruefen, ob Fehler aufgetreten ist.
        void check() const throw (RuntimeException);
        
        /// Mutex zum Synchronisieren von Gain-Zugriffen.
        mutable Mutex mGainMutex;
        
        /// Fuehre das Fade-In aus
        void fadeIn(unsigned int msec);
        /// Fuehre das Fade-Out aus
        void fadeOut(unsigned int msec);
        
        /// Berechne den Anstieg der Lautstarke beim Fade-In
        ALfloat calculateFadeIn(unsigned RL_LONGLONG duration, unsigned RL_LONGLONG time,
                ALfloat gain);

        /// Berechne die Abnahme der Lautstarke beim Fade-Out
        ALfloat calculateFadeOut(unsigned RL_LONGLONG duration, unsigned RL_LONGLONG time,
                ALfloat gain);

    public:
        /// Der Standardkonstruktor
        SoundResource(const Ogre::String& name);
        /// Der Destruktor
        virtual ~SoundResource();
        /// Gibt die eingestellte Position der Soundquelle zurueck
        const Ogre::Vector3 getPosition() const throw (RuntimeException);
        /// Setzt die Position der Soundquelle.
        void setPosition(const Ogre::Vector3& direction) throw (RuntimeException);
        /// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck
        const ALfloat getGain() const throw (RuntimeException);
        /// Setzt die relative Lautstaerke der Soundquelle.
        void setGain(const ALfloat gain) throw (RuntimeException);
        /// Gibt die Richtung der Soundquelle zurueck.
        const Ogre::Vector3 getDirection() const throw (RuntimeException);
        /// Gibt die Geschwindigkeit der Soundquelle zurueck.
        const Ogre::Vector3 getVelocity() const throw (RuntimeException);
        /// Setzt die Richtung der Soundquelle.
        void setDirection(const Ogre::Vector3&) throw (RuntimeException);
        /// Setzt die Geschwindigkeit der Soundquelle.
        void setVelocity(const Ogre::Vector3&) throw (RuntimeException);
        /// Laedt die Soundquelle.
        virtual void load();
        /// Entlaedt die Soundquelle.
        virtual void unload();
        
        /// Spielt den Sound ab.
        virtual void play(unsigned int msec = 0) throw (RuntimeException);
        /// Pausiert den Sound.
        virtual void pause() throw (RuntimeException);
        /// Stoppt den Sound.
        virtual void stop(unsigned int msec = 0) throw (RuntimeException);
        /// Zurueck auf Anfang.
        virtual void rewind() throw (RuntimeException);
        /// Den Zustand des Sounds in OpenAL zurueckgeben.
        const ALenum getState() const throw (RuntimeException);
        /// Spielt der Stream noch. Nicht verwechseln mit getState.
        const bool isPlaying() const;
        
        
};

}
#endif
