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
#include "VorbisFile.h"
#include "Exception.h"

/// Die Callback-Struktur, die Vorbisfile braucht
ov_callbacks rl::VorbisFile::gVorbisCallbacks = {
    &rl::VorbisFile::VorbisRead,
    &rl::VorbisFile::VorbisSeek,
    &rl::VorbisFile::VorbisClose,
    &rl::VorbisFile::VorbisTell
};

namespace rl {
    
VorbisFile::VorbisFile(const SoundResourcePtr &res):
    mSoundResource(res)
{
    mOggMemoryFile.mDataStream = mSoundResource->getDataStream();
    mOggMemoryFile.mDataSize = mSoundResource->getSize();
    mOggMemoryFile.mDataRead = 0; 
    if (ov_open_callbacks(&mOggMemoryFile, &mOggStream, NULL, 0,
        VorbisFile::gVorbisCallbacks) != 0)
    {
        Throw(RuntimeException, "Could not read Ogg file from memory");
    }
    mVorbisInfo = *ov_info(&mOggStream, -1);
    double time = ov_time_total(&mOggStream, -1);
    mTotalTime.sec = (long long int)time;
    mTotalTime.nsec = (int)(time - mTotalTime.sec) * pow(10.0, 9.0);
}

VorbisFile::~VorbisFile()
{
    ov_clear(&mOggStream);
}

const boost::xtime &VorbisFile::getTotalTime() const
{
    return mTotalTime;
}

/**
 ******************************************************************************
 * Der folgende Code wurde dem Beispielcode von Jesse Maurais und Spree Tree
 * auf DevMaster.net entliehen und den hiesigen Bedürfnissen angepasst.
 * Der Originalcode kann von hier geladen werden:
 *  http://www.devmaster.net/articles.php?catID=6
 ******************************************************************************/
size_t VorbisFile::VorbisRead(void *ptr, size_t byteSize, size_t sizeToRead, void *datasource)
{
    size_t spaceToEOF;      // How much more we can read till we hit the EOF marker
    size_t actualSizeToRead;        // How much data we are actually going to read from memory
    SOggFile *vorbisData;   // Our vorbis data, for the typecast

    // Get the data in the right format
    vorbisData = (SOggFile *) datasource;

    // Calculate how much we need to read.  This can be sizeToRead*byteSize or less depending on how near the EOF marker we are
    spaceToEOF = vorbisData->mDataSize - vorbisData->mDataRead;
    if ((sizeToRead * byteSize) < spaceToEOF)
        actualSizeToRead = (sizeToRead * byteSize);
    else
        actualSizeToRead = spaceToEOF;

    // A simple copy of the data from memory to the datastruct that the vorbis libs will use
    if (actualSizeToRead) {
        vorbisData->mDataRead += vorbisData->mDataStream->read(ptr, actualSizeToRead);
        // Copy the data from the start of the file PLUS how much we have already read in
        // TODO: memcpy (ptr, (char *) vorbisData->mDataPtr + vorbisData->mDataRead,
        //        actualSizeToRead);
        // Increase by how much we have read by
        //vorbisData->mDataRead += (actualSizeToRead);
    }

    // Return how much we read (in the same way fread would)
    return actualSizeToRead;
}

int VorbisFile::VorbisSeek(void *datasource, ogg_int64_t offset, int whence)
{
    size_t spaceToEOF;      // How much more we can read till we hit the EOF marker
    ogg_int64_t actualOffset;       // How much we can actually offset it by
    SOggFile *vorbisData;   // The data we passed in (for the typecast)

    // Get the data in the right format
    vorbisData = (SOggFile *) datasource;

    // Goto where we wish to seek to
    switch (whence) {
        case SEEK_SET:     // Seek to the start of the data file
            // Make sure we are not going to the end of the file
            if (vorbisData->mDataSize >= offset)
                actualOffset = offset;
            else
                actualOffset = vorbisData->mDataSize;
            // Set where we now are
            vorbisData->mDataRead = (int) actualOffset;
            break;
        case SEEK_CUR:     // Seek from where we are
            // Make sure we dont go past the end
            spaceToEOF = vorbisData->mDataSize - vorbisData->mDataRead;
            if (offset < spaceToEOF)
                actualOffset = (offset);
            else
                actualOffset = spaceToEOF;
            // Seek from our currrent location
            vorbisData->mDataRead += actualOffset;
            break;
        case SEEK_END:     // Seek from the end of the file
            vorbisData->mDataRead = vorbisData->mDataSize + 1;
            break;
        default:
            printf ("*** ERROR *** Unknown seek command in VorbisSeek\n");
            break;
    };
    vorbisData->mDataStream->seek(vorbisData->mDataRead);

    return 0;
}

int VorbisFile::VorbisClose(void *datasource)
{
   SOggFile *vorbisData;

    // Get the data in the right format
    vorbisData = (SOggFile *) datasource;

    // This file is called when we call ov_close.  If we wanted, we could free our memory here, but
    // in this case, we will free the memory in the main body of the program, so dont do anything
    vorbisData->mDataStream->seek(0);
    return 1;
}

long VorbisFile::VorbisTell(void *datasource)
{
    SOggFile *vorbisData;

    // Get the data in the right format
    vorbisData = (SOggFile *) datasource;

    // We just want to tell the vorbis libs how much we have read so far
    return vorbisData->mDataStream->tell();
}

}
