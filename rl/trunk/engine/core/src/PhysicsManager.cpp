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
using namespace std;

namespace rl
{

    PhysicsManager::PhysicsManager(rl::World* world)
        :   mEnabled(true),
            mPhysicalThings(),
            mSpaces(),
            mOdeWorld(new OgreOde::World(world->getSceneManager())),
            mGlobalSpace(new OgreOde::HashTableSpace(0)),
            mCurrSpace(mGlobalSpace),
            mOdeStepper(new OgreOde::ForwardFixedQuickStepper(0.01)),
            mOdeLevel(0)
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
    void PhysicsManager::createLevelGeometry(SceneNode* levelNode)
    {
        delete mOdeLevel;
        mOdeLevel = 0;
        if (levelNode)
        {
            mOdeLevel = MeshInformer::createStaticTriangleMesh(levelNode,
                mGlobalSpace);
        }
        else
        {
            setEnabled(false);
        }        
    }
    
    Geometry* PhysicsManager::getLevelGeometry()
    {
        return mOdeLevel;
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
        ///@todo verallgemeinern
        Vector3 offset(Vector3::ZERO);
        if (geomType == PT_BOX)
        {
            geom = createBoxGeometry(size, density);
        }
        else if (geomType == PT_SPHERE)
        {
            double radius = max(size.x, max(size.y, size.z)) / 2.0;
            geom = createSphereGeometry(radius, density);
        }
        else if (geomType == PT_CAPSULE)
        {
            double radius = max(size.x, size.z) / 2.0;
            geom = createCapsuleGeometry(size.y - 2.0 * radius, radius, density);
            offset = Vector3(0.0, (size.y - 2.0 * radius) / 2.0 + radius, 0.0);
        }
        PhysicalThing* pt = new PhysicalThing(geom, offset);
        mPhysicalThings.push_back(pt);        
        return pt;
    }

    Geometry* PhysicsManager::createSphereGeometry(Real radius,
        Real density)
    {
        Geometry* geom = new SphereGeometry(radius, mCurrSpace);
        if (density > 0.0)
        {
            // Objekt hat eine Masse, also einen Body verpassen.
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
            // Objekt hat eine Masse, also einen Body verpassen.
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
        Geometry* geom = new CapsuleGeometry(radius, height, mCurrSpace);
        ///@todo verallgemeinern.
        geom->setOrientation(Quaternion(Degree(90), Vector3::UNIT_X));
        if (density > 0.0)
        {
            // Objekt hat eine Masse, also einen Body verpassen.
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
        vector<PhysicalThing*>::iterator it = find(mPhysicalThings.begin(),
            mPhysicalThings.end(), thing);

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
            vector<Space*>::iterator it = find(mSpaces.begin(), mSpaces.end(), s);
            if (it != mSpaces.end())
            {
                mSpaces.erase(it);
            }
        }
    }

    // Objects which are grouped in the same SimpleSpace don't collide
    void PhysicsManager::createSimpleSpace()
    {
        // Ist unser aktueller Space nicht leer
        if((mCurrSpace != mGlobalSpace && mCurrSpace->getGeometryCount() > 0)
            || (mCurrSpace == mGlobalSpace))
        {
            // Ist der zuletzt erschaffene vielleicht leer
            // (Passiert wenn neu erschaffen,
            //  und dann direkt den globalen reaktiviert)
            if( mSpaces.back()->getGeometryCount() == 0)
            {
                mCurrSpace = mSpaces.back();
            }
            // Dann müssen wir wohl nen neuen erstellen
            else
            {
                SimpleSpace *s = new SimpleSpace(mGlobalSpace);
                mSpaces.push_back(s);
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
        Geometry* g1 = contact->getFirstGeometry();
        Geometry* g2 = contact->getSecondGeometry();

        if(g1 && g2)
        {
            // Check for collisions between things that are connected
            // and ignore them.
            OgreOde::Body* b1 = g1->getBody();
            OgreOde::Body* b2 = g2->getBody();
            if(b1 && b2) if(OgreOde::Joint::areConnected(b1,b2)) return false; 
        }
        
        // Set the friction at the contact
        contact->setCoulombFriction(OgreOde::Utility::Infinity);

        // Yes, this collision is valid
        bool rval = true;
        for(vector<CollisionListener*>::size_type i = 0;
            i < mCollisionListeners.size(); i++)
        {
            rval = rval && mCollisionListeners[i]->collision(contact);
        }
        return rval;
    }

    bool PhysicsManager::preStep(Real time)
    {
        return true;
    }
    
    void PhysicsManager::setEnabled(bool enabled)
    {
        mEnabled = enabled;
    }

    void PhysicsManager::addCollisionListener(CollisionListener* cl)
    {
        mCollisionListeners.push_back(cl);
    }
    
    void PhysicsManager::removeCollisionListener(CollisionListener* cl)
    {
        vector<CollisionListener*>::iterator it =
            find(mCollisionListeners.begin(), mCollisionListeners.end(), cl);
        
        if (it != mCollisionListeners.end())
        {
            mCollisionListeners.erase(it);
        }
    }
}
