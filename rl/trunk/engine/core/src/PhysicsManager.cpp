/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
#include "ActorManager.h"

#include "Exception.h"

// Define a max macro
#ifndef max
#define max(a,b) (((a)>=(b))?(a):(b))
#endif

using namespace OgreOde;
using namespace std;

template<> rl::PhysicsManager* Singleton<rl::PhysicsManager>::ms_Singleton = 0;

namespace rl
{
    PhysicsManager& PhysicsManager::getSingleton(void)
    {
        return Singleton<PhysicsManager>::getSingleton();
    }

    PhysicsManager* PhysicsManager::getSingletonPtr(void)
    {
        return Singleton<PhysicsManager>::getSingletonPtr();
    }

    PhysicsManager::PhysicsManager( )
        :   mEnabled(false),
            mPhysicalThings(),
            mCollisionListeners(),
			mOdeWorld(new OgreOde::World( 
				CoreSubsystem::getSingleton().getWorld()->getSceneManager() ) ),
            mGlobalSpace(mOdeWorld->getDefaultSpace()),
			mLevelGeomSpace(mGlobalSpace),
            mOdeStepper(new OgreOde::ForwardFixedQuickStepper(0.05)),
			mFallSpeed(0.1),
			mCameraNode(NULL),
			mOdeCamera(NULL),
			mControlNode(NULL),
			mOdeActor(NULL)
    {
        mOdeWorld->setGravity(Vector3(0, -98.0665, 0));
        mOdeWorld->setCFM(10e-5);
        mOdeWorld->setERP(0.2);
        mOdeWorld->setAutoSleep(true);
        mOdeWorld->setContactCorrectionVelocity(1.0);
		mOdeWorld->setContactSurfaceLayer(0.00001);
        mOdeWorld->setCollisionListener(this);
		mOdeWorld->setDamping(0.2, 0.0); // linear, angular
	
		mLevelGeomSpace->setInternalCollisions( false );

		mOdeStepper->setAutomatic(OgreOde::ForwardFixedQuickStepper::AutoMode_NotAutomatic,
	        Root::getSingletonPtr());
    }

    PhysicsManager::~PhysicsManager()
    {
		delete mLevelGeomSpace;
		mLevelGeomSpace = 0;
        delete mGlobalSpace;
        mGlobalSpace = 0;
        delete mOdeWorld;
        mOdeWorld = 0;
    }

    void PhysicsManager::run(Real elapsedTime)
    {
        if( mEnabled && elapsedTime > 0.0f )
        {
            mOdeStepper->step(elapsedTime);
			mLevelGeomSpace->collide(mOdeActor);

			//mOdeActor->collide(mOdeLevel, this);
			//mOdeCamera->collide(mOdeLevel, this);
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


	/*PhysicalThing* PhysicsManager::createOrientedPhysicalThing(
		Actor* actor, const int geomType,
		Real density, OgreOde::Space* odeSpace, 
		OffsetMode offsetMode)
	{

	}*/


    PhysicalThing* PhysicsManager::createPhysicalThing(const int geomType,
        const Vector3& size, Real density, OgreOde::Space* odeSpace, OffsetMode offsetMode)
    {
        PhysicalThing* rval = NULL;
        
        if (geomType != GT_NONE) {
            Geometry* geom = NULL;
            Body* body = density > 0.0 ? new Body() : NULL;
			
			//Objekte mit Body werden in TransformGeometry gekapselt und die dürfen
			//nicht in einem Space liegen
			OgreOde::Space* space = body != NULL ? NULL : odeSpace;
			
            
            ///@todo verallgemeinern
            Vector3 offset(Vector3::ZERO);
            Quaternion orientationBias(Quaternion::IDENTITY);
            
            if (geomType == GT_BOX)
            {
                geom = new BoxGeometry(size, space);

                if (offsetMode == OM_BOTTOMCENTERED)
                {
                    offset = Vector3(0.0, size.y / 2.0, 0.0);
                }

                if (density > 0.0)
                {
                    // Objekt hat eine Masse, also einen Body verpassen.
                    OgreOde::BoxMass mass(1.0, size);
                    mass.setDensity(density, size);
                    body->setMass(mass);
                }
            }
            else if (geomType == GT_SPHERE)
            {
                double radius = max(size.x, max(size.y, size.z)) / 2.0;

                if (offsetMode == OM_BOTTOMCENTERED)
                {
                    offset = Vector3(0.0, size.y / 2.0, 0.0);
                }
                
                geom = new SphereGeometry(radius, space);

                if (density > 0.0)
                {
                    // Objekt hat eine Masse, also einen Body verpassen.
                    OgreOde::SphereMass mass(1.0, radius);
                    mass.setDensity(density, radius);
                    body->setMass(mass);
                }
            }
            else if (geomType == GT_CAPSULE)
            {
                double radius = max(size.x, size.z) / 2.0;
                double height = size.y - 2.0 * radius;

                if (offsetMode == OM_BOTTOMCENTERED)
                {
                    offset = Vector3(0.0, (size.y - 2.0 * radius) / 2.0 + radius, 0.0);
                }

                geom = new CapsuleGeometry(radius, height, space);

                orientationBias = Quaternion(Degree(90), Vector3::UNIT_X); //UNIT_X
                if (density > 0.0)
                {
                    // Objekt hat eine Masse, also einen Body verpassen.
                    OgreOde::Body* body = new OgreOde::Body();
                    OgreOde::CapsuleMass mass(1.0, radius, Vector3::UNIT_Y, height);//Y
                    mass.setDensity(density, radius, Vector3::UNIT_Y, height);//Y
                    body->setMass(mass);
                }
            }

            // Falls es ein Körper ist, können wir die Ursprungsänderung
            // nicht per offset regeln, sondern nehmen eine TransformGeometry
            if (body)
            {
                TransformGeometry* trans = new TransformGeometry(mGlobalSpace);
                geom->setPosition(offset);
                geom->setOrientation(orientationBias);
                trans->setEncapsulatedGeometry(geom);
                
                geom = trans;
                geom->setBody(body);
               
                // Offset und bias werden schon von der TransformGeom geregelt
                // deshalb diese zurücksetzen.
                offset = Vector3::ZERO;
                orientationBias = Quaternion::IDENTITY;
            }

            rval = new PhysicalThing(geom, offset, orientationBias);
            mPhysicalThings.push_back(rval);        
        }
        return rval;
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

    bool PhysicsManager::collision(Contact* contact)
    {
        Geometry* g1 = contact->getFirstGeometry();
        Geometry* g2 = contact->getSecondGeometry();
                
        if(g1 && g2)
        {
			if (g1 == mOdeActor)
				return collisionWithPlayerActor(g2, contact);
			if (g2 == mOdeActor)
				return collisionWithPlayerActor(g1, contact);
			if (g1 == mOdeCamera || g2 == mOdeCamera)
				return collisionCameraWithLevel(contact);

            // Check for collisions between things that are connected
            // and ignore them.
            OgreOde::Body* b1 = g1->getBody();
            OgreOde::Body* b2 = g2->getBody();
            if(b1 && b2)
            {
                if(OgreOde::Joint::areConnected(b1,b2)) return false; 
				
				PhysicalThing* pt1 = reinterpret_cast<PhysicalThing*>(b1->getUserData());
                PhysicalThing* pt2 = reinterpret_cast<PhysicalThing*>(b2->getUserData());
                contact->setCoulombFriction(pt1->getFriction() * pt2->getFriction());
                contact->setBouncyness(
                    pt1->getBounceRestitutionValue() * pt2->getBounceRestitutionValue(),
                    max(pt1->getBounceVelocityThreshold(),
                        pt2->getBounceVelocityThreshold())); 
                contact->setSoftness(max(pt1->getSoftErp(), pt2->getSoftErp()),
                    pt1->getSoftness() * pt2->getSoftness());
            }
            else if (b1 || b2)
            {
                PhysicalThing* pt = reinterpret_cast<PhysicalThing*>(b1 ? 
                    b1->getUserData() : b2->getUserData());
                if (pt)
                {
                    contact->setCoulombFriction(pt->getFriction());
                    contact->setBouncyness(pt->getBounceRestitutionValue(),
                        pt->getBounceVelocityThreshold()); 
                    contact->setSoftness(pt->getSoftErp(), pt->getSoftness());
                }
            }
        }

        // Yes, this collision is valid
        return true;
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

	void PhysicsManager::toggleDebugOde()
	{
		mOdeWorld->setShowDebugObjects(!mOdeWorld->getShowDebugObjects());
	}

	bool PhysicsManager::collisionWithPlayerActor(OgreOde::Geometry* geometry, Contact* contact)
	{
		if (geometry != mOdeCamera)
		{
			Ogre::Vector3 correction = contact->getNormal() * contact->getPenetrationDepth();
			mOdeActor->setPosition(mOdeActor->getPosition() + correction);
			mFallSpeed = 0.0;
		}
		else
		{
			mFallSpeed = 0.1;
		}
		return true;
	}

	bool PhysicsManager::collisionCameraWithLevel(Contact* contact)
	{
		mCameraNode->translate(
			Vector3(0.0, 0.0, -contact->getPenetrationDepth()),
			Node::TS_LOCAL);
		mCameraNode->_update(true, false);
		//mTargetDistance = mCameraNode->getPosition().z;
		return true;
	}

	void PhysicsManager::addLevelGeometry( Ogre::Entity* ent )
	{
		if( ent == NULL )
			Throw( NullPointerException, "Übergebene Entity darf nicht NULL sein" );

		Matrix4 m = Matrix4::IDENTITY;

		// Wenn die Entity an einem Node befestigt ist, Transformation übernehmen
		if( ent->getParentNode() != NULL )
			m = ent->getParentNode()->_getFullTransform();

		// Ein Ode-TriMesh erstellen
		OgreOde::EntityInformer ei( ent, m );
		Geometry* odeGeom = ei.createStaticTriangleMesh(mLevelGeomSpace);
	}

	void PhysicsManager::clearLevelGeometry(  )
	{
		for( int i=(mLevelGeomSpace->getGeometryCount()-1); i >=0 ; i-- )
		{
			Geometry* odeGeom = mLevelGeomSpace->getGeometry(i);
			mLevelGeomSpace->removeGeometry(*odeGeom);
			delete odeGeom;
		}
	}


	void PhysicsManager::setFallSpeed(Ogre::Real fallspeed)
	{
		mFallSpeed = fallspeed;
	}

	Ogre::Real PhysicsManager::getFallSpeed()
	{
		return mFallSpeed;
	}

	void PhysicsManager::setActor(Geometry* actor, SceneNode* controlNode)
	{
		mOdeActor = actor; 
		mControlNode = controlNode;
	}

	void PhysicsManager::setCamera(Geometry* camera, SceneNode* cameraNode)
	{
		mOdeCamera = camera;
		mCameraNode = cameraNode;
	}
}
