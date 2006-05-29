/* 
	OgreNewt Library

	Ogre implementation of Newton Game Dynamics SDK

	OgreNewt basically has no license, you may use any or all of the library however you desire... I hope it can help you in any way.


	please note that the "fastdelegate" library file included here is not of my creation, refer to that file for information.


		by Walaber

*/

#ifndef _INCLUDE_OGRENEWT_BODY
#define _INCLUDE_OGRENEWT_BODY


#include "OgreNewt_Prerequisites.h"
#include <Ogre.h>
#include <Newton.h>
#include "OgreNewt_World.h"
#include "OgreNewt_Collision.h"
#include "OgreNewt_MaterialID.h"
#include "FastDelegate.h"


// OgreNewt namespace.  all functions and classes use this namespace.
namespace OgreNewt
{

/*
	CLASS DEFINITION:

		Body

	USE:
		this class represents a NewtonBody rigid body!
*/
//! main class for all Rigid Bodies in the system.
class _OgreNewtExport Body
{
public:
	//! custom force callbacFk.
	/*!
		this function is called from within the OgreNewt::World::update() command when applying forces to Rigid Bodies, such as
		gravity, etc.

		You can set this as the custom force callback for a body by using the setCustomForceCallback() function.
		Using fastdelegate means OgreNewt can now accept pointers to member functions of specific classes.
	*/
	typedef fastdelegate::FastDelegate1< OgreNewt::Body* > ForceCallback;

	//! custom transform callback.
	/*!
		This function is called from within the OgreNewt::World::update() command for all Rigid Bodies, after all collision and
		forces have been resolved.  this command is intended to allow the user to align a visual object with the new position and
		orientation of the rigid body.  OgreNewt has a general transform callback built-in for use with Ogre.  however you can 
		create your own for special cases.  you are passed a quaternion (orientation) and vector (position) of the rigid body
		in world space.
	*/
	typedef fastdelegate::FastDelegate3< OgreNewt::Body*, const Ogre::Quaternion&, const Ogre::Vector3& > TransformCallback;
	

	//! buoyancy plane callback
	/*!
		this function is a user-defined function that supplies Newton with the plane equation for the surface of the liquid when
		applying buoyancy forces.  the user should create their own function for this, that returns an Ogre::Plane based on their
		own criteria.  you get a pointer to the OgreNewt::Body, and it's current orientation and location to boot.  note that this
		callback will be called for each collision primitive in the body (if it is a compound collision).  if you want to
		ignore buoyancy for this collision primitive, just return false from the function.  otherwise, fill the "retPlane" with your
		liquid surface plane, and return true to apply buoyancy to the primitive.
	*/
	typedef fastdelegate::FastDelegate5< int, OgreNewt::Body*, const Ogre::Quaternion&, const Ogre::Vector3&, Ogre::Plane&, bool > buoyancyPlaneCallback;


	//! constructor.
	/*!
		creates a Rigid Body in an OgreNewt::World, based on a specific collision shape.
		\param W pointer to the OgreNewt::World/
		\param col pointer to an OgreNewt::Collision object that represents the shape of the rigid body.
		\param bodytype simple integer value used to identify the type of rigid body, useful for determining bodies in callbacks.
	*/
	Body( const World* W, CollisionPtr col, int bodytype = 0 );

	//! destructor
	~Body();

	//! set user data to connect this class to another.
	/*!
		you can use this to store a pointer to a parent class, etc.  then inside one of the many callbacks, you can get the pointer
		using this "userData" system.
	*/
	void setUserData( void* data ) { m_userdata = data; }

	//! retrieve pointer to previously set user data.
	void* getUserData() const { return m_userdata; }

	//! get a pointer to the NewtonBody object
	/*!
		this is the NewtonBody used by the Newton SDK.  in most cases you shouldn't need to access this.
	*/
	NewtonBody* getNewtonBody() const { return m_body; }

	//! get a pointer to the attached SceneNode.
	/*!
		if you have "attached" this body to an Ogre::SceneNode, this retrieves the node.
	*/
	Ogre::SceneNode* getOgreNode() const { return m_node; }

	//! get a pointer to the OgreNewt::World this body belongs to.
	const OgreNewt::World* getWorld() const { return m_world; }

	//! set the type for this body.
	/*!
		this sets the "type" for the body, which can also be set in the constructor.
		\param type integer value to represent the type of body, e.g. "FLOOR" or "CANON BALL", etc. used for differentation in material callbacks.
	*/
	void setType( int type ) { m_type = type; }

	//! get the type set for this body.
	int getType() const { return m_type; }

	//! attach this body to an Ogre::SceneNode*
	/*!
		This is an easy way to connect a Rigid Body with an Ogre::SceneNode.
		This automatically sets up a standard Transform callback when you call this.
		After calling this, the Ogre::SceneNode will have its position orientation updated
		to that of the Rigid Body each time you call World::update(),
		and the body has moved during the update.
	*/
	void attachToNode(Ogre::SceneNode* node, const Ogre::Vector3& offset = Ogre::Vector3::ZERO,
		const Ogre::Quaternion& orientationBias = Ogre::Quaternion::IDENTITY);

	//! set a standard gravity callback for this body to use.
	/*!
		This sets a very basic force callback for this body, that simply applies a standard 9,8m/s^2 gravity force to the body.
		in most cases you will need more forces acting on your Rigid Bodies, which you accomplich through custom force callbacks.
	*/
	void setStandardForceCallback();

	//! set a custom force callback for this body to use.
	/*
		This specifies a custom callback to use for applying forces to a body.  if you are using a standard non-member function, or a static member function, you can simply pass a pointer to the function here.. like this:
		setCustomForceAndTorqueCallback( &myCallbackFunction );

		If you want to bind to a class member, you also need to pass a pointer to the class itself, using the fastdelegate system, like so:
		setCustomForceAndTorqueCallback( fastdelegate::MakeDelegate( (MyClass*)classInstance, &MyClass::myCallback ) );  (from outside the class) or:
		setCustomForceAndTorqueCallback( fastdelegate::MakeDelegate( this, &MyClass::myCallback ) );  (from inside the class).
	*/
	void setCustomForceAndTorqueCallback( ForceCallback callback );

	//! remove any force callbacks.
	void removeForceAndTorqueCallback() { NewtonBodySetForceAndTorqueCallback( m_body, NULL );  m_forcecallback = NULL; }

	//! set a custom transform callback.
	/*
		sets a custom transform callback for the rigid body. see the docs on setCustomForceAndTorqueCallback for a description of how to use this funciton.
	*/
	void setCustomTransformCallback( TransformCallback callback );

	//! remove any transform callbacks.
	void removeTransformCallback() { m_transformcallback = NULL; }

	//! position and orient the body arbitrarily.
	/*!
		generally in a physics engine you shouldn't directly set the location/rotation of a Body, because this defies physics laws.  this command exists to set up bodies initially.
		\param orient quaternion representing body orientation in world space.
		\param pos vector representing body position in world space. 
	*/
	void setPositionOrientation( const Ogre::Vector3& pos, const Ogre::Quaternion& orient );

	//! set the mass and inertia for the body.
	/*!
		Set the mass of the Rigid Body.  Inertia is also set here.  Inertia represents a body's "resistance" to rotation around the 3 primary axis.  OgreNewt has a few utility functions that can help you calculate these values based on several primitive shapes.
		\param mass real value for the body mass
		\param inertia vector representing body moment of inertia
	*/
	void setMassMatrix( Ogre::Real mass, const Ogre::Vector3& inertia );

	//! set the body's center of mass
	/*!
		Set a new center of mass for the body that is different than the current, without offsetting the body.
		You can use this to adjust the center of mass of a body at runtime.
	*/
	void setCenterOfMass( const Ogre::Vector3& centerOfMass ) { NewtonBodySetCentreOfMass( m_body, &centerOfMass.x ); }

	//! get the center of mass.
	/*!
		returns the current center of mass, as an offset from the original origin when the body was created.
	*/
	Ogre::Vector3 getCenterOfMass() const;

	//! freeze the rigid body.
	/*!
		this command "freezes" the Rigid Body, removing it from the active simulation list.  it will "unfreeze" if another body comes in contact with it, or you "unfreeze" it.
		\sa unFreeze()
	*/
	void freeze() { NewtonWorldFreezeBody( m_world->getNewtonWorld(), m_body ); }

	//! unfreeze the rigid body.
	/*!
		\sa freeze()
	*/
	void unFreeze() { NewtonWorldUnfreezeBody( m_world->getNewtonWorld(), m_body ); }

	//! set the material for the body
	/*!
		Materials are an extremely powerful way to control body behavior. first create a new MaterialID object, and then pass a pointer
		to apply that material to the body. 
		\param ID pointer to an OgreNewt::MaterialID object to use as the material for the body.
	*/
	void setMaterialGroupID( const MaterialID* ID ) { m_matid = ID; NewtonBodySetMaterialGroupID( m_body, m_matid->getID() ); }
	
	//! prevents fast moving bodies from "tunneling" through other bodies.
	/*!
		continuous collision is an advanced feature that prevents fast moving bodies from "tunneling" (missing collision) with other bodies.  there 
		is a performance hit envolved, so this feature should only be used for bodies that have a high likelyhood of tunneling.
		
		note that continuous collision can also be set on a per-material basis via the MaterialPair class.
	*/
	void setContinuousCollisionMode( unsigned state ) { NewtonBodySetContinuousCollisionMode( m_body, state ); }

	//! set whether all parent/children pairs connected to this body should be allowed to collide.
	void setJointRecursiveCollision( unsigned state ) { NewtonBodySetJointRecursiveCollision( m_body, state ); }

	//! set an arbitrary omega for the body.
	/*!
		again, setting velocity/omega directly for a body in realtime is not recommended for proper physics behavior.  this function is intended to be used to setup a Body initially.
		\param omega vector representing the desired omega (rotational velocity)
	*/
	void setOmega( const Ogre::Vector3& omega ) { NewtonBodySetOmega( m_body, &omega.x ); }

	//! set an arbitrary velocity for the body.
	/*!
		again, setting velocity/omega directly for a body in realtime is not recommended for proper physics behavior.  this function is intended to be used to setup a Body initially.
		\param vel vector representing the desired velocity.
	*/
	void setVelocity( const Ogre::Vector3& vel ) { NewtonBodySetVelocity( m_body, &vel.x ); }

	//! set the linear damping for the body.
	void setLinearDamping( Ogre::Real damp ) { NewtonBodySetLinearDamping( m_body, (float)damp ); }

	//! set the angular damping for the body.
	void setAngularDamping( const Ogre::Vector3& damp ) { NewtonBodySetAngularDamping( m_body, &damp.x ); }

	//! set the Coriolos Forces mode for the body.
	void setCoriolisForcesMode( int mode ) { NewtonBodyCoriolisForcesMode( m_body, mode ); }

	//! set the collision that represents the shape of the body
	/*!
		This can be used to change the collision shape of a body mid-simulation.
		For example making the collision for a character smaller when crouching, etc.
		The old collision will get deleted.
		\param col pointer to the new OgreNewt::Collision shape.
	*/
	void setCollision(CollisionPtr col);

	//! set whether the body should "freeze" when equilibruim is reached.
	/*!
		user-controlled bodies should disable freezing, because frozen bodies' callbacks are not called... so a callback that implements motion based on user input will not be called!
	*/
	void setAutoFreeze( int flag ) { NewtonBodySetAutoFreeze ( m_body, flag); }

	//! set the factors that cause a body to "freeze" when equilibrium reached.
	void setFreezeThreshold( Ogre::Real speed, Ogre::Real omega, int framecount ) { NewtonBodySetFreezeTreshold( m_body, (float)speed, (float)omega, framecount ); }

	//! get a pointer to the OgreNewt::Collision for this body
	CollisionPtr Body::getCollision() const;

	//! get a pointer to the Material assigned to this body.
	const OgreNewt::MaterialID* getMaterialGroupID() const;

	//! returns current setting for this body.
	int getContinuousCollisionMode() const { return NewtonBodyGetContinuousCollisionMode( m_body ); }

	//! returns current setting for this body.
	int getJointRecursiveCollision() const { return NewtonBodyGetJointRecursiveCollision( m_body ); }

	//! get position and orientation in form of an Ogre::Vector(position) and Ogre::Quaternion(orientation)
	void getPositionOrientation( Ogre::Vector3& pos, Ogre::Quaternion& orient ) const;

	//! get Ogre::Real(mass) and Ogre::Vector3(inertia) of the body.
	void getMassMatrix( Ogre::Real& mass, Ogre::Vector3& inertia ) const;

	//! get invert mass + inertia for the body.
	void getInvMass( Ogre::Real& mass, Ogre::Vector3& inertia ) const;

	//! get omega of the body. in global space. 
	Ogre::Vector3 getOmega() const;

	//! get velocity of the body. in global coordinates.
	Ogre::Vector3 getVelocity() const;

	//! get whether the body is frozen or not
	/*!
		returns a value of 0 if the body is frozen.
		returns a value of 1 if the body is active.
	*/
	int getSleepingState() const { return NewtonBodyGetSleepingState( m_body ); }

	//! get auto-freeze state for the body
	int getAutoFreeze() const { return NewtonBodyGetAutoFreeze( m_body ); }

	//! get linear damping
	Ogre::Real getLinearDamping() const { return (Ogre::Real)NewtonBodyGetLinearDamping( m_body ); }

	//! get angular damping
	Ogre::Vector3 getAngularDamping() const;

	//! get the freeze threshold
	void getFreezeThreshold( Ogre::Real& speed, Ogre::Real& omega ) const { NewtonBodyGetFreezeTreshold( m_body, &speed, &omega ); }

	//! add an impulse (relative change in velocity) to a body.  values are in world coordinates.
	void addImpulse( const Ogre::Vector3& deltav, const Ogre::Vector3& posit ) { NewtonAddBodyImpulse( m_body, &deltav.x, &posit.x ); }

	//! add force to the body.  
	/*!
		this function is only valid inside a ForceCallback function!
	*/
	void addForce( const Ogre::Vector3& force ) { NewtonBodyAddForce( m_body, &force.x ); }

	//! add torque to the body.
	/*!
		this function is only valid inside a ForceCallback function!
	*/
	void addTorque( const Ogre::Vector3& torque ) { NewtonBodyAddTorque( m_body, &torque.x ); }

	//! set the force for a body.
	/*!
		this function is only valid inside a ForceCallback function!
	*/
	void setForce( const Ogre::Vector3& force ) { NewtonBodySetForce( m_body, &force.x ); }

	//! set the torque for a body.
	/*!
		this function is only valid inside a ForceCallback function!
	*/
	void setTorque( const Ogre::Vector3& torque ) { NewtonBodySetTorque( m_body, &torque.x ); }


	//! apply a buoyancy force to the body.
	/*!
		buoyancy is one of the more powerful and overlooked features of the Newton physics system.  you can of course
		simulate floating objects, and even lighter-than-air objects like balloons, etc.
		\param fluidDensity density of the fluid.
		\param fluidLinearViscosity how much the fluid slows linear motion
		\param fluidAngularViscosity how much the fluid slows rotational motion
		\param gravity vector representing world gravity.
		\param buoyancyPlaneCallback user function that returns the plane equation for the fluid at the current location. pass NULL to assume the body is fully immersed in fluid.  see the setCustomForceAndTorqueCallback() docs to info on how to bind class member functions.
	*/
	void addBouyancyForce( Ogre::Real fluidDensity, Ogre::Real fluidLinearViscosity , Ogre::Real fluidAngularViscosity , const Ogre::Vector3& gravity, buoyancyPlaneCallback callback );



	//! helper function that adds a force (and resulting torque) to an object in global cordinates.
	/*!
		this function is only valid inside a ForceCallback function!
		\param force vector representing force, in global space
		\param pos vector representing location of force, in global space
	*/
	void addGlobalForce( const Ogre::Vector3& force, const Ogre::Vector3& pos );

	// helper function that adds a force (and resulting torque) to an object in local coordinates.
	/*!
		this function is only valid inside a ForceCallback function!
		\param force vector representing force, in local space of the body
		\param pos vector representing locatino of force, in local space of the body
	*/
	void addLocalForce( const Ogre::Vector3& force, const Ogre::Vector3& pos );

    Ogre::Vector3 getOffset() const;
    void setOffset(const Ogre::Vector3& offset);

 protected:

	NewtonBody*			m_body;
	CollisionPtr	    m_collision;
	const MaterialID*	m_matid;
	const World*		m_world;
	

	void*				m_userdata;
	
	int					m_type;
	Ogre::SceneNode*	m_node;
	Ogre::Vector3       m_offset;
	Ogre::Quaternion    m_orientationBias;

	ForceCallback			m_forcecallback;
	TransformCallback		m_transformcallback;
	buoyancyPlaneCallback	m_buoyancycallback;

private:

	static void _CDECL newtonDestructor( const NewtonBody* body );

	static void _CDECL newtonTransformCallback( const NewtonBody* body, const float* matrix );
	static void _CDECL newtonForceTorqueCallback( const NewtonBody* body );

	static int _CDECL newtonBuoyancyCallback( const int collisionID, void* context, const float* globalSpaceMatrix, float* globalSpacePlane );

	// standard gravity force callback.
	static void standardForceCallback( Body* me );

	// standard transform callback.
	static void standardTransformCallback( Body* me, const Ogre::Quaternion& orient, const Ogre::Vector3& pos );
};




}

#endif
// _INCLUDE_OGRENEWT_BODY

