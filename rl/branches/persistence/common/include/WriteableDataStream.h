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

#ifndef _H_WriteableDataStream
#define _H_WriteableDataStream

#include "CommonPrerequisites.h"

#include <iostream>

namespace rl
{
    class _RlCommonExport WriteableDataStream : public Ogre::DataStream
    {
    public:
		WriteableDataStream() : Ogre::DataStream() {}
		WriteableDataStream(const Ogre::String &name) : Ogre::DataStream(name) {}
        virtual void write(void* buf, size_t count) {}
        virtual void flush() {}
    };

    typedef Ogre::SharedPtr<WriteableDataStream> WriteableDataStreamPtr;

    class _RlCommonExport WriteableFileStreamDataStream : public WriteableDataStream
    {
    protected:
		std::fstream* mpStream;
        bool mFreeOnClose;	
    public:
        WriteableFileStreamDataStream(std::fstream* s, 
            bool freeOnClose = true);

        WriteableFileStreamDataStream(const Ogre::String& name, 
            std::fstream* s, 
            bool freeOnClose = true);

        WriteableFileStreamDataStream(const Ogre::String& name, 
            std::fstream* s, 
            size_t size, 
            bool freeOnClose = true);

        ~WriteableFileStreamDataStream();

		virtual size_t read(void* buf, size_t count);

        virtual size_t readLine(char* buf, size_t maxCount, const Ogre::String& delim = "\n");
		
		virtual void skip(long count);
	
	    virtual void seek( size_t pos );

		virtual size_t tell(void) const;

	    virtual bool eof(void) const;

        virtual void close(void);

        virtual std::fstream* getFStream();

        virtual void write(void* buf, size_t count);

        virtual void flush();
    };

    typedef Ogre::SharedPtr<WriteableFileStreamDataStream> WriteableFileStreamDataStreamPtr;
}

#endif

