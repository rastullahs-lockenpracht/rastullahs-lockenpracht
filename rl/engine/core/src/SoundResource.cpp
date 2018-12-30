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

#include "SoundResource.h"

using namespace Ogre;

namespace rl
{

    /**
     * @author JoSch
     * @date 07-23-2004
     */
    SoundResource::SoundResource(ResourceManager* creator, const String& name, ResourceHandle handle,
        const String& group, bool isManual, ManualResourceLoader* loader)
        : Resource(creator, name, handle, group, isManual, loader)
        , mDataStream()
    {
    }

    /**
     * @author JoSch
     * @date 07-24-2004
     */
    SoundResource::~SoundResource()
    {
    }

    /** Laedt die Soundquelle und setzt die Werte.
     * @author JoSch
     * @date 10-11-2004
     */
    void SoundResource::loadImpl()
    {
        if (!isLoaded())
        {
            mDataStream = ResourceGroupManager::getSingleton().openResource(mName, mGroup);
        }
    }

    /** Entlaedt die Soundquelle und setzt die Werte.
     * @author JoSch
     * @date 07-23-2004
     */
    void SoundResource::unloadImpl()
    {
        mDataStream.reset();
    }

    /**
     * @author JoSch
     * @date 03-14-2005
     */
    size_t SoundResource::calculateSize() const
    {
        return mDataStream->size();
    }

    /**
     * @author JoSch
     * @date 03-14-2005
     */
    const Ogre::DataStreamPtr& SoundResource::getDataStream() const
    {
        return mDataStream;
    }

} /* End of Namespace */
