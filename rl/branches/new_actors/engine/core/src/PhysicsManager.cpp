/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

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

using namespace OgreOde;

namespace rl
{

    PhysicsManager::PhysicsManager(rl::World* world)
        :   mEnabled(true),
            mPhysicalThings(),
            mSimpleSpaces(),
            mOdeWorld(new OgreOde::World(world->getSceneManager())),
            mGlobalSpace(new OgreOde::HashTableSpace(0)),
            mCurrSpace(mGlobalSpace),
            mOdeStepper(new OgreOde::ForwardFixedQuickStepper(0.01))
    {
        mOdeWorld->setGravity(Vector3(0, -980.665 * 2.56, 0));
        mOdeWorld->setCFM(10e-5);
        mOdeWorld->setERP(0.8);
        mOdeWorld->setAutoSleep(true);
        mOdeWorld->setContactCorrectionVelocity(10.0);

	    mOdeStepper->setAutomatic(OgreOde::Stepper::AutoMode_NotAutomatic,
	        Root::getSingletonPtr());
    }

    PhysicsManager::~PhysicsManager()
    {
        delete mGlobalSpace;
        mGlobalSpace = 0;
        delete mOdeWorld;
        mOdeWorld = 0;
    }

    void PhysicsManager::run(Real elapsedTime)
    {
        if( mEnabled && elapsedTime > 0.0f )
        {
        }
    }

    OgreOde::JointGroup*  PhysicsManager::getContactJointGroup()
    {
        ///!todo implementieren
        return 0;
    }

    ///!todo Das aktuelle Levelmesh setzen.
    /// bzw. Flags benutzen, die immvoables auszeichnen.
    /// irgendeine Loesung dazu einfallen lassen.
    void PhysicsManager::setWorldScene( World* world )
    {
        if (!world)
        {
            setEnabled(false);
        }
    }

    void PhysicsManager::setCFM(Real cfm)
    {
        mOdeWorld->setCFM(cfm);
    }

    Real PhysicsManager::getCFM()
    {
        return mOdeWorld->getCFM();
    }

    void PhysicsManager::setERP(Real erp)
    {
        mOdeWorld->setERP(erp);
    }

    Real PhysicsManager::getERP()
    {
        return mOdeWorld->getERP();
    }

    void PhysicsManager::setGravity( Real x, Real y, Real z )
    {
        mOdeWorld->setGravity(Vector3(x, y, z));
    }

    Vector3 PhysicsManager::getGravity()
    {
        return mOdeWorld->getGravity();
    }

    OgreOde::World* PhysicsManager::getWorld()
    {
        return mOdeWorld;
    }

    /*PhysicalThing* PhysicsManager::createSpherePhysicalThing(Real Radius,
        Real density)
    {
        PhysicalThing* phys = new PhysicalThing(mCurrSpace, actor);
        mPhysicalThings.insert( PhysicalThingActorPair(actor, phys) );

        Real radius = 1;

        MovableObject* obj = actor->_getSceneNode()->getAttachedObject( 0 );

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
            Vector3 s = actor->_getSceneNode()->getScale();

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
    }*/

    PhysicalThing* PhysicsManager::createBoxPhysicalThing(const Vector3& size,
        Real density)
    {
    /*
        PhysicalThing* phys = new PhysicalThing(mCurrSpace, actor);
        mPhysicalThings.insert( PhysicalThingActorPair(actor, phys) );

        Vector3 length = Vector3(1,1,1);

        MovableObject* obj = actor->_getSceneNode()->getAttachedObject( 0 );

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
            Vector3 s = actor->_getSceneNode()->getScale();

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
    */
        return 0;
    }

    /*PhysicalThing* PhysicsManager::createCapsulePhysicalThing(Real height,
        Real radius, Real density)
    {
        PhysicalThing* phys = new PhysicalThing(mCurrSpace, actor);
        mPhysicalThings.insert( PhysicalThingActorPair(actor, phys) );

        Real radius = 1;
        Real length = 1;

        MovableObject* obj = actor->_getSceneNode()->getAttachedObject( 0 );

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
    }*/

    void PhysicsManager::removeAndDestroyPhysicalThing(PhysicalThing* thing)
    {
        PhysicalThingSet::iterator it = mPhysicalThings.find(thing);

        if( it != mPhysicalThings.end() )
        {
            PhysicalThing* phys = *it;
            mPhysicalThings.erase(it);
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
        OgreOde::Space* s = thing->getSpace();

        if( mCurrSpace == s )
            activateGlobalSpace();

        if( s != mGlobalSpace && s->getGeometryCount() == 0 )
        {
            mSimpleSpaces.remove(dynamic_cast<OgreOde::SimpleSpace*>(s));
        }
    }

    // Objects which are grouped in the same SimpleSpace don't collide
    void PhysicsManager::createSimpleSpace()
    {
        // Ist unser aktueller Space nicht leer
        if((mCurrSpace != mGlobalSpace && mCurrSpace->getGeometryCount() > 0 ) || (mCurrSpace == mGlobalSpace) )
        {
            // Ist der zuletzt erschaffene vielleicht leer
            // (Passiert wenn neu erschaffen, und dann direkt den globalen reaktiviert)
            if( mSimpleSpaces.back()->getGeometryCount() == 0)
            {
                mCurrSpace = mSimpleSpaces.back();
            }
            // Dann müssen wir wohl nen neuen erstellen
            else
            {
                SimpleSpace *s = new SimpleSpace(mGlobalSpace);
                mSimpleSpaces.push_back(s);
                mCurrSpace = s;
            }
        }
    }

    OgreOde::Space* PhysicsManager::getCurrSpace()
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

    bool PhysicsManager::collision(Contact* contact)
    {
    /*
        PhysicalThing* thing1 = (PhysicalThing*)
            contact->getFirstGeometry()->getUserData();
        PhysicalThing* thing2 = (PhysicalThing*)
            contact->getSecondGeometry()->getUserData();

        if( thing1 && thing2 )
        {
            thing1->testCollide( thing2 );
        }
        ///@todo richten!
        return false;
    */
        return false;
    }

    void PhysicsManager::setEnabled(bool enabled)
    {
        mEnabled = enabled;
    }

}
