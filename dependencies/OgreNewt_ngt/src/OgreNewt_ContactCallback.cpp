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

int _CDECL ContactCallback::onAABBOverlap( const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1, int threadIndex )
{
	ContactCallback* me;

	me = (ContactCallback*)NewtonMaterialGetMaterialPairUserData( material );

	me->m_material = (NewtonMaterial*)material;

	//save the bodies...
	me->m_body0 = (OgreNewt::Body*)NewtonBodyGetUserData( body0 );
	me->m_body1 = (OgreNewt::Body*)NewtonBodyGetUserData( body1 );

	return me->onAABBOverlap( threadIndex );
}

void _CDECL ContactCallback::contactProcess(const NewtonJoint *contactJoint, float timeStep, int threadIndex )
{
/*    // pruefe ob nur TriggerVolume - funktioniert mit aktuellem newton noch nicht
    if( NewtonCollisionIsTriggerVolume(NewtonBodyGetCollision(NewtonJointGetBody1(contactJoint))) ||
        NewtonCollisionIsTriggerVolume(NewtonBodyGetCollision(NewtonJointGetBody1(contactJoint))) )
    {
	    ContactCallback *me = (ContactCallback*)NewtonJointGetUserData(contactJoint);
        me->m_material = NULL;
        me->m_body0 = (OgreNewt::Body*)NewtonBodyGetUserData( NewtonJointGetBody0(contactJoint) );
    	me->m_body1 = (OgreNewt::Body*)NewtonBodyGetUserData( NewtonJointGetBody1(contactJoint) );
        me->contactProcess( (Ogre::Real)timeStep, threadIndex );

        return;
    }
*/
bool test = false;
        for (void* contact = NewtonContactJointGetFirstContact (contactJoint); contact; contact = NewtonContactJointGetNextContact (contactJoint, contact))
        {
	        ContactCallback *me;
                NewtonMaterial* material = NewtonContactGetMaterial(contact);

                me = (ContactCallback*)NewtonMaterialGetMaterialPairUserData( material );

                me->m_material = material;
                me->m_body0 = (OgreNewt::Body*)NewtonBodyGetUserData( NewtonJointGetBody0(contactJoint) );
	        me->m_body1 = (OgreNewt::Body*)NewtonBodyGetUserData( NewtonJointGetBody1(contactJoint) );

	        // call the user-defined callback function!
	        if( !me->contactProcess( (Ogre::Real)timeStep, threadIndex ) )
                {
                    NewtonContactJointRemoveContact(contactJoint, contact);
                }
        }
}

/*
void _CDECL ContactCallback::contactEnd( const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1, int threadIndex )
{
	ContactCallback* me;

	me = (ContactCallback*)NewtonMaterialGetMaterialPairUserData( material );

	me->m_material = (NewtonMaterial*)material;
	me->m_body0 = (OgreNewt::Body*)NewtonBodyGetUserData( body0 );
	me->m_body1 = (OgreNewt::Body*)NewtonBodyGetUserData( body1 );

	me->userEnd( threadIndex );
}
*/




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
