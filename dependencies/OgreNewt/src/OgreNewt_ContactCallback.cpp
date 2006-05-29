#include <OgreNewt_ContactCallback.h>
#include <OgreNewt_Body.h>

namespace OgreNewt
{

	
ContactCallback::ContactCallback()
{

}

ContactCallback::~ContactCallback()
{
}

int _CDECL ContactCallback::contactBegin( const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1 )
{
	ContactCallback* me;

	me = (ContactCallback*)NewtonMaterialGetMaterialPairUserData( material );

	me->m_material = (NewtonMaterial*)material;

	//save the bodies...
	me->m_body0 = (OgreNewt::Body*)NewtonBodyGetUserData( body0 );
	me->m_body1 = (OgreNewt::Body*)NewtonBodyGetUserData( body1 );

	return me->userBegin();
}

int _CDECL ContactCallback::contactProcess( const NewtonMaterial* material, const NewtonContact* contact )
{
	ContactCallback *me;

	me = (ContactCallback*)NewtonMaterialGetMaterialPairUserData( material );

	me->m_material = (NewtonMaterial*)material;
	me->m_contact = (NewtonContact*)contact;

	// call the user-defined callback function!
	return me->userProcess();

}

void _CDECL ContactCallback::contactEnd( const NewtonMaterial* material )
{
	ContactCallback* me;

	me = (ContactCallback*)NewtonMaterialGetMaterialPairUserData( material );

	me->m_material = (NewtonMaterial*)material;

	me->userEnd();
}





Ogre::Vector3 ContactCallback::getContactForce() const 
{
	Ogre::Vector3 force;
	NewtonMaterialGetContactForce( m_material, &force.x );
	return force;
}


void ContactCallback::getContactPositionAndNormal( Ogre::Vector3& pos, Ogre::Vector3& norm ) const 
{
	NewtonMaterialGetContactPositionAndNormal( m_material, &pos.x, &norm.x );
}


void ContactCallback::getContactTangentDirections( Ogre::Vector3& dir0, Ogre::Vector3& dir1 ) const 
{
	NewtonMaterialGetContactTangentDirections( m_material, &dir0.x, &dir1.x );
}





}
