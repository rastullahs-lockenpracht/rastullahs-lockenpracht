
#include "SoundPrerequisites.h"
#include "Video.h"
#include <cppunit/extensions/HelperMacros.h>
#include <boost/thread.hpp>
#include <CEGUI/CEGUI.h>
#include <OgreRenderWindow.h>
#include <OGRE/OgreCEGUIRenderer.h>
#include <CoreSubsystem.h>
#include <OgreSceneManager.h>
#include <OgreCEGUIResourceProvider.h>
#include <OgreFrameListener.h>
#include "World.h"
#include "Sleep.h"

using namespace boost;
using namespace rl;
using namespace CEGUI;
using namespace Ogre;

class VideoTest : public CppUnit::TestFixture, FrameListener {
private:
    thread *mThread;
    Video *mVideo;
    
public:
    VideoTest()
    {
    }
    
	void setUp()
	{
        mThread = 0;
        mVideo = 0;
        
        World* world = CoreSubsystem::getSingleton().getWorld();
        SceneManager* sceneMgr = world->getSceneManager();
        
        Ogre::RenderWindow* window = Ogre::Root::getSingleton().getAutoCreatedWindow();
        CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "Initialisiere CEGUI-Renderer", "UiSubsystem::initializeUiSubsystem");
        OgreCEGUIRenderer* rend = 
            new OgreCEGUIRenderer(window, 
                                Ogre::RENDER_QUEUE_OVERLAY, 
                                false, 
                                3000,
                                sceneMgr);

        CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "Initialisiere CEGUI-System", "UiSubsystem::initializeUiSubsystem");
        new System(rend, NULL, new OgreCEGUIResourceProvider(), (utf8*)"cegui.config"); 
        CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "CEGUI-System initialisiert", "UiSubsystem::initializeUiSubsystem");
        
        // load scheme and set up defaults
        ///@todo Hier sollte was Lookunabhängiges rein!!! FIXME TODO BUG!
        System::getSingleton().setDefaultMouseCursor((utf8*)"RastullahLook", (utf8*)"MouseArrow");
        CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "Mauszeiger", "UiSubsystem::initializeUiSubsystem");
        Window* sheet = CEGUI::WindowManager::getSingleton().createWindow((utf8*)"DefaultGUISheet", (utf8*)"RootWindow");
        CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "Rootfenster UiSubsystem::initializeUiSubsystem");
        sheet->setSize(
            Absolute, 
            CEGUI::Size(Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth(), 
                Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight()));
        sheet->setPosition(Absolute, CEGUI::Point(0, 0));
        System::getSingleton().setGUISheet(sheet);
        System::getSingleton().setTooltip("RastullahLook/Tooltip");
        CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "CEGUI geladen UiSubsystem::initializeUiSubsystem");
        TextureManager::getSingleton().setDefaultNumMipmaps(5);
        Ogre::Root::getSingleton().addFrameListener(this);

        Camera *mCamera = sceneMgr->createCamera("PlayerCam");

        // Position it at 500 in Z direction
        mCamera->setPosition(Ogre::Vector3(0,0,500));
        // Look back along -Z
        mCamera->lookAt(Ogre::Vector3(0,0,-300));
        mCamera->setNearClipDistance(5);
        Viewport* vp = window->addViewport(mCamera);
        vp->setBackgroundColour(ColourValue(0,0,0));

        // Alter the camera aspect ratio to match the viewport
        mCamera->setAspectRatio(
                Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

        
        mThread = new thread(*this);
	}

	void tearDown()
	{
        if (mThread)
        {
            delete mThread;
        }
	}

	void test_showVideo()
	{
        mVideo = new Video("/test/video", "intro.ogv");
        mVideo->play();
        msleep(10000);
	    CPPUNIT_ASSERT(true);
	}
    
    void operator()()
    {
        Ogre::Root::getSingleton().startRendering();
    }
    
    virtual bool frameStarted(const FrameEvent &evt)
    {
        if (mVideo)
        {
            mVideo->update();
        }
    }

	CPPUNIT_TEST_SUITE(VideoTest);
	CPPUNIT_TEST(test_showVideo);
    CPPUNIT_TEST_SUITE_END();
};
CPPUNIT_TEST_SUITE_REGISTRATION(VideoTest);
