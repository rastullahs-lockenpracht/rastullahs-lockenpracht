/* SoundResource.cpp - Diese Klassse kapselt einen Sound.
 * (C) 2004. Team Pantheon. www.team-pantheon.de
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

#include "SoundResource.h"
#include "SoundSubsystem.h"

using namespace Ogre;

namespace rl {
    
    

/**
 * @author JoSch
 * @date 07-23-2004
 */
SoundResource::SoundResource(ResourceManager* creator, const String& name, ResourceHandle handle,
        const String& group, bool isManual, ManualResourceLoader* loader):
    Resource(creator, name, handle, group, isManual, loader)
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
    if (!mIsLoaded)
    {
        // Holen wir erstmal die Daten.
		mDataStream = ResourceGroupManager::getSingleton().openResource(mName, mGroup);
		size_t numBytes = mDataStream->size();

        if (StringUtil::endsWith(mName, ".ogg"))
        {
            mSoundDataType = OggVorbis;
        } 
		else 
		{
            mIsLoaded = false;
            return;
        }
    }
 
}

/** Entlaedt die Soundquelle und setzt die Werte.
 * @author JoSch
 * @date 07-23-2004
 */
void SoundResource::unloadImpl()
{
    if (!mDataStream.isNull())
    {
        mDataStream.setNull();
    }
    this->mIsLoaded = false;
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
SoundDataType SoundResource::getSoundDataType() const
{
    return mSoundDataType;
}

/**
 * @author JoSch
 * @date 03-14-2005
 */
Ogre::DataStreamPtr SoundResource::getDataStream() const
{
    return mDataStream;
}

/**
 * @author JoSch
 * @date 03-14-2005
 */
int SoundResource::getSize() const
{
    return calculateSize();
}


/**
 * @author Blakharaz
 * @date 03-14-2005
 */
SoundResourcePtr::SoundResourcePtr(const ResourcePtr& res) : SharedPtr<SoundResource>()
{
    // lock & copy other mutex pointer
    OGRE_LOCK_MUTEX(*res.OGRE_AUTO_MUTEX_NAME)
        OGRE_COPY_AUTO_SHARED_MUTEX(res.OGRE_AUTO_MUTEX_NAME)
        pRep = static_cast<SoundResource*>(res.getPointer());
    pUseCount = res.useCountPointer();
    if (pUseCount != 0)
        ++(*pUseCount);
}

/**
 * @author Blakharaz
 * @date 03-14-2005
 */
SoundResourcePtr& SoundResourcePtr::operator =(const ResourcePtr& res)
{
    if (pRep == static_cast<SoundResource*>(res.getPointer()))
        return *this;
    release();

    // lock & copy other mutex pointer
    OGRE_LOCK_MUTEX(*res.OGRE_AUTO_MUTEX_NAME)
        OGRE_COPY_AUTO_SHARED_MUTEX(res.OGRE_AUTO_MUTEX_NAME)
        pRep = static_cast<SoundResource*>(res.getPointer());
    pUseCount = res.useCountPointer();
    if (pUseCount != 0)
        ++(*pUseCount);

    return *this;
}

/**
 * @author Blakharaz
 * @date 03-14-2005
 */
void SoundResourcePtr::destroy()
{
    SharedPtr<SoundResource>::destroy();
}



} /* End of Namespace */

