/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __SoundMovable_H__
#define __SoundMovable_H__

#include "SoundPrerequisites.h"
#include "SoundResource.h"
#include <vector>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "EventSource.h"
#include "EventListener.h"
#include "EventCaster.h"
#include "SoundEvents.h"

namespace Ogre {
    class MovableObject;
    class Vector3;
}

namespace rl {
    typedef vector<ALuint> ALuintVector;

    class _RlSoundExport SoundMovable : public Ogre::MovableObject,
         public virtual EventListener<SoundEvent>,
         public virtual EventSource, 
         public virtual EventCaster<SoundEvent>
    {
    private:
        /// Der Name des SoundMovable
        Ogre::String mName;
        /// Der Soundresource, auf den wir verweisen.
        SoundResourcePtr mSoundResource;

        // Damit die Timings alle 0.5 sek. abgeschickt werden.
        int mTicks;
    
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
            /// Zeiger auf die Ogg-Daten durch den Datenstrom
            Ogre::DataStreamPtr mDataStream;
            /// Die Groesse der Datei
            int mDataSize;
            /// Positionszeiger in die Datei
            int mDataRead;
        } mOggMemoryFile;
        
        // Behandelt FadeIn
        class FadeIn
        {
            private:
                /// Welche SoundResource bedienen wir?
                SoundMovable *that;
            public:
                /// Standardkonstruktor
                FadeIn();
                /// Setze die gewuenschte SoundResource
                void setThat(SoundMovable *that);
                /// Die Funktormethode
                void operator()();
        } mFadeInFunctor;

        // Behandelt FadeOut
        class FadeOut
        {
            private:
                /// Welche SoundResource bedienen wir?
                SoundMovable *that;
            public:
                /// Standardkonstruktor
                FadeOut();
                 /// Setze die gewuenschte SoundResource
                void setThat(SoundMovable *that);
                /// Die Funktormethode
                void operator()();
        } mFadeOutFunctor;
        
        // Behandelt Streamen
        class Streaming
        {
            private:
                /// Welche SoundResource bedienen wir?
                SoundMovable *that;
            public:
                /// Standardkonstruktor
                Streaming();
                /// Setze die gewuenschte SoundResource
                void setThat(SoundMovable *that);
                /// Die Funktormethode
                void operator()();
        } mStreamFunctor;
        

        /// Berechne den Anstieg der Lautstarke beim Fade-In
        ALfloat calculateFadeIn(unsigned RL_LONGLONG time, ALfloat gain);

        /// Berechne die Abnahme der Lautstarke beim Fade-Out
        ALfloat calculateFadeOut(unsigned RL_LONGLONG time, ALfloat gain);

        /// Der Thread, der das Fade-In behandelt
        mutable boost::thread *mFadeInThread;
        /// Der Thread, der das Fade-Out behandelt
        mutable boost::thread *mFadeOutThread;
        /// Der Thread, der das Streamen behandelt.
        mutable boost::thread *mStreamThread;
       
        /// Die Lautstärke, die Berechnungsgrundlage ist.
        ALfloat mGain;
        
        /// Zeit in ms, die wir unterbechen.
        static int mSleepTime;
                
        /// Die gekapselte Soundquelle
        ALuint mSource;
        /// Die Buffer, die wir benutzen
        ALuintVector mBuffers;
        /// Wieviele Buffer werden benutzt.
        static const short mDefaultBufferCount = 8;
        /// Grösse des Buffers.
        static const int BUFFER_SIZE = (2048 * 2);
        /// Die Uebrgabe der Callbacks.
        static ov_callbacks mVorbisCallbacks;
        
        /// Dauer des FadeIns
        unsigned int mFadeIn;
        /// Dauer des FadeOuts
        unsigned int mFadeOut;
  
        /// Welches Soundformat hat dieser Sound.
        ALenum mFormat;
        /// Wie gross ist der Sound in Bytes.
        ALsizei mSize;
        /// Mit welcher Frequenz wurde gesampelt.
        ALsizei mFrequency;
        /// Loopen?
        ALboolean mLoop;
        /// Die komplette Zeit des Stücks in Sekunden.
        double mTime;
        /// Das OggVorbis-Filehandle
        OggVorbis_File mOggStream;
      

        /// Ueberpruefen, ob Fehler aufgetreten ist.
        void check() const throw (RuntimeException);
        
        /// Mutex zum Synchronisieren von Gain-Zugriffen.
        mutable boost::mutex mGainMutex;
        
        /// Fuehre das Fade-In aus
        void fadeIn(unsigned int msec);
        /// Fuehre das Fade-Out aus
        void fadeOut(unsigned int msec);
        /// Wir warten, bis alle Buffer durch sind.
        void waitForEnd();

    public:
        /// Konstruktor
        SoundMovable(const Ogre::String& name);
        /// Konstruktor
        SoundMovable(const SoundResourcePtr &soundres);
        /// Destruktor
        virtual ~SoundMovable();
        /// Name zurückgeben
        virtual const Ogre::String& getName() const;
        /// Moveable-Typ
        virtual const Ogre::String& getMovableType() const;
        /// Kamera informieren
        virtual void _notifyCurrentCamera(Ogre::Camera *cam);
        /// Unsere Bounding-Box
        virtual const Ogre::AxisAlignedBox& getBoundingBox(void) const;
        /// Bound-Radius
        virtual Ogre::Real getBoundingRadius() const;
        /// Rendern
        virtual void _updateRenderQueue(Ogre::RenderQueue *queue);
    
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
        /// Wir haben ein Ereignis erhalten.
        virtual bool eventRaised(SoundEvent *anEvent) const;
        
        /// Der Funktor für den fadeIn-Thread.
        void fadeIn();
        /// Der Funktor für den fadeOut-Thread.
        void fadeOut();
        /// Der Funktor für den Streaming-Thread.
        void runStreaming();
        
        /// Liefert die Dauer des FadeIn zurueck.
        unsigned int getFadeIn() const;
        /// Setzt die Dauer des FadeIn zurueck.
        void setFadeIn(unsigned int dauer);
        /// Liefert die Dauer des FadeIn zurueck.
        unsigned int getFadeOut() const;
        /// Setzt die Dauer des FadeIn zurueck.
        void setFadeOut(unsigned int dauer);
        /// Ist laeuft AL noch
        const bool playing() const;

protected:
        /// Shared class-level name for Movable type
        static Ogre::String msMovableType;
        static Ogre::AxisAlignedBox msAABox;

private:
        // Grabbed from example
        void display();
        bool playback();
        bool update();
        bool oggstream(ALuint buffer);
        bool stream(ALuint buffer);
        void empty();
        void check();
        Ogre::String errorString(int code);
    }; 

    class _RlSoundExport SoundMovablePtr :
        public Ogre::SharedPtr<SoundMovable>
    {
    public:
        SoundMovablePtr() : Ogre::SharedPtr<SoundMovable>() {}
        explicit SoundMovablePtr(SoundMovable* rep) : Ogre::SharedPtr<SoundMovable>(rep) {}
        SoundMovablePtr(const SoundMovablePtr& res) : Ogre::SharedPtr<SoundMovable>(res) {}
    protected:
        void destroy();
    };

}
#endif
