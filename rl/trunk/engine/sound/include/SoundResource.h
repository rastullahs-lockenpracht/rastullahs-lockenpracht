/* SoundResource.h - Diese Klassse kapselt einen Sound.
 * (C) 2004. Team Pantheon. www.team-pantheon.de
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

#ifndef SOUNDRESOURCE_H
#define SOUNDRESOURCE_H

#include "SoundPrerequisites.h"
#include <Ogre.h>
#include <OpenThreads/Thread>
#include <OpenThreads/Mutex>
#include "EventSource.h"
#include "EventListener.h"
#include "EventCaster.h"
#include "SoundEvent.h"

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
class _RlSoundExport SoundResource: public Resource,
         public EventListener<SoundEvent> {
    private:
        /// Lesen der Vorbisdatei.
        static size_t VorbisRead(void *ptr, size_t byteSize, size_t sizeToRead, 
                 void *datasource);
        /// Positionsetzen in der Vorbisdatei.
        static int VorbisSeek(void *datasource, ogg_int64_t offset, int whence);
        /// Die Vorbisdatei schliessen.
        static int VorbisClose(void *datasource);
        /// Wo sind wir in der Vorbisdatei.
        static long VorbisTell(void *datasource);
                                        
         /// Verwaltungsstruktur fuer Vorbisdateien
        struct SOggFile
        {
            /// Zeiger auf die Ogg-Daten
            unsigned char* mDataPtr;
            /// Die Groesse der Datei
            int mDataSize;
            /// Positionszeiger in die Datei
            int mDataRead;
        } mOggMemoryFile;
        
        /// Diese Klasse kapselt das Fade in.
        class FadeThread : public Thread,
            public virtual EventSource, 
            public virtual EventCaster<SoundEvent> {
            private:
                /// Damit wir wissen, wo wir hinwollen ;-)
                SoundResource *that;
                /// Fade In oder Out
                bool mFadeIn;
                /// Dauer des Fades in msek.
                unsigned long int mDauer;
                /// Absichern von mDauer
                mutable Mutex mDauerMutex;
                /// Die Lautstärke, die Berechnungsgrundlage ist.
                ALfloat mGain;
                /// Absichern von mGain.
                mutable Mutex mGainMutex;
            protected:
                /// Berechne den Anstieg der Lautstarke beim Fade-In
                ALfloat calculateFadeIn(unsigned RL_LONGLONG time);

                /// Berechne die Abnahme der Lautstarke beim Fade-Out
                ALfloat calculateFadeOut(unsigned RL_LONGLONG time);

            public:
                /// Der Konstruktor.
                FadeThread(SoundResource *that, bool fadeIn);
                /// Die Arbeitsroutine.
                void run();
                /// Die Fadedauer setzen.
                void setDauer(const unsigned long int dauer);
                /// Die Fadedauer bekommen.
                const unsigned long int getDauer() const;
                /// Die Ausgangslautstärke setzen.
                void setGain(const ALfloat gain);
                /// Die Ausgangslautstärke bekommen.
                const ALfloat getGain() const;
                

        };
        /// Der Thread, der das Fade-In behandelt
        mutable FadeThread mFadeInThread;
        /// Der Thread, der das Fade-Out behandelt
     
        mutable FadeThread mFadeOutThread;
       
        /// Streamen der Sounddaten
        class StreamThread : public Thread,
            public virtual EventSource, 
            public virtual EventCaster<SoundEvent> {
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

        };
        /// Der Thread, der das Streamen behandelt.
        mutable StreamThread mStreamThread;
    
    protected:
        /// Die gekapselte Soundquelle
        ALuint mSource;
        /// Die Buffer, die wir benutzen
        ALuint *mBuffers;
        /// Wieviele Buffer werden benutzt.
        static const short mBufferCount = 4;
        /// Grösse des Buffers.
        static const int BUFFER_SIZE = (4096 * 2);
        /// Die Uebrgabe der Callbacks.
        static ov_callbacks mVorbisCallbacks;
        
        /// Welches Soundformat hat dieser Sound.
        ALenum mFormat;
        /// Wie gross ist der Sound in Bytes.
        ALsizei mSize;
        /// Wie gross ist ein Sample in Bits.
        ALsizei mBits;
        /// Mit welcher Frequenz wurde gesampelt.
        ALsizei mFrequency;
        /// Loopen?
        ALboolean mLoop;
        /// Die komplette Zeit des Stücks in Sekunden.
        double mTime;
        /// Unsere Daten von Ogres ResourceManager.
        DataChunk *mData;
        /// Die Art des Sounds.
        SoundDataType mSoundDataType;
        /// Für Waves dekodieren wir die Daten im voraus.
        ALvoid *mWAVData;
        /// Das OggVorbis-Filehandle
        OggVorbis_File mOggStream;
        

        /// Ueberpruefen, ob Fehler aufgetreten ist.
        void check() const throw (RuntimeException);
        
        /// Mutex zum Synchronisieren von Gain-Zugriffen.
        mutable Mutex mGainMutex;
        
        /// Fuehre das Fade-In aus
        void fadeIn(unsigned int msec);
        /// Fuehre das Fade-Out aus
        void fadeOut(unsigned int msec);
        
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
        /// Wir haben ein Ereignis erhalten.
        virtual bool eventRaised(SoundEvent *anEvent);
        
        

        void open(unsigned char *data, unsigned int size);
        void release();
        void display();
        bool playback();
        bool playing();
        bool update();
        bool stream(ALuint buffer);
        void empty();
        void check();
        Ogre::String errorString(int code);
        
};

}
#endif
