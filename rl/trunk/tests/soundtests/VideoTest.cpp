#include <xercesc/util/XMemory.hpp>	// Muss vor Ogre stehen (zumindest für VS)


#include "SoundPrerequisites.h"

#include <Ogre.h>

#include <OgreRenderWindow.h>
#include <OgreCEGUIRenderer.h>

#include <OgreNoMemoryMacros.h>
#include <CEGUI.h>
#include <OgreMemoryMacros.h>

#include <CoreSubsystem.h>
#include <OgreSceneManager.h>
#include <OgreCEGUIResourceProvider.h>
#include <OgreFrameListener.h>
#include <OgreLight.h>
#include <OgrePlatform.h>
#include "World.h"
#include "Sleep.h"
#include "DebugWindow.h"
#include "ExampleApplication.h"
#include "Video.h"


using namespace rl;
using namespace CEGUI;
using namespace Ogre;

class VideoTest : public ExampleApplication, public FrameListener {
private:
    Video *mVideo;
    
public:
    VideoTest()
    {
    }
    
	void setUp()
	{
        mVideo = 0;
        
        Ogre::RenderWindow* window = Ogre::Root::getSingleton().getAutoCreatedWindow();
        OgreCEGUIRenderer* rend = 
            new OgreCEGUIRenderer(window, 
                                Ogre::RENDER_QUEUE_OVERLAY, 
                                false, 
                                3000,
                                mSceneMgr);

        new System(rend, NULL, new OgreCEGUIResourceProvider(), (utf8*)"cegui.config"); 
        // load scheme and set up defaults
        ///@todo Hier sollte was Lookunabh�ngiges rein!!! FIXME TODO BUG!
        System::getSingleton().setDefaultMouseCursor((utf8*)"RastullahLook", (utf8*)"MouseArrow");
        Window* sheet = CEGUI::WindowManager::getSingleton().createWindow((utf8*)"DefaultWindow", (utf8*)"root");
        sheet->setSize(
            Absolute, 
            CEGUI::Size(Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth(), 
                Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight()));
        sheet->setPosition(Absolute, CEGUI::Point(0, 0));
        System::getSingleton().setGUISheet(sheet);
        TextureManager::getSingleton().setDefaultNumMipmaps(5);
        FrameWindow* frame = (FrameWindow*)WindowManager::getSingleton().createWindow("RastullahLook/FrameWindow", "testWindow");
        sheet->addChildWindow(frame);
        frame->setPosition(Point(0.10f, 0.10f));
        frame->setSize(Size(0.80f, 0.80f));
        frame->setAlwaysOnTop(true);
        frame->setText("Video");
        mVideo = new Video("testWindow/Play", "intro.ogv");
        mVideo->play();
        StaticImage *image = (StaticImage*)WindowManager::getSingleton().createWindow("RastullahLook/StaticImage", "testWindow/image");
        frame->addChildWindow(image);
        
        image->setMaximumSize(Size(2.0f, 2.0f));
        image->setPosition(Point(0.0f, 0.05f));
        image->setSize(Absolute, Size(mVideo->getWidth(), mVideo->getHeight()));
        image->setFrameEnabled(false);
        image->setBackgroundEnabled(false);
    
        //Now attach Texture to
        if(mVideo->getTexture())
        {
            CEGUI::String temp = "MyImagesNumber";
            CEGUI::String tempName = mVideo->getTextureName();
    
            Imageset *img = ImagesetManager::getSingleton().createImageset( 
                    temp, mVideo->getTexture() );
    
            unsigned int width = mVideo->getWidth();
            unsigned int height= mVideo->getHeight();
    
            img->defineImage( tempName, Point(0.0f,0.0f), Size( width, height ), Point(0.0f,0.0f));
            image = (StaticImage*)WindowManager::getSingleton().getWindow("testWindow/image");
            image->setImage( temp, tempName);
        }
  
	}
    
    void createScene()
    {
//        Entity *e = mSceneMgr->createEntity("held", "held.mesh");
//        SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
//        node->attachObject(e);
//        node->setPosition(0,0,0);

        ColourValue c = ColourValue(0.5, 0.5, 0.5);
        mSceneMgr->setAmbientLight(c);
        setUp();
        mRoot->addFrameListener(this);

    }

	void showVideo()
	{
        mVideo->play();
	}
    
    virtual bool frameStarted(const FrameEvent &evt)
    {
        if (mVideo)
        {
            mVideo->update();
        }

        return true;
    }
};

#if OGRE_PLATFORM == OGRE_WIN32
INT WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,INT) 
#else
int main(int, char**) 
#endif
{
   VideoTest myApp;     // Instanciate our subclass
   try {           // This try-catch provides a nice way of popping up errors if they occur.
        myApp.go(); // ExampleApplication provides a go method, which starts the rendering.
   } catch (Ogre::Exception& e) {
        std::cerr <<"Exception:\n";
        std::cerr <<e.getFullDescription().c_str() <<"\n";
        return 1; // Return nonzero on failure    
   }
}
