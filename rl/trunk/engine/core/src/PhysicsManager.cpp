#include "PhysicsManager.h"

#include "OgreNoMemoryMacros.h"
#include <ode/ode.h>
#include "OgreMemoryMacros.h"

#include "CoreSubsystem.h"
#include "World.h"
#include "PhysicalThing.h"
#include "Actor.h"

template<> rl::PhysicsManager* Singleton<rl::PhysicsManager>::ms_Singleton = 0;

// Define a max macro
#ifndef max
#define max(a,b) (((a)>=(b))?(a):(b))
#endif

namespace rl
{

    PhysicsManager::PhysicsManager() : mPhysicalThings(), mSimpleSpaces()
    {

        mWorld = new dWorld();
        mGlobalSpace = new dHashSpace(0);
        mCurrSpace = mGlobalSpace;
        mContactJointGroup = new dJointGroup();

        mQuery = 0;
        mStepSize = 0.005;
        mLeftOverTime = 0.0;
        setGravity( 0, 0, -9.80665 );
        mWorld->setCFM( 1e-5 );
        mWorld->setERP( 0.8 );
        dWorldSetAutoDisableFlag (mWorld->id(),1);
        //dWorldSetContactMaxCorrectingVel (mWorld->id(),0.2);
        //dWorldSetContactSurfaceLayer (mWorld->id(),0.001);

        m_Enabled = false;
    }

    PhysicsManager::~PhysicsManager()
    {
        mContactJointGroup->empty();
        delete mGlobalSpace;
        delete mWorld;
        delete mContactJointGroup;

        if( mQuery )
        {
            mQuery->clearResults();
            delete mQuery;
        }
    }

    void PhysicsManager::run( Real elapsedTime )
    {
        if( m_Enabled && elapsedTime > 0.0f )
        {
            int steps = (int) floor( ( ( elapsedTime + mLeftOverTime ) / mStepSize ) );

            for( int i = 0; i < steps ; i++ )
            {
                // Collide stuff ( to enable collision between non-movables )
                mGlobalSpace->collide((void*)this,&PhysicsManager::collisionCallback);

                // World Collision
                IntersectionSceneQueryResult& results = mQuery->execute();
                // World can only collide with OgreMovables 
                for (SceneQueryMovableWorldFragmentIntersectionList::iterator wIter = results.movables2world.begin(); 
                    results.movables2world.end() != wIter; ++wIter)
                {
                    MovableObject* mo = wIter->first;
                    SceneQuery::WorldFragment* wf = wIter->second;

                    UserDefinedObject* uo = mo->getUserObject();


                    if( uo )
                    {
                        // Cast to Actor
                        Actor* actor = static_cast<Actor*>(uo);

                        PhysicalThing* phys = actor->getPhysicalThing();

                        if( phys && phys->isColliding() )
                        {
                            //LogManager::getSingleton().logMessage( String("World kollidiert evtl. mit ")  << actor->getName() );
                            phys->testCollide( wf );
                        }
                    }

                }

                // Step the world 
                // dWorldQuickStep(mWorld->id(), elapsedTime );
                mWorld->step(mStepSize);

                // Empty ContactJoints ( no longer needed )
                mContactJointGroup->empty();

                // Update the positions of everything that we're keeping track of
                for(PhysicalThingActorMap::iterator i = mPhysicalThings.begin();i != mPhysicalThings.end();i++)
                {
                    i->second->update();
                }
            }

            mLeftOverTime = ( elapsedTime + mLeftOverTime ) - ( steps * mStepSize );
        }
    }

    dJointGroup*  PhysicsManager::getContactJointGroup()
    {
        return mContactJointGroup;
    }

    void PhysicsManager::setWorldScene( World* world )
    {
        if (world == NULL)
            setEnabled(false);
        else
        {
            SceneManager* sceneMgr = world->getSceneManager();

            if( mQuery )
                sceneMgr->destroyQuery( mQuery );

            mQuery = sceneMgr->createIntersectionQuery();
            // BSP
            mQuery->setWorldFragmentType( SceneQuery::WFT_PLANE_BOUNDED_REGION );
        }
    }

    void PhysicsManager::setCFM(Real cfm)
    {
        mWorld->setCFM(cfm);
    }

    Real PhysicsManager::getCFM()
    {
        return mWorld->getCFM();
    }

    void PhysicsManager::setERP(Real erp)
    {
        mWorld->setERP(erp);
    }

    Real PhysicsManager::getERP()
    {
        return mWorld->getERP();
    }

    void PhysicsManager::setGravity( Real x, Real y, Real z )
    {
        mWorld->setGravity( x*100,y*100,z*100 );
    }

    Vector3 PhysicsManager::getGravity()
    {
        dVector3 vec;
        memset(vec, 0, sizeof(dMatrix3));;
        mWorld->getGravity(vec);
        return Vector3( vec[0], vec[1], vec[2] );
    }

    dWorld* PhysicsManager::getWorld()
    {
        return mWorld;
    }

    PhysicalThing* PhysicsManager::createPhysicalThing(Actor* actor)
    {
        PhysicalThing* phys = new PhysicalThing(mCurrSpace, actor);
        mPhysicalThings.insert( PhysicalThingActorPair(actor, phys) );
        return phys;
    }

    PhysicalThing* PhysicsManager::createSpherePhysicalThing(Actor* actor, Real density, bool noDynamics)
    {
        PhysicalThing* phys = new PhysicalThing(mCurrSpace, actor);
        mPhysicalThings.insert( PhysicalThingActorPair(actor, phys) );

        Real radius = 1;

        MovableObject* obj = actor->getSceneNode()->getAttachedObject( 0 );

        CoreSubsystem::log( String("PhysicsManager - Erstelle ein Kugel-Physik-Objekt für den Aktor ") + actor->getName() );

        // Try to create the sphere from the Actor
        if( obj != 0 )
        {
            CoreSubsystem::log( " Eingebettete Entity gefunden" );

            const AxisAlignedBox &aab = obj->getBoundingBox();

            // Adjust the size a bit since it's really intended for culling stuff
            Vector3 x = aab.getMaximum() - Vector3::UNIT_SCALE;
            Vector3 n = aab.getMinimum() + Vector3::UNIT_SCALE;
            // Calculate Scaling
            Vector3 s = actor->getSceneNode()->getScale();

            // Calculating the size of the sphere
            radius = max((x.x - n.x)*s.x,max((x.y - n.y)*s.y,(x.z - n.z)*s.z)) * 0.5;
            CoreSubsystem::log( String(" Radius der Kugel - ") + StringConverter::toString(radius) );
        }
        else
            CoreSubsystem::log( " Keine eingebettete Entity gefunden - verwende Radius 1" );

        phys->createSphereGeometry( radius, actor->getPosition(), actor->getOrientation());

        if( ! noDynamics )
            phys->createSphereMass( density, radius, actor->getPosition(), actor->getOrientation());

        return phys;
    }

    PhysicalThing* PhysicsManager::createBoxPhysicalThing(Actor* actor, Real density, bool noDynamics)
    {
        PhysicalThing* phys = new PhysicalThing(mCurrSpace, actor);
        mPhysicalThings.insert( PhysicalThingActorPair(actor, phys) );

        Vector3 length = Vector3(1,1,1);

        MovableObject* obj = actor->getSceneNode()->getAttachedObject( 0 );

        CoreSubsystem::log( String("PhysicsManager - Erstelle ein Würfel-Physik-Objekt für den Aktor ") + actor->getName() );

        // Try to create the Box from the Actor
        if( obj != 0 )
        {
            CoreSubsystem::log( " Eingebettete Entity gefunden" );

            const AxisAlignedBox &aab = obj->getBoundingBox();

            // Adjust the size a bit since it's really intended for culling stuff
            Vector3 x = aab.getMaximum() - Vector3::UNIT_SCALE;
            Vector3 n = aab.getMinimum() + Vector3::UNIT_SCALE;
            // Calculate Scaling
            Vector3 s = actor->getSceneNode()->getScale();

            // Calculating the size of the box
            length[0] = (x.x - n.x) * s.x;
            length[1] = (x.y - n.y) * s.y;
            length[2] = (x.z - n.z) * s.z;

            CoreSubsystem::log( String(" Größe der Box - ") + 
                StringConverter::toString(length[0]) + " " + 
                StringConverter::toString(length[1]) + " " + 
                StringConverter::toString(length[2]) );
        }
        else
            CoreSubsystem::log( " Keine eingebettete Entity gefunden - verwende Größe 1" );

        phys->createBoxGeometry( length, actor->getPosition(), actor->getOrientation());

        if( ! noDynamics )
            phys->createBoxMass( density, length, actor->getPosition(), actor->getOrientation());

        return phys;
    }

    PhysicalThing* PhysicsManager::createCappedCylinderPhysicalThing(Actor* actor, Real density, bool noDynamics)
    {
        PhysicalThing* phys = new PhysicalThing(mCurrSpace, actor);
        mPhysicalThings.insert( PhysicalThingActorPair(actor, phys) );

        Real radius = 1;
        Real length = 1;

        MovableObject* obj = actor->getSceneNode()->getAttachedObject( 0 );

        // Try to create the sphere from the Actor
        if( obj != 0 )
        {
            const AxisAlignedBox &aab = obj->getBoundingBox();

            // Adjust the size a bit since it's really intended for culling stuff
            Vector3 x = aab.getMaximum() - Vector3::UNIT_SCALE;
            Vector3 n = aab.getMinimum() + Vector3::UNIT_SCALE;

            // Cylinders are z-aligned
            length = x.z - n.z;

            // Calculating the radsius of the cylinder
            radius = max((x.x - n.x),(x.y - n.y)) * 0.5;
        }

        phys->createCappedCylinderGeometry( radius, length, actor->getPosition(), actor->getOrientation() );

        if( ! noDynamics )
            phys->createCappedCylinderMass( density, radius, length, actor->getPosition(), actor->getOrientation());

        return phys;
    }

    void PhysicsManager::removePhysicalThing(Actor* actor)
    {
        PhysicalThingActorMap::iterator pPhysicalThingIter = mPhysicalThings.find(actor);

        if( pPhysicalThingIter != mPhysicalThings.end() )
        {
            PhysicalThing* phys = pPhysicalThingIter->second;
            mPhysicalThings.erase(pPhysicalThingIter);
            delete phys;
        }
    }

    void PhysicsManager::activateGlobalSpace()
    {
        mCurrSpace = mGlobalSpace;
    }

    void PhysicsManager::activatePhysicalThingSpace( PhysicalThing* thing )
    {
        mCurrSpace = thing->getSpace();
    }

    void PhysicsManager::moveToCurrentSpace( PhysicalThing* thing )
    {
        thing->setSpace(mCurrSpace);
    }

    void PhysicsManager::removePhysicalThingSpace( PhysicalThing* thing )
    {
        dSpace* s = thing->getSpace();

        if( mCurrSpace = s )
            activateGlobalSpace();

        if( s != mGlobalSpace && s->getNumGeoms() == 0 )
        {
            mSimpleSpaces.remove((dSimpleSpace*)s);
        }
    }

    // Objects which are grouped in the same SimpleSpace don't collide
    void PhysicsManager::createSimpleSpace()
    {
        // Ist unser aktueller Space nicht leer
        if( (mCurrSpace != mGlobalSpace && mCurrSpace->getNumGeoms() > 0 ) || (mCurrSpace == mGlobalSpace) )
        {
            // Ist der zuletzt erschaffene vielleicht leer
            // (Passiert wenn neu erschaffen, und dann direkt den globalen reaktiviert)
            if( mSimpleSpaces.back()->getNumGeoms() == 0)
            {
                mCurrSpace = mSimpleSpaces.back();
            }
            // Dann müssen wir wohl nen neuen erstellen
            else
            {
                dSimpleSpace *s = new dSimpleSpace(mGlobalSpace->id());
                mSimpleSpaces.push_back(s);
                mCurrSpace = s;
            }
        }

    }

    dSpace* PhysicsManager::getCurrSpace()
    {
        return mCurrSpace;
    }

    PhysicsManager& PhysicsManager::getSingleton(void)
    {
        return Singleton<PhysicsManager>::getSingleton();
    }

    PhysicsManager* PhysicsManager::getSingletonPtr(void)
    {
        return Singleton<PhysicsManager>::getSingletonPtr();
    }

    void PhysicsManager::collisionCallback( void* data, dGeomID o1, dGeomID o2)
    {
        PhysicsManager* pm = (PhysicsManager*)data;

        if( o1 && o2 )
        {
            PhysicalThing* thing1 = (PhysicalThing*)dGeomGetData( o1 );
            PhysicalThing* thing2 = (PhysicalThing*)dGeomGetData( o2 );

            if( thing1 && thing2 )
            {
                thing1->testCollide( thing2 );
            }
        }
    }

    void PhysicsManager::setEnabled(bool enabled)
    {
        m_Enabled = enabled;
    }

}


