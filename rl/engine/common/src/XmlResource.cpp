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

#include "stdinc.h" //precompiled header

#include "XmlProcessor.h"
#include "XmlResource.h"
#include "XmlResourceManager.h"

using namespace Ogre;

namespace rl
{

    XmlResource::XmlResource(ResourceManager* creator, const Ogre::String& name, ResourceHandle handle,
        const Ogre::String& group, bool isManual, ManualResourceLoader* loader)
        : Resource(creator, name, handle, group, isManual, loader)
        , mCharBuffer(NULL)
    {
    }

    XmlResource::~XmlResource()
    {
        unload();
    }

    void XmlResource::loadImpl()
    {
        DataStreamPtr ds = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup);
        mSize = ds->size();

        mCharBuffer = new char[mSize];
        ds->read(mCharBuffer, mSize);
    }

    void XmlResource::unloadImpl()
    {
        delete[] mCharBuffer;
        mCharBuffer = 0;
    }

    size_t XmlResource::calculateSize() const
    {
        return mSize;
    }

    const char* XmlResource::getContent()
    {
        if (!mCharBuffer)
        {
            load();
        }

        return mCharBuffer;
    }
}
