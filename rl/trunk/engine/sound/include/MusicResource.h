/* Copyright 2004 Jochen Schaefer */
#ifndef MUSICRESOURCE_H
#define MUSICRESOURCE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <OpenThreads/Thread>
#include "SoundPrerequisites.h"
#include "OgreString.h"
#include "SndResource.h"

using namespace OpenThreads;

namespace rl {

/** Diese Klasse kapselt eine OpenAl-Source (MUSIK) fuer
 * den ResourceManager von Ogre
 * @author Josch
 * @date 06-17-2004
 * @date 07-12-2004
 * @version 2.0
 */
class _RlSoundExport MusicResource: public SndResource {
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
            char* mDataPtr;
            /// Die Groesse der Datei
            int mDataSize;
            /// Positionszeiger in die Datei
            int mDataRead;
        }; 
        SOggFile mOggMemoryFile;
        
        /// Diese Klasse kapselt den Arbeitsthread.
        class MusicThread : public Thread {
            private:
                /// Damit wir wissen, wo wir hinwollen ;-)
                MusicResource *that;
            public:
                /// Der Konstruktor.
                MusicThread(MusicResource *that);
                /// Die Arbeitsroutine.
                void run();
                /// Wie lange der Fade-In/Out dauern soll
                unsigned int mFadeIn, mFadeOut;
        } mMusicThread;
        friend class MusicThread;
        
        /// Grösse des Buffers.
        static const int BUFFER_SIZE = (4096 * 2);
        /// Die Uebrgabe der Callbacks.
        static ov_callbacks mVorbisCallbacks;
        /// Der Kommentar zur Ogg-Datei.
        vorbis_comment *mVorbisComment;
        /// Informationen ueber die Oggdatei.
        vorbis_info *mVorbisInfo;
        /// Formatinfo der Oggdatei.
        ALenum mFormat;
        /// Das Vorbis-FILE
        OggVorbis_File mOggStream;
                
        /// Ob wir spielen, oder nicht
        bool mPlaying;
        /// Die Daten
        unsigned char *mData;
        /// Die Grösse der Daten.
        unsigned int mDataSize;
        
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
        
        
    public:
        /// Der Standardkonstruktor
        MusicResource(const Ogre::String &name);
        /// Destruktor
        ~MusicResource();
        
        /// Spielt den Sound ab.
        virtual void play(unsigned int msec = 0) throw (RuntimeException);
        /// Stoppt den Sound.
        virtual void stop(unsigned int msec = 0) throw (RuntimeException);
        /// Den Zustand des Sounds zurueckgeben.
        const bool isPlaying() const;
        /// Laedt die Ogg-Datei.
        virtual void load(); 
};

}
#endif
