#ifndef __World_H__
#define __World_H__

#include "CorePrerequisites.h"


namespace Ogre {

    class SceneManager;

}

namespace rl {

class CameraActor;

class _RlCoreExport World
{
public:
    enum FogMode { FOG_NONE, FOG_EXP, FOG_EXP2, FOG_LINEAR  };

    World() : mSceneMgr(0), mCamera(0) {}

    /**@todo Klaeren, wer für Zerstoerung von mCamera und mSceneMgr
     *verantwortlich ist. */
    virtual ~World() {}

    virtual SceneManager* getSceneManager(void);
	virtual void setSceneManager(SceneManager* SceneMgr);

	virtual CameraActor* getActiveCamera(void) const { return mCamera; };
	
    virtual void clearScene(void) = 0;
	virtual void loadScene(const String& levelName ) = 0;
    virtual void initializeDefaultCamera(void) = 0;	

    //Enables / disables a 'sky plane' i.e.
    virtual void setSkyPlane(bool enable, const Plane &plane,
        const String &materialName, Real scale=1000, Real tiling=10,
        bool drawFirst=true, Real bow=0);
 	
    //Enables / disables a 'sky box' i.e.
    virtual void setSkyBox(bool enable, const String &materialName,
        Real distance=5000, bool drawFirst=true );
 	    
    //Enables / disables a 'sky dome' i.e.
    virtual void setSkyDome(bool enable, const String &materialName,
        Real curvature=10, Real tiling=8, Real distance=4000,
        bool drawFirst=true );
 	    
    //Sets the fogging mode applied to the scene.
    virtual void setFog(FogMode mode=FOG_NONE,
        const ColourValue &colour=ColourValue::White,
        Real expDensity=0.001, Real linearStart=0.0, Real linearEnd=1.0);
 	    
    //Returns the fog mode for the scene.
    virtual FogMode getFogMode (void) const;
 	    
    //Returns the fog colour for the scene.
    virtual const ColourValue & getFogColour (void) const;
 	    
    //Returns the fog start distance for the scene.
    virtual Real getFogStart (void) const;
 	    
    //Returns the fog end distance for the scene.
    virtual Real getFogEnd (void) const;

    virtual void setAmbientLight(Real r, Real g, Real b);
    
    virtual Quaternion getWorldAxis();
protected:
	World(Ogre::SceneType sceneType, bool hasPhysics);

    SceneManager* mSceneMgr;
    CameraActor* mCamera;
    bool mbSceneLoaded;
};

}
#endif
