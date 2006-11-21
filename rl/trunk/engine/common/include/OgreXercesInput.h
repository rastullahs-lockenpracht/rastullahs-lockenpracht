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

#ifndef __OgreXercesInput_H__
#define __OgreXercesInput_H__

#pragma warning (push)
#pragma warning (disable : 4244)
#include <xercesc/sax/InputSource.hpp>
#include <xercesc/util/BinInputStream.hpp>
#pragma warning (pop)

#include "CommonPrerequisites.h"

#include <OgreDataStream.h>

namespace rl {

    class _RlCommonExport OgreBinInputStream : public XERCES_CPP_NAMESPACE::BinInputStream
    {
    public:
        OgreBinInputStream(const Ogre::DataStreamPtr& stream);

        virtual unsigned int curPos() const;
        virtual unsigned int readBytes(XMLByte* const toFill, const unsigned int maxToRead);

    private:
        Ogre::DataStreamPtr mStream;
    };

    class _RlCommonExport OgreInputSource : public XERCES_CPP_NAMESPACE::InputSource
    {
    public:
        OgreInputSource(const Ogre::DataStreamPtr& stream);

        virtual XERCES_CPP_NAMESPACE::BinInputStream* makeStream() const;

    private:
        Ogre::DataStreamPtr mStream;
    };
}

#endif //__OgreXercesInput_H__
