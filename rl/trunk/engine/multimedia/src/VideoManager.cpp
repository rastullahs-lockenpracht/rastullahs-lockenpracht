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
#include "VideoManager.h"
#include "Video.h"

using namespace Ogre;

template<> rl::VideoManager* Singleton<rl::VideoManager>::ms_Singleton = 0;

namespace rl {

/**
 * @return Das Singleton-Objekt
 * @author JoSch
 * @date 05-26-2004
 */
VideoManager& VideoManager::getSingleton(void)
{
	return Singleton<VideoManager>::getSingleton();
}

/**
 * @return Das Singleton-Zeiger
 * @author JoSch
 * @date 05-26-2004
 */
VideoManager* VideoManager::getSingletonPtr(void)
{
    return Singleton<VideoManager>::getSingletonPtr();
}

/**
 * @author JoSch
 * @date 05-26-2004
 */
VideoManager::VideoManager():
    mVideoList()
{   
}

/**
 * @author JoSch
 * @date 05-26-2004
 */


/**
 * @param video Das Video, das hinzugefügt werden soll.
 * @author JoSch
 * @date 09-06-2005
 */
void VideoManager::addVideo(Video *video)
{
    mVideoList.push_back(video);
}

/**
 * @return Alle Videos in der Liste löschen
 * @author JoSch
 * @date 09-06-2005
 */
void VideoManager::clearVideos()
{
    mVideoList.clear();
}

/**
 * @param Das Video, das gelöscht werden soll
 * @author JoSch
 * @date 09-06-2005
 */
void VideoManager::removeVideo(Video *video)
{
    for(VideoList::iterator i = mVideoList.begin();
        i != mVideoList.end();)
    {
        if (*i == video)
        {
            i = mVideoList.erase(i);
        } else {
            i ++;
        }
    }
}



}
