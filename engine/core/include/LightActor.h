#ifndef __LightActor_H__
#define __LightActor_H__

#include <OgreLight.h>

#include "CorePrerequisites.h"
#include "Actor.h"

namespace rl {

/** 
@brief
    A specifc Actor containing a Light
@remark
	This Actor wraps all the functinality given by a 
	Ogre Light
@par
	The Scenenode in which the Light resides and the Light
@see 
	Ogre::Light
*/
class _RlCoreExport LightActor : public virtual Actor
{
public:
    /** Constructor
		@param name The Actors name
		@param pSceneNode The SceneNode of the Camera
		@param pLight The Light
	*/
	LightActor( const String& name, Light* pLight );

	/** Constructor
		@param name The Actors name
		@param pParentSceneNode The Parent SceneNode for the Camera
		@param pLight The Light
	*/
	LightActor( const String& name, SceneNode* pParentSceneNode, Light* pLight );

	/** Default Deconstructor */
	virtual ~LightActor();

	/** Returns the Typename */
	virtual const String& getTypeName();

	/** Gets the internal Camera object. */
	Light* getOgreLight(void)
	{
		return mLight;
	}

    void setAttenuation( Real range, Real constant,Real linear, Real quadric );
    void setDiffuseColour( Real red, Real green, Real blue );
    void setDirection( Real x, Real y, Real z );
    void setSpecularColour( Real red, Real green, Real blue );
    void setSpotlightRange( Real innerangle, Real outerangle, Real falloff = 1 );
    void setVisible( bool visible );

	static LightActor* castDown( Actor* pActor )
	{ return dynamic_cast<LightActor*>(pActor);  }
protected:
	static const String TYPENAME;
	Light *mLight;
};

}
#endif
