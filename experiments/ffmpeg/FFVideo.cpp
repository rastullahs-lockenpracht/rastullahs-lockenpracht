#include "FFVideo.h"

#include <OgreNoMemoryMacros.h>
#include <CEGUISystem.h>
#include <OgreMemoryMacros.h>
#include <OgreExternalTextureSourceManager.h>
#include <OgreCEGUIRenderer.h>

FFVideo::FFVideo(Ogre::String& )
{			
	OgreCEGUIRenderer* guirenderer = 
			static_cast<OgreCEGUIRenderer*>(System::getSingleton().getRenderer());
		TexturePtr tex = TextureManager::getSingleton().createManual(
				"Video", 
				ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
				Ogre::TEX_TYPE_2D, 
				640, 480, 
				0, 0, 
				Ogre::PF_BYTE_RGB, 
				Ogre::TU_DYNAMIC_WRITE_ONLY);
		mTexture = tex.getPointer();
		mCeGuiTexture = guirenderer->createTexture(tex);

}

FFVideo::~FFVideo(void)
{
}
