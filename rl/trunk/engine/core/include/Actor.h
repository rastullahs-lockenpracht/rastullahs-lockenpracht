#ifndef __Actor_H__
#define __Actor_H__

#include <OgreUserDefinedObject.h>

#include "CorePrerequisites.h"

namespace rl {

class PhysicalThing;

/** The Base Actor
	@remarks
		Actors are not to be generated directly, but via
		factory methods in ActorFactory. This is the base Actor, which
		is extended in other classes. It extends Ogres UserDefinedObject
		to be assigned to an OgreObject.
	@par 
		Every Actor has a unique name. Every Actorclass also has a Typename.
*/
class _RlCoreExport Actor : public UserDefinedObject
{
public:
	/** Constructs an Actor 
		@param name The Actors unique name
	*/
	Actor( const String& name);
	/** Constructs an Actor 
		@param name The Actors unique name
		@param pParentSceneNode The parent SceneNode for the Actors Scenenode
	*/
	Actor( const String& name, SceneNode* pParentSceneNode);

	/** Default Deconstructor */
	virtual ~Actor();
	
	/** Gets the SceneNode which is being used to represent this object's position in
	       the OGRE world. */
	virtual SceneNode* getSceneNode(void);
    virtual void setSceneNode(SceneNode* node);

     /** Connects another Actor to this Actor only via the SceneNode
        Use this only for Actor which none overwritten methods    */
    void attachActorToSceneNode(Actor* actor);
    /** Disconnects another Actor from this Actor with was connected only via the SceneNode*/
    void detachActorFromSceneNode(Actor* actor);

	/** Returns the TypeName */
	virtual const String& getTypeName();

    virtual bool isSimpleOgreActor();

    /** Returns the unique Name of this Actor */
	const String& getName();

    PhysicalThing* getPhysicalThing();
    void setPhysicalThing( PhysicalThing* thing );

    // MOVABLE

    /** Gets the current position of this object. */
	const Vector3& getPosition(void);
    /** Gets the current orientation of this object. */
	const Quaternion& getOrientation(void);

    // Override these
    
    /** Sets the position of this object. */
	virtual void setPosition(Real x, Real y, Real z);
    /** Sets the orientation of this object. */
    virtual void setOrientation(Real w, Real x, Real y, Real z);

    // These only use the methods above
    
    /** Sets the position of this object. */
	virtual void setPosition(const Vector3& vec);
	/** Sets the orientation of this object. */
	virtual void setOrientation(const Quaternion& orientation); 
	/** Moves the object along it's local  axes.
	           @par
	               This method moves the object by the supplied vector along the
	               local axes of the obect.
	           @param 
	               d Vector with x,y,z values representing the translation.
	       */
	void translate(const Vector3& d);
	/** Moves the object along it's local axes.
	    @par
	        This method moves the object by the supplied vector along the
	        local axes of the obect.
	    @param x, y z Real x, y and z values representing the translation.
	*/
	void translate(Real x, Real y, Real z);
	/** Moves the object along the world axes.
	    @par
	        This method moves the object by the supplied vector along the
	        world axes.
	    @param 
	        d Vector with x,y,z values representing the translation.
	*/
	void translateWorldSpace(const Vector3& d);
	/** Moves the object along the world axes.
	    @par
	        This method moves the object by the supplied vector along the
	        local axes of the obect.
	    @param x, y z Real x, y and z values representing the translation.
	*/
	void translateWorldSpace(Real x, Real y, Real z);
	/** Rotate the object around the local Z-axis.
	*/
	void roll(Real angleunits);
	/** Rotate the object around the local X-axis.
	*/
	void pitch(Real angleunits);
	/** Rotate the object around the local Y-axis.
	*/
	void yaw(Real angleunits);
	/** Rotate the object around an arbitrary axis.
	*/
	void rotate(const Vector3& axis, Real angleunits);
	/** Rotate the object around an arbitrary axis.
	*/
	void rotate(Real x,Real y,Real z, Real angleunits);
	/** Rotate the object around an aritrary axis using a Quarternion.
	*/
	void rotate(const Quaternion& q);

protected:
	/** The TypeName */
	static const String TYPENAME;
	/** The Position in the OgreWorld */
	SceneNode* mSceneNode;
    /** The Physics Body */
    PhysicalThing* mPhysical;
	/** The Name */
	String mName;
};
}
#endif
