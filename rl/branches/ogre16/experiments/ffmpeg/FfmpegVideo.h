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
#ifndef __FfmpegVideo_H__
#define __FfmpegVideo_H__

#include "MultiMediaPrerequisites.h"

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>


#include <CEGUITexture.h>
#include <OgreTexture.h>

namespace rl {

	class _RlMultimediaExport FfmpegVideo {

	public:
		FfmpegVideo(const Ogre::String& filename);

		void play();
	private:
		bool getNextFrame(AVFormatContext *pFormatCtx, AVCodecContext *pCodecCtx, 
		int videoStream, AVFrame *pFrame);

		Ogre::Texture* mTexture;
		CEGUI::Texture* mCeGuiTexture;
		

	};
}

#endif // __FfmpegVideo_H__
