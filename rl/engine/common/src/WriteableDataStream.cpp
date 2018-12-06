/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#include "stdinc.h"

#include "WriteableDataStream.h"

using namespace Ogre;

namespace rl
{
    WriteableFileStreamDataStream::WriteableFileStreamDataStream(std::fstream* s, bool freeOnClose)
    {
        mpStream->seekg(0, std::ios_base::end);
        mSize = mpStream->tellg();
        mpStream->seekg(0, std::ios_base::beg);
    }

    WriteableFileStreamDataStream::WriteableFileStreamDataStream(
        const Ogre::String& name, std::fstream* s, bool freeOnClose)
    {
        mpStream->seekg(0, std::ios_base::end);
        mSize = mpStream->tellg();
        mpStream->seekg(0, std::ios_base::beg);
    }

    WriteableFileStreamDataStream::WriteableFileStreamDataStream(
        const Ogre::String& name, std::fstream* s, size_t size, bool freeOnClose)
    {
        mSize = size;
    }

    WriteableFileStreamDataStream::~WriteableFileStreamDataStream()
    {
        close();
    }

    size_t WriteableFileStreamDataStream::read(void* buf, size_t count)
    {
        mpStream->read(static_cast<char*>(buf), static_cast<std::streamsize>(count));
        return mpStream->gcount();
    }

    size_t WriteableFileStreamDataStream::readLine(char* buf, size_t maxCount, const Ogre::String& delim)
    {
        if (delim.empty())
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "No delimiter provided", "FileStreamDataStream::readLine");
        }
        if (delim.size() > 1)
        {
            LogManager::getSingleton().logMessage(
                "WARNING: FileStreamDataStream::readLine - using only first delimeter");
        }
        // Deal with both Unix & Windows LFs
        bool trimCR = false;
        if (delim.at(0) == '\n')
        {
            trimCR = true;
        }
        // maxCount + 1 since count excludes terminator in getline
        mpStream->getline(buf, static_cast<std::streamsize>(maxCount + 1), delim.at(0));
        size_t ret = mpStream->gcount();
        // three options
        // 1) we had an eof before we read a whole line
        // 2) we ran out of buffer space
        // 3) we read a whole line - in this case the delim character is taken from the stream but not written in the
        // buffer so the read data is of length ret-1 and thus ends at index ret-2 in all cases the buffer will be null
        // terminated for us

        if (mpStream->eof())
        {
            // no problem
        }
        else if (mpStream->fail())
        {
            // Did we fail because of maxCount hit? No - no terminating character
            // in included in the count in this case
            if (ret == maxCount)
            {
                // clear failbit for next time
                mpStream->clear();
            }
            else
            {
                OGRE_EXCEPT(
                    Exception::ERR_INTERNAL_ERROR, "Streaming error occurred", "FileStreamDataStream::readLine");
            }
        }
        else
        {
            // we need to adjust ret because we want to use it as a
            // pointer to the terminating null character and it is
            // currently the length of the data read from the stream
            // i.e. 1 more than the length of the data in the buffer and
            // hence 1 more than the _index_ of the NULL character
            --ret;
        }

        // trim off CR if we found CR/LF
        if (trimCR && buf[ret - 1] == '\r')
        {
            --ret;
            buf[ret] = '\0';
        }
        return ret;
    }

    void WriteableFileStreamDataStream::skip(long count)
    {
#if defined(STLPORT)
        // Workaround for STLport issues: After reached eof of file stream,
        // it's seems the stream was putted in intermediate state, and will be
        // fail if try to repositioning relative to current position.
        // Note: tellg() fail in this case too.
        if (mpStream->eof())
        {
            mpStream->clear();
            // Use seek relative to either begin or end to bring the stream
            // back to normal state.
            mpStream->seekg(0, std::ios::end);
        }
#endif
        mpStream->clear(); // Clear fail status in case eof was set
        mpStream->seekg(static_cast<std::ifstream::pos_type>(count), std::ios::cur);
    }

    void WriteableFileStreamDataStream::seek(size_t pos)
    {
        mpStream->clear(); // Clear fail status in case eof was set
        mpStream->seekg(static_cast<std::streamoff>(pos), std::ios::beg);
    }

    size_t WriteableFileStreamDataStream::tell(void) const
    {
        mpStream->clear(); // Clear fail status in case eof was set
        return mpStream->tellg();
    }

    bool WriteableFileStreamDataStream::eof(void) const
    {
        return mpStream->eof();
    }

    void WriteableFileStreamDataStream::close(void)
    {
        if (mpStream)
        {
            mpStream->close();
            if (mFreeOnClose)
            {
                // delete the stream too
                delete mpStream;
                mpStream = 0;
            }
        }
    }

    std::fstream* WriteableFileStreamDataStream::getFStream()
    {
        return mpStream;
    }

    void WriteableFileStreamDataStream::write(void* buf, size_t count)
    {
        mpStream->clear();
        mpStream->write(static_cast<char*>(buf), static_cast<std::streamsize>(count));
    }

    void WriteableFileStreamDataStream::flush()
    {
        mpStream->clear();
        mpStream->flush();
    }
}
