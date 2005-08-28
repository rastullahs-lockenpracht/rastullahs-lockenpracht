/*-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright © 2000-2003 The OGRE Team
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
LGPL like the rest of the engine.
-----------------------------------------------------------------------------
*/
/*
-----------------------------------------------------------------------------
Filename:    ExampleApplication.h
Description: Base class for all the OGRE examples
-----------------------------------------------------------------------------
*/

#ifndef __ExampleApplication_H__
#define __ExampleApplication_H__

#include "Ogre.h"
#include "OgreConfigFile.h"
#include "ExampleFrameListener.h"


namespace Ogre
{

	/** Base class which manages the standard startup of an Ogre application.
		Designed to be subclassed for specific examples if required.
	*/
	class ExampleApplication
	{
	public:
		/// Standard constructor
		ExampleApplication()
		{
            mFrameListener = 0;
			mRoot = 0;
		}
		/// Standard destructor
		virtual ~ExampleApplication()
		{
            if (mFrameListener)
                delete mFrameListener;
			if (mRoot)
				delete mRoot;
		}

		/// Start the example
		virtual void go(void)
		{
			if (!setup())
				return;

			mRoot->startRendering();
		}

	protected:
		Root *mRoot;
		Camera* mCamera;
		SceneManager* mSceneMgr;
		RenderWindow* mWindow;
        ExampleFrameListener* mFrameListener;

		// These internal methods package up the stages in the startup process
		/** Sets up the application - returns false if the user chooses to abandon configuration. */
		virtual bool setup(void)
		{
            static std::string CONF_DIR = "./modules/common/conf/";
            mRoot =
            #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
                new Ogre::Root( 
                        CONF_DIR+"plugins-win.cfg", 
                        CONF_DIR+"rastullah.cfg", 
                        "logs/ogre.log" );
            #elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
                new Ogre::Root( 
                    CONF_DIR+"plugins-linux.cfg", 
                    CONF_DIR+"rastullah.cfg", 
                     "logs/ogre.log" );
            #else
                new Ogre::Root( 
                        CONF_DIR+"plugins-mac.cfg", 
                      CONF_DIR+"rastullah.cfg", 
                        "logs/ogre.log" );
            #endif
//			setupResources();

			bool carryOn = configure();
			if (!carryOn) return false;

            ResourceGroupManager::getSingleton().addResourceLocation("./modules/common/sound", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/minidemo/sound", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/common/models", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/minidemo/models", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/common/gui", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/minidemo/gui", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/common/gui/fonts", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/minidemo/gui/fonts", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/common/gui/imagesets", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/minidemo/gui/imagesets", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/common/gui/schemes", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/minidemo/gui/schemes", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/common/gui/windows", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/minidemo/gui/windows", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation(".", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/common/materials", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/minidemo/materials", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/common/materials/alpha", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/common/OgreCore.zip", "Zip");
//            ResourceGroupManager::getSingleton().addResourceLocation("./modules/minidemo/materials/alpha", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/common/materials/env", "FileSystem");
//            ResourceGroupManager::getSingleton().addResourceLocation("./modules/minidemo/materials/env", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/common/materials/model", "FileSystem");
//            ResourceGroupManager::getSingleton().addResourceLocation("./modules/minidemo/materials/model", "FileSystem");
            ResourceGroupManager::getSingleton().addResourceLocation("./modules/common/materials/tiled", "FileSystem");
//            ResourceGroupManager::getSingleton().addResourceLocation("./modules/minidemo/materials/tiled", "FileSystem");

            ResourceGroupManager::getSingleton().initialiseAllResourceGroups();


			chooseSceneManager();
			createCamera();
			createViewports();

			// Set default mipmap level (NB some APIs ignore this)
			TextureManager::getSingleton().setDefaultNumMipmaps(5);

			// Create the scene
			createScene();

			createFrameListener();

			return true;

		}
		/** Configures the application - returns false if the user chooses to abandon configuration. */
		virtual bool configure(void)
		{
			// Show the configuration dialog and initialise the system
			// You can skip this and use root.restoreConfig() to load configuration
			// settings if you were sure there are valid ones saved in ogre.cfg
			if(mRoot->showConfigDialog())
			{
				// If returned true, user clicked OK so initialise
				// Here we choose to let the system create a default rendering window by passing 'true'
				mWindow = mRoot->initialise(true);
				return true;
			}
			else
			{
				return false;
			}
		}

		virtual void chooseSceneManager(void)
		{
			// Get the SceneManager, in this case a generic one
			mSceneMgr = mRoot->getSceneManager(ST_GENERIC);
		}
		virtual void createCamera(void)
		{
			// Create the camera
			mCamera = mSceneMgr->createCamera("PlayerCam");

			// Position it at 500 in Z direction
			mCamera->setPosition(Vector3(0,0,500));
			// Look back along -Z
			mCamera->lookAt(Vector3(0,0,-300));
			mCamera->setNearClipDistance(5);

		}
		virtual void createFrameListener(void)
		{
            mFrameListener= new ExampleFrameListener(mWindow, mCamera);
            mRoot->addFrameListener(mFrameListener);
		}

		virtual void createScene(void) = 0;    // pure virtual - this has to be overridden

		virtual void createViewports(void)
		{
			// Create one viewport, entire window
			Viewport* vp = mWindow->addViewport(mCamera);
			vp->setBackgroundColour(ColourValue(0,150,0));

			// Alter the camera aspect ratio to match the viewport
			mCamera->setAspectRatio(
				Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
		}

		/// Method which will define the source of resources (other than current folder)
		virtual void setupResources(void)
		{
			ConfigFile cf;
			cf.load("resources.cfg");

			// Go through all sections & settings in the file
			ConfigFile::SectionIterator seci = cf.getSectionIterator();

			String secName, typeName, archName;
			while (seci.hasMoreElements())
			{
	            secName = seci.peekNextKey();
		        ConfigFile::SettingsMultiMap *settings = seci.getNext();
			    ConfigFile::SettingsMultiMap::iterator i;
	            for (i = settings->begin(); i != settings->end(); ++i)
		        {
			        typeName = i->first;
				    archName = i->second;
	                ResourceGroupManager::getSingleton().addResourceLocation(
		                archName, typeName, secName);
			    }
			}
		}
	};

} //end namesapce
#endif

