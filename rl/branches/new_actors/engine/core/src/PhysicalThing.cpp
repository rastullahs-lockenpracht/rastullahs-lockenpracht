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

#include "PhysicalThing.h"

#include "OgreNoMemoryMacros.h"
#include <ode/ode.h>
#include "OgreMemoryMacros.h"
#include "OgreLogManager.h"

#include "Actor.h"
#include "PhysicsManager.h"
#include "Exception.h"

using namespace OgreOde;

namespace rl
{
    PhysicalThing::PhysicalThing(Space* space)
        :   mBounceRestitution(0.0f),
            mBounceVelocityThresh(0.0f),
            mSoftness(0.0f),
            mFriction(0.0f),
            mSoftErp(0.0f),
            mGeometry(0),
            mBody(0),
            mSpace(space),
            mActor(0)    
    {
    }

    PhysicalThing::~PhysicalThing(void)
    {
        delete mGeometry;
        mGeometry = 0;
        delete mBody;
        mBody = 0;
    }

    void PhysicalThing::setPosition( Real x, Real y, Real z )
    {
        if( mBody )
            mBody->setPosition(Vector3(x,y,z));

        if( mGeometry )
            mGeometry->setPosition(Vector3(x,y,z));  
    }

    void PhysicalThing::setOrientation( Real w, Real x, Real y, Real z )
    {
        if( mBody )
        {
            mBody->setOrientation(Quaternion(w, x, y, z));
        }

        if( mGeometry )
        {
            // Kein setQuaternion, na danke...
            mGeometry->setOrientation(Quaternion(w,x,y,z)); 
        }
    }

    void PhysicalThing::setSpace(Space* space )
    {
        if( mGeometry )
        {
            mSpace->removeGeometry(*mGeometry);
            space->addGeometry(*mGeometry);
        }
        mSpace = space;
    }

    bool PhysicalThing::isDynamic( void )
    {
        return mBody && mBody->isAwake();
    }

    void PhysicalThing::setDynamic(bool dynamic)
    {
        if( mBody )
        {
            if(dynamic)
                mBody->wake();
            else
                mBody->sleep();
        }
    }

    void PhysicalThing::stopDynamics()
    {
        if( mBody )
        {
            mBody->setAngularVelocity(Vector3(0,0,0));
            mBody->setLinearVelocity(Vector3(0,0,0));
            mBody->addForce(-mBody->getForce());
            mBody->sleep();
        }
    }

    //void PhysicalThing::createCappedCylinderMass( Real density, Real radius, Real length, const Vector3& position, const Quaternion& orientation )
    //{
    //    delete mBody;

    //    mBody = new Body();
    //    mBody->setPosition(position); 
    //    mBody->setOrientation(orientation);
    //    mBody->setUserData(reinterpret_cast<unsigned long>(this));
    //    mBody->setMass(CapsuleMass(density, radius, Vector3::ZERO, length));

    //    setDynamic(true);

    //    if (mGeometry)
    //        mGeometry->setBody(mBody);
    //}

    //void PhysicalThing::createBoxMass( Real density, const Vector3& length, const Vector3& position, const Quaternion& orientation)
    //{
    //    delete mBody;

    //    mBody = new Body();
    //    mBody->setPosition(position); 
    //    mBody->setOrientation(orientation);
    //    mBody->setUserData(reinterpret_cast<unsigned long>(this));
    //    mBody->setMass(BoxMass(density, length));

    //    setDynamic(true);

    //    if (mGeometry)
    //        mGeometry->setBody(mBody);
    //}

    //void PhysicalThing::createSphereMass( Real density, Real radius, const Vector3& position, const Quaternion& orientation )
    //{
    //    delete mBody;

    //    mBody = new Body();
    //    mBody->setPosition(position); 
    //    mBody->setOrientation(orientation);
    //    mBody->setUserData(reinterpret_cast<unsigned long>(this));
    //    mBody->setMass(SphereMass(density, radius));

    //    setDynamic( true );

    //    if (mGeometry)
    //        mGeometry->setBody(mBody);
    //}

    //void PhysicalThing::createCappedCylinderGeometry( Real radius, Real length, const Vector3& position, const Quaternion& orientation)
    //{
    //    delete mGeometry;
    //    mGeometry = new CapsuleGeometry(radius, length, mSpace);
    //    mGeometry->setUserData(reinterpret_cast<unsigned long>(this));
    //    mGeometry->setPosition(position);
    //    mGeometry->setOrientation(orientation);

    //    if( mBody )
    //        mGeometry->setBody(mBody);
    //}

    ///*
    //Set the geom to a box
    //*/
    //void PhysicalThing::createBoxGeometry( const Vector3& length, const Vector3& position, const Quaternion& orientation )
    //{
    //    delete mGeometry;
    //    mGeometry = new BoxGeometry(length, mSpace);
    //    mGeometry->setUserData(reinterpret_cast<unsigned long>(this));
    //    mGeometry->setPosition(position);
    //    mGeometry->setOrientation(orientation);

    //    if( mBody )
    //        mGeometry->setBody(mBody);
    //}

    ///*
    //Set the geom to a sphere
    //*/
    //void PhysicalThing::createSphereGeometry( Real radius, const Vector3& position, const Quaternion& orientation )
    //{
    //    delete mGeometry;
    //    mGeometry = new SphereGeometry(radius, mSpace);
    //    mGeometry->setUserData(reinterpret_cast<unsigned long>(this));
    //    mGeometry->setPosition(position);
    //    mGeometry->setOrientation(orientation);

    //    if( mBody )
    //        mGeometry->setBody(mBody);
    //}

    /*
    Return the scene node we're simulating
    */
    Actor *PhysicalThing::getActor(void)
    {
        return mActor;
    }

    /*
    Return the body we're using for simulation
    */
    Body* PhysicalThing::getBody(void)
    {
        return mBody;
    }

    Geometry* PhysicalThing::getGeometry( void )
    {
        return mGeometry;
    }

    /*
    Return the space that we were created in
    */
    Space* PhysicalThing::getSpace(void)
    {
        return mSpace;
    }

    /*
    Update the Actor we're being physical for
    */
    void PhysicalThing::update(void)
    {
        // If it's dynamic and associated with an Actor
        if( mActor && mBody )
        {
            // Set the Actor according to the ODE data
            mActor->setPosition(mBody->getPosition());
            //LogManager::getSingleton().logMessage( String("Position x:")  << pos[0] << " y:" << pos[1] << " z:" << pos[2] << "\n" );

            mActor->setOrientation(mBody->getOrientation());
        }
    }

    bool PhysicalThing::testCollide(PhysicalThing* other)
    {
        bool collided = false;
/*
        if ( other != 0 )
        {
            const int N = 10;
            dContact contact[N];
            int numColls = dCollide( mGeom->id(), other->getGeomID() , N, &contact[0].geom, sizeof(dContact));

            if (numColls)
            {
                if( this->isDynamic() || other->isDynamic() )
                {
                    for( int i = 0; i < numColls; i++ )
                    {
                        // Create contact joints if object is dynamics simulated
                        contact[i].surface.mode = dContactBounce | dContactSoftERP | dContactApprox1;
                        contact[i].surface.bounce = mBounceRestitution;
                        contact[i].surface.bounce_vel = mBounceVelocityThresh;
                        contact[i].surface.soft_erp = mSoftErp;

                        if (mSoftness > 0)
                        {
                            contact[i].surface.mode |= dContactSoftCFM;
                            contact[i].surface.soft_cfm = mSoftness;
                        }

                        contact[i].surface.mu = mFriction;
                        contact[i].surface.mu2 = 0;

                        // Kollision, nur dynamische Objekte ändern ihre Position
                        if( this->isDynamic() && other->isDynamic() )
                            contactJoint.attach(mBody->id(), other->getBodyID());
                        else if ( other->isDynamic() )
                            contactJoint.attach(other->getBodyID(), 0 );
                        else if ( this->isDynamic() )
                            contactJoint.attach(mBody->id(), 0 );
                    }
                }
                collided = true;
            }
        }
*/
        return collided;
    }

    void PhysicalThing::addForce(const Vector3& direction)
    {
        addForce(direction.x, direction.y, direction.z);
    }

    void PhysicalThing::addForce(Real dir_x, Real dir_y, Real dir_z)
    {
        if(mBody)
        {
            mBody->wake();
            mBody->addRelativeForce(Vector3(dir_x, dir_y, dir_z));
        }
    }

    void PhysicalThing::addForceWorldSpace(const Vector3& direction)
    {
        addForceWorldSpace(direction.x, direction.y, direction.z );
    }

    void PhysicalThing::addForceWorldSpace(Real dir_x, Real dir_y, Real dir_z)
    {
        if(mBody)
        {
            mBody->wake();
            mBody->addForce(Vector3(dir_x, dir_y, dir_z));
        }
    }

    void PhysicalThing::addForce(const Vector3& direction, const Vector3& atPosition)
    {
        addForce(direction.x, direction.y, direction.z,atPosition.x, atPosition.y, atPosition.z);
    }

    void PhysicalThing::addForce(Real dir_x, Real dir_y, Real dir_z,Real pos_x, Real pos_y, Real pos_z)
    {
        if(mBody)
        {
            mBody->wake();
            mBody->addRelativeForceAtRelative(Vector3(dir_x, dir_y, dir_z),
                Vector3(pos_x, pos_y, pos_z));
        }
    }

    void PhysicalThing::addForceWorldSpace(const Vector3& direction,
        const Vector3& atPosition)
    {
        addForceWorldSpace(direction.x, direction.y, direction.z,
            atPosition.x, atPosition.y, atPosition.z);
    }

    void PhysicalThing::addForceWorldSpace(Real dir_x, Real dir_y, Real dir_z,
        Real pos_x, Real pos_y, Real pos_z)
    {
        if(mBody)
        {
            mBody->wake();
            mBody->addForceAt(Vector3(dir_x, dir_y, dir_z),
                Vector3(pos_x, pos_y, pos_z));
        }
    }

    void PhysicalThing::addTorque(const Vector3& direction)
    {
        addTorque(direction.x, direction.y, direction.z);
    }

    void PhysicalThing::addTorque(Real x, Real y, Real z)
    {
        if(mBody)
        {
            mBody->wake();
            mBody->addRelativeTorque(Vector3(x, y, z));
        }
    }

    void PhysicalThing::addTorqueWorldSpace(const Vector3& direction)
    {
        addTorqueWorldSpace(direction.x, direction.y, direction.z);
    }

    void PhysicalThing::addTorqueWorldSpace(Real x, Real y, Real z)
    {
        if(mBody)
        {
            mBody->wake();
            mBody->addTorque(Vector3(x, y, z));
        }
    }

    void PhysicalThing::setLinearVelocity(const Vector3& vel)
    {
        setLinearVelocity(vel.x, vel.y, vel.z);
    }

    void PhysicalThing::setLinearVelocity(Real x, Real y, Real z)
    {
        if (mBody)
        {
            mBody->wake();
            mBody->setLinearVelocity(Vector3(x, y, z));
        }
    }

    void PhysicalThing::setAngularVelocity(const Vector3& vel)
    {
        setAngularVelocity(vel.x, vel.y, vel.z);
    }

    void PhysicalThing::setAngularVelocity(Real x, Real y, Real z)
    {
        if (mBody)
        {
            mBody->wake();
            mBody->setAngularVelocity(Vector3(x, y, z));
        }
    }

    const Vector3 PhysicalThing::getLinearVelocity(void)
    {
        Vector3 rval(Vector3::ZERO);
        if( mBody )
        {
            rval = mBody->getLinearVelocity();
        }
        return rval;
    }

    const Vector3 PhysicalThing::getAngularVelocity(void)
    {
        Vector3 rval(Vector3::ZERO);
        if( mBody )
        {
            rval = mBody->getAngularVelocity();
        }
        return rval;
    }

    void PhysicalThing::setBounceParameters(Real restitutionValue, Real velocityThreshold)
    {
        mBounceRestitution = restitutionValue;
        mBounceVelocityThresh = velocityThreshold;
    }

    Real PhysicalThing::getBounceRestitutionValue(void)
    {
        return mBounceRestitution;
    }

    Real PhysicalThing::getBounceVelocityThreshold(void)
    {
        return mBounceVelocityThresh;
    }

    void PhysicalThing::setSoftness(Real softness)
    {
        mSoftness = softness;
    }

    Real PhysicalThing::getSoftness(void)
    {
        return mSoftness;
    }

    void PhysicalThing::setFriction(Real friction)
    {
        mFriction = friction;
    }

    Real PhysicalThing::getFriction(void)
    {
        return mFriction;
    }

    void PhysicalThing::setSoftErp(Real erp)
    {
        mSoftErp = erp;
    }

    Real PhysicalThing::getSoftErp(void)
    {
        return mSoftErp;
    }

}

