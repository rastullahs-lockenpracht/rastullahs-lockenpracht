/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Clarified Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Clarified Artistic License for more details.
*
*  You should have received a copy of the Clarified Artistic License
*  along with this program; if not you can get it here
*  http://www.jpaulmorrison.com/fbp/artistic2.htm.
*/
#ifndef VORBISFILE_H_
#define VORBISFILE_H_

#include "MultimediaPrerequisites.h"
#include <boost/thread/xtime.hpp>
extern "C" {
#include <vorbis/vorbisfile.h>
}
#include "SoundResource.h"

namespace rl {
 
   
class _RlMultimediaExport VorbisFile
{
private:
    static size_t VorbisRead(void *ptr, size_t size, size_t nmemb, void *datasource);
    static int VorbisSeek(void *datasource, ogg_int64_t offset, int whence);
    static int VorbisClose(void *datasource);
    static long VorbisTell(void *datasource);
    /// Die Übergabe der Callbacks.
    static ov_callbacks gVorbisCallbacks;
    boost::xtime mTotalTime;
    SoundResourcePtr mSoundResource;
    OggVorbis_File mOggStream; 
    vorbis_info mVorbisInfo; 
    
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

public:
	VorbisFile(const SoundResourcePtr &res);
	virtual ~VorbisFile();
    const boost::xtime &getTotalTime() const;
};

}
#endif /*VORBISFILE_H_*/
