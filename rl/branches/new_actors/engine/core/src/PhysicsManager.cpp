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
        mOdeWorld->setGravity(Vector3(0, -980.665, 0));
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
        ///@todo implementieren
        return 0;
    }

    ///@todo Das aktuelle Levelmesh setzen.
    void PhysicsManager::setLevelGeometry(SceneNode* levelNode)
    {
        delete mOdeLevel;
        mOdeLevel = 0;
        if (levelNode)
        {
            mOdeLevel = MeshInformer::createStaticTriangleMesh(levelNode, mGlobalSpace);
        }
        else
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

    PhysicalThing* PhysicsManager::createPhysicalThing(const int geomType,
        const Ogre::Vector3& size, Real density)
    {
        Geometry* geom = 0;
        if (geomType == PT_BOX)
        {
            geom = createBoxGeometry(size, density);
        }
        else if (geomType == PT_SPHERE)
        {
            ///@todo nicht einfach x als Radius nehmen, sondern die maximale Ausdehnung.
            geom = createSphereGeometry(size.x / 2.0, density);
        }
        else if (geomType == PT_CAPSULE)
        {
            geom = createCapsuleGeometry(size.y, size.x / 2.0, density);
        }
        PhysicalThing* pt = new PhysicalThing(geom, mCurrSpace);
        mPhysicalThings.insert(pt);        
        return pt;
    }

    Geometry* PhysicsManager::createSphereGeometry(Real radius,
        Real density)
    {
        Geometry* geom = new SphereGeometry(radius, mCurrSpace);
        if (density > 0.0)
        {
            //Objekt hat eine Masse, also einen Body verpassen.
            OgreOde::Body* body = new OgreOde::Body();
            OgreOde::SphereMass mass(1.0, radius);
            mass.setDensity(density, radius);
            body->setMass(mass);
            geom->setBody(body);
        }
        return geom;
    }

    Geometry* PhysicsManager::createBoxGeometry(const Vector3& size,
        Real density)
    {
        Geometry* geom = new BoxGeometry(size, mCurrSpace);
        if (density > 0.0)
        {
            //Objekt hat eine Masse, also einen Body verpassen.
            OgreOde::Body* body = new OgreOde::Body();
            OgreOde::BoxMass mass(1.0, size);
            mass.setDensity(density, size);
            body->setMass(mass);
            geom->setBody(body);
        }
        return geom;
    }

    Geometry* PhysicsManager::createCapsuleGeometry(Real height,
        Real radius, Real density)
    {
        Geometry* geom = new OgreOde::CapsuleGeometry(radius, height, mCurrSpace);
        if (density > 0.0)
        {
            //Objekt hat eine Masse, also einen Body verpassen.
            OgreOde::Body* body = new OgreOde::Body();
            OgreOde::CapsuleMass mass(1.0, radius, Vector3::UNIT_Y, height);
            mass.setDensity(density, radius, Vector3::UNIT_Y, height);
            body->setMass(mass);
            geom->setBody(body);
        }
        return geom;
    }

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
