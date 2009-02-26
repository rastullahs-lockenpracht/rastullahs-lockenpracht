/* 
	OgreNewt Library

	Ogre implementation of Newton Game Dynamics SDK

	OgreNewt basically has no license, you may use any or all of the library however you desire... I hope it can help you in any way.

		by Walaber

*/
#ifndef _INCLUDE_OGRENEWT_CONTACTCALLBACK
#define _INCLUDE_OGRENEWT_CONTACTCALLBACK

#include <Newton.h>
#include "OgreNewt_Body.h"

// OgreNewt namespace.  all functions and classes use this namespace.
namespace OgreNewt
{


//! custom contact behavior
/*!
	this class is for creating custom behavior between material GroupIDs.
	this class must be inherited, and the user functions created, and then
	added to a MaterialPair class.
*/
class _OgreNewtExport ContactCallback
{
public:

	//! constructor
	ContactCallback();

	//! destructor
	virtual ~ContactCallback();

	// basic contact control commands...

	//! disable the current contact.
	void disableContact() const { NewtonMaterialDisableContact( m_material ); }

	//! get the current physics timestep
	Ogre::Real getCurrentTimestep() const { return (Ogre::Real)NewtonMaterialGetCurrentTimestep( m_material ); }

	//! get the face ID of a TreeCollision object
	unsigned getContactFaceAttribute() const { return NewtonMaterialGetContactFaceAttribute( m_material ); }

	//! get the Collision ID of a body currently colliding
	unsigned getBodyCollisionID( OgreNewt::Body* body ) const { return NewtonMaterialGetBodyCollisionID( m_material, body->getNewtonBody() ); }

	//! speed of the collision
	Ogre::Real getContactNormalSpeed() const { return (Ogre::Real)NewtonMaterialGetContactNormalSpeed( m_material, m_contact ); }

	//! force of the collision
	/*!
		only valid for objects in a stable state (sitting on top of each other, etc)
	*/
	Ogre::Vector3 getContactForce() const;

	//! get positoin and normal of the collision
	void getContactPositionAndNormal( Ogre::Vector3& pos, Ogre::Vector3& norm ) const;

	//! get the tangent vectors of the collision
	void getContactTangentDirections( Ogre::Vector3& dir0, Ogre::Vector3& dir1 ) const;

	//! get tangent speed of the collision
	Ogre::Real getContactTangentSpeed( int index ) const { return (Ogre::Real)NewtonMaterialGetContactTangentSpeed( m_material, m_contact, index ); }

	//! set softness of the current contact
	void setContactSoftness( Ogre::Real softness ) const { NewtonMaterialSetContactSoftness( m_material, (float)softness ); }

	//! set elasticity of the current contact
	void setContactElasticity( Ogre::Real elasticity ) const { NewtonMaterialSetContactElasticity( m_material, (float)elasticity ); }

	//! set friction state of current contact
	void setContactFrictionState( int state, int index ) const { NewtonMaterialSetContactFrictionState( m_material, state, index ); }

	//! set static friction for current contact
	void setContactStaticFrictionCoef( Ogre::Real coef, int index ) const { NewtonMaterialSetContactStaticFrictionCoef( m_material, (float)coef, index ); }

	//! set kinetic friction for current contact
	void setContactKineticFrictionCoef( Ogre::Real coef, int index ) const { NewtonMaterialSetContactKineticFrictionCoef( m_material, (float)coef, index ); }

	//! set tangent acceleration for contact
	void setContactTangentAcceleration( Ogre::Real accel, int index ) const { NewtonMaterialSetContactTangentAcceleration( m_material, (float)accel, index ); }

	//! align tangent vectors with a user supplied direction
	void rotateTangentDirections( const Ogre::Vector3& dir ) const { NewtonMaterialContactRotateTangentDirections( m_material, &dir.x ); }

	//! manually set the normal for the collision.
	void setContactNormalDirection( const Ogre::Vector3& dir ) const { NewtonMaterialSetContactNormalDirection( m_material, &dir.x ); }

	//! manually set the acceleration along the collision normal.
	void setContactNormalAcceleration( Ogre::Real accel ) const { NewtonMaterialSetContactNormalAcceleration( m_material, accel ); }



	// user-defined callback function.
	
	//! user defined Begin function
	/*!
		this function is called when 2 bodies AABB overlap.  they have not yet collided, but *may* do so this loop.
		at this point, m_body0 and m_body1 are defined, but the contact isn't yet valid, so none of the member functions
		can be called yet.  they must be called from the userProcess() function.
		return 0 to ignore the collision, 1 to allow it.
	*/
	virtual int userBegin() { return 1; }

	//! user-defined Process function
	/*!
		user process function.  is called for each contact between the 2 bodies.  all member functions are valid from
		within this function, and will affect the current contact.  return 0 to ignore the collision, 1 to allow it.
	 */
	virtual int userProcess() { return 1; }

	//! user-defined End function
	/*!
		called after all contacts between the 2 bodies have been processed. no member functions should be called from within this
		function either, as all contacts have been processed at this point.
	*/
	virtual void userEnd() {  }

	//! get the NewtonMaterial from this callback.
	NewtonMaterial* _getNewtonMaterial() { return m_material; }

	//! get the NewtonContact from this callback.
	NewtonContact* _getNewtonContact() { return m_contact; }


	//! internal function.
	static int _CDECL contactBegin( const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1 );
	//! internal function.
	static int _CDECL contactProcess( const NewtonMaterial* material, const NewtonContact* contact );
	//! internal function.
	static void _CDECL contactEnd( const NewtonMaterial* material );


protected:

	NewtonMaterial* m_material;
	NewtonContact* m_contact;

	OgreNewt::Body* m_body0;
	OgreNewt::Body* m_body1;
};


}	// end NAMESPACE OgreNewt

#endif
// _INCLUDE_OGRENEWT_CONTACTCALLBACK

