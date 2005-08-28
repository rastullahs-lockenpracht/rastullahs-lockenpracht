
#include "SoundPrerequisites.h"
#include "Video.h"
#include <CEGUI/CEGUI.h>
#include <OgreRenderWindow.h>
#include <OGRE/OgreCEGUIRenderer.h>
#include <CoreSubsystem.h>
#include <OgreSceneManager.h>
#include <OgreCEGUIResourceProvider.h>
#include <OgreFrameListener.h>
#include <OgreLight.h>
#include <Ogre.h>
#include "World.h"
#include "Sleep.h"
#include "DebugWindow.h"
#include "ExampleApplication.h"

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
        ///@todo Hier sollte was Lookunabhängiges rein!!! FIXME TODO BUG!
        System::getSingleton().setDefaultMouseCursor((utf8*)"RastullahLook", (utf8*)"MouseArrow");
        Window* sheet = CEGUI::WindowManager::getSingleton().createWindow((utf8*)"DefaultGUISheet", (utf8*)"RootWindow");
        sheet->setSize(
            Absolute, 
            CEGUI::Size(Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth(), 
                Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight()));
        sheet->setPosition(Absolute, CEGUI::Point(0, 0));
        System::getSingleton().setGUISheet(sheet);
        TextureManager::getSingleton().setDefaultNumMipmaps(5);
        StaticText* panel = (StaticText*)WindowManager::getSingleton().createWindow("RastullahLook/StaticText", "Panel");
        sheet->addChildWindow(panel);
        panel->setPosition(Point(0.10f, 0.10f));
        panel->setSize(Size(0.80f, 0.80f));
        panel->setAlwaysOnTop(true);
        
        
	}
    
    void createScene()
    {
        Entity *e = mSceneMgr->createEntity("held", "held.mesh");
        SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        node->attachObject(e);
        node->setPosition(0,0,0);

        ColourValue c = ColourValue(0.5, 0.5, 0.5);
        mSceneMgr->setAmbientLight(c);
        setUp();
        mRoot->addFrameListener(this);
        showVideo();
    }

	void showVideo()
	{
        mVideo = new Video("Example/TheoraVideoPlayer/Play", "intro.ogv");
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
