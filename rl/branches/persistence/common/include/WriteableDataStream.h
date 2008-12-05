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
        virtual void write(void* buf, size_t count) = 0;
        virtual void flush() = 0;
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

		size_t read(void* buf, size_t count);

        size_t readLine(char* buf, size_t maxCount, const Ogre::String& delim = "\n");
		
		void skip(long count);
	
	    void seek( size_t pos );

		size_t tell(void) const;

	    bool eof(void) const;

        void close(void);

        std::fstream* getFStream();

        void write(void* buf, size_t count);

        void flush();
    };

    typedef Ogre::SharedPtr<WriteableFileStreamDataStream> WriteableFileStreamDataStreamPtr;
}

#endif

