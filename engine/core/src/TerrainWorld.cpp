#include "TerrainWorld.h"
#include "CameraActor.h"
#include "ActorFactory.h"

namespace rl {

	TerrainWorld::TerrainWorld() : World(ST_EXTERIOR_CLOSE, false)
	{
	}

	TerrainWorld::~TerrainWorld()
	{
	}

	void TerrainWorld::clearScene()
	{
	}

	void TerrainWorld::loadScene(const String& levelName)
	{	
        initializeDefaultCamera();

        Plane waterPlane;

        // Set ambient light
        mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

        // Create a light
        Light* l = mSceneMgr->createLight("MainLight");
        // Accept default settings: point light, white diffuse, just set position
        // NB I could attach the light to a SceneNode if I wanted it to move automatically with
        //  other objects, but I don't
        l->setPosition(20,80,50);

        // Fog
        // NB it's VERY important to set this before calling setWorldGeometry 
        // because the vertex program picked will be different
        ColourValue fadeColour(0.93, 0.86, 0.76);
        setFog( FOG_EXP, fadeColour, .001);


        mSceneMgr -> setWorldGeometry( levelName );
        
        // Define the required skyplane
        Plane plane;
        // 5000 world units from the camera
        plane.d = 5000;
        // Above the camera, facing down
        plane.normal = -Vector3::UNIT_Y;
	}

	void TerrainWorld::initializeDefaultCamera()
	{
				// Also change position, and set Quake-type orientation
        // Get random player start point
		mCamera = ActorFactory::getSingleton().createCameraActor("DefaultCamera");

		// Position it at 500 in Z direction
        mCamera->setPosition(Vector3(128,25,128));
        // Look back along -Z
        mCamera->lookAt(Vector3(0,0,-300));
        mCamera->setNearClipDistance( 1 );
        mCamera->setFarClipDistance( 384 );

		// Set a nice viewpoint
        mCamera->setPosition(707,2500,528);
        mCamera->setOrientation(Quaternion(-0.3486, 0.0122, 0.9365, 0.0329));
        //mRoot -> showDebugOverlay( true );

		// Don't yaw along variable axis, causes leaning
        mCamera->setFixedYawAxis(true, Vector3::UNIT_Z);

		// Infinite far plane?
		if (Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE))
        {
            mCamera->setFarClipDistance(0);
        }

        // Create one viewport, entire window
        Viewport* newVp = Ogre::Root::getSingleton().getAutoCreatedWindow()->addViewport(mCamera->getOgreCamera(),1);
        ColourValue fadeColour(0.93, 0.86, 0.76);
        newVp->setBackgroundColour(fadeColour);
	}

	Quaternion TerrainWorld::getWorldAxis()
	{
		return Quaternion::IDENTITY;
	}

}
