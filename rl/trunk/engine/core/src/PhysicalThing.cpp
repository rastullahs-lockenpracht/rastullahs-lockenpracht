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

namespace rl
{

    PhysicalThing::PhysicalThing( dSpace* space, Actor* actor )
    {
        RlAssert(space!=0,"Space has to be defined");
        RlAssert(actor!=0,"Actor has to be defined");
        mSpace = space;
        mActor  = actor;

        mFriction = dInfinity;
        mBounceRestitution = 0.1;
        mBounceVelocityThresh = 0.1;
        mSoftness = 0.0;

        mSoftErp = 0.0;
        mActor->setPhysicalThing( this );

        mColliding = false;
        mDynamic = false;

        mGeom = 0;
        mBody  = 0;

    }

    PhysicalThing::~PhysicalThing(void)
    {
        if( mGeom )
        {
            delete mGeom;
            if( mSpace->getNumGeoms() == 0  )
            {
                PhysicsManager::getSingleton().removePhysicalThingSpace( this );
            }
        }

        if( mBody )
            delete mBody;    
    }

    void PhysicalThing::setPosition( Real x, Real y, Real z )
    {
        if( mBody )
        {
            mBody->setPosition(x,y,z);
            dMass mass;
            mBody->getMass( &mass );
            mass.translate(x,y,z);
        }

        if( mGeom )
            mGeom->setPosition(x,y,z);  
    }

    void PhysicalThing::setOrientation( Real w, Real x, Real y, Real z )
    {
        if( mBody )
        {
            dReal dquat[4] = { w, x, y, z };
            mBody->setQuaternion(dquat);
        }

        if( mGeom )
        {
            // Kein setQuaternion, na danke...
            setGeomRotation( Quaternion(w,x,y,z) ); 
        }
    }

    void PhysicalThing::setGeomRotation( const Quaternion& q )
    {
        dReal dquat[4] = { q.w, q.x, q.y, q.z };
        dMatrix3 dm3;
        memset(dm3, 0, sizeof(dMatrix3));
        dRfromQ( dm3, dquat );
        mGeom->setRotation( dm3 );
    }

    void PhysicalThing::setMassRotation( const Quaternion& q )
    {
        dReal dquat[4] = { q.w, q.x, q.y, q.z };
        dMatrix3 dm3;
        memset(dm3, 0, sizeof(dMatrix3));
        dRfromQ( dm3, dquat );
        //mass.rotate( dm3 );
    }

    void PhysicalThing::setSpace( dSpace* space )
    {
        if( mGeom )
        {
            mSpace->remove( mGeom->id() );
            space->add( mGeom->id() );
        }
        mSpace = space;
    }

    bool PhysicalThing::isColliding( void )
    {
        return mColliding;
    }

    void PhysicalThing::setColliding( bool collide )
    {
        if( mGeom )
        {
            mColliding = collide;        

            if(collide)
                mGeom->enable();
            else
                mGeom->disable();
        }
        else
            mColliding = false;        
    }

    bool PhysicalThing::isDynamic( void )
    {
        return mDynamic;
    }

    void PhysicalThing::setDynamic( bool dynamic )
    {
        if( mBody )
        {
            // Wenn Modus Aenderung Kraefte verwerfen
            if( mDynamic != dynamic )
                mBody->setForce( 0, 0, 0 );

            if(dynamic)
                mBody->enable();
            else
                mBody->disable();

            mDynamic = dynamic;
        }
        else
            mDynamic = false;
    }

    void PhysicalThing::stopDynamics()
    {
        if( mBody )
        {
            mBody->setAngularVel(0,0,0);
            mBody->setLinearVel(0,0,0);
            mBody->setForce( 0, 0, 0 );
            mBody->disable();
        }
    }

    void PhysicalThing::createCappedCylinderMass( Real density, Real radius, Real length, const Vector3& position, const Quaternion& orientation )
    {
        dBody* oldBody = mBody; 
        mBody = new dBody( PhysicsManager::getSingleton().getWorld()->id() );
        dReal dquat[4] = { orientation.w, orientation.x, orientation.y, orientation.z }; 
        mBody->setPosition( position.x, position.y, position.z ); 
        mBody->setQuaternion( dquat );
        mBody->setData((void*) this );

        dMass mass;
        mBody->getMass( &mass );
        mass.setCappedCylinder( density, 3, radius, length);
        mass.translate( position.x, position.y, position.z );

        setDynamic( true );

        if( mGeom )
            mGeom->setBody( mBody->id() );

        if( oldBody )
            delete oldBody;
    }

    void PhysicalThing::createBoxMass( Real density, const Vector3& length, const Vector3& position, const Quaternion& orientation)
    {
        dBody* oldBody = mBody;

        mBody = new dBody( PhysicsManager::getSingleton().getWorld()->id() );
        dReal dquat[4] = { orientation.w, orientation.x, orientation.y, orientation.z };
        mBody->setPosition( position.x, position.y, position.z ); 
        mBody->setQuaternion( dquat );
        mBody->setData((void*) this );

        dMass mass;
        mBody->getMass( &mass );
        mass.setBox( density, length.x, length.y, length.z);
        mass.translate( position.x, position.y, position.z );

        setDynamic( true );

        if( mGeom )
            mGeom->setBody( mBody->id() );

        if( oldBody )
            delete oldBody;
    }

    void PhysicalThing::createSphereMass( Real density, Real radius, const Vector3& position, const Quaternion& orientation )
    {
        dBody* oldBody = mBody;

        mBody = new dBody( PhysicsManager::getSingleton().getWorld()->id() );
        dReal dquat[4] = { orientation.w, orientation.x, orientation.y, orientation.z }; 
        mBody->setPosition( position.x, position.y, position.z ); 
        mBody->setQuaternion( dquat );
        mBody->setData((void*) this );

        dMass mass;
        mBody->getMass( &mass );
        mass.setSphere( density, radius);  

        setDynamic( true );

        if( mGeom )
            mGeom->setBody( mBody->id() );

        if( oldBody )
            delete oldBody;
    }

    void PhysicalThing::createCappedCylinderGeometry( Real radius, Real length, const Vector3& position, const Quaternion& orientation)
    {
        dGeom* oldGeom = mGeom;

        mGeom = (dGeom*)( new dCCylinder( mSpace->id(),radius,length ) );
        mGeom->setData((void*)this);
        mGeom->setPosition(position.x, position.y, position.z);
        setGeomRotation( orientation );
        setColliding( true );

        if( mBody )
            mGeom->setBody( mBody->id() );

        if( oldGeom )
            delete oldGeom;
    }

    /*
    Set the geom to a box
    */
    void PhysicalThing::createBoxGeometry( const Vector3& length, const Vector3& position, const Quaternion& orientation )
    {
        dGeom* oldGeom = mGeom;
        mGeom = (dGeom*)(new dBox(mSpace->id(),length.x,length.y,length.z));
        mGeom->setData((void*)this);
        mGeom->setPosition(position.x, position.y, position.z);
        setGeomRotation( orientation );
        setColliding( true );
        if( mBody )
            mGeom->setBody( mBody->id() );

        if( oldGeom )
            delete oldGeom;
    }

    /*
    Set the geom to a sphere
    */
    void PhysicalThing::createSphereGeometry( Real radius, const Vector3& position, const Quaternion& orientation )
    {
        dGeom* oldGeom = mGeom;
        mGeom = (dGeom*)( new dSphere( mSpace->id(),radius ) );
        mGeom->setData((void*)this);
        mGeom->setPosition(position.x, position.y, position.z);
        setGeomRotation( orientation  );
        setColliding( true );

        if( mBody )
            mGeom->setBody( mBody->id() );
        if( oldGeom )
            delete oldGeom;
    }

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
    dBodyID PhysicalThing::getBodyID(void)
    {
        if( mBody ) 
            return mBody->id();
        else 
            return 0;
    }

    dGeomID PhysicalThing::getGeomID( void )
    {
        if( mGeom ) 
            return mGeom->id();
        else 
            return 0;
    }

    /*
    Return the space that we were created in
    */
    dSpace* PhysicalThing::getSpace(void)
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
            // Get the position and orientation from ODE
            const dReal* pos = mBody->getPosition();
            const dReal* quat = mBody->getQuaternion();

            // Set the Actor according to the ODE data
            mActor->setPosition((Real)pos[0],(Real)pos[1],(Real)pos[2]);
            //LogManager::getSingleton().logMessage( String("Position x:")  << pos[0] << " y:" << pos[1] << " z:" << pos[2] << "\n" );

            mActor->setOrientation((Real)quat[0],(Real)quat[1],(Real)quat[2],(Real)quat[3]);
        }
    }

    bool PhysicalThing::testCollide(SceneQuery::WorldFragment* wf)
    {
        if( wf == 0 )
            return false;

        switch (wf->fragmentType)
        {
        case SceneQuery::WFT_NONE:
            return false;
        case SceneQuery::WFT_PLANE_BOUNDED_REGION:
            return testPlaneBoundsCollide( wf->planes );
        default:
            break;
        };

        // not handled
        return false;
    }

    bool PhysicalThing::testPlaneBoundsCollide( std::list<Plane>* planes )
    {
        bool collided = false;
        for ( std::list<Plane>::const_iterator planeIter = planes->begin(); planeIter != planes->end(); ++planeIter)
        {
            const Plane *boundPlane = &(*planeIter);
            Real dist = boundPlane->getDistance(mActor->getPosition());
            if (dist >= 0.0f)
            {
                dPlane odePlane(0, boundPlane->normal.x, boundPlane->normal.y, boundPlane->normal.z,-boundPlane->d);
                const int N = 10;
                dContact contact[N];
                int numColls = dCollide( mGeom->id(), odePlane.id() , N, &contact[0].geom, sizeof(dContact));
                if (numColls)
                {
                    for( int i = 0; i < numColls; i++ )
                    {
                        // Create contact joints if object is dynamics simulated
                        if ( this->isDynamic() )
                        {
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

                            dContactJoint contactJoint(
                                PhysicsManager::getSingleton().getWorld()->id(), 
                                PhysicsManager::getSingleton().getContactJointGroup()->id(), 
                                &contact[i]);

                            // Set ODE body, world fragment body is 0 clearly (immovable)
                            contactJoint.attach(mBody->id(), 0);
                        }
                    }
                    collided = true;
                }
            }
        }
        return collided;
    }

    bool PhysicalThing::testCollide(PhysicalThing* other)
    {
        bool collided = false;
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

                        dContactJoint contactJoint(
                            PhysicsManager::getSingleton().getWorld()->id(), 
                            PhysicsManager::getSingleton().getContactJointGroup()->id(), 
                            &contact[i]);

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
        return collided;
    }

    void PhysicalThing::addForce(const Vector3& direction)
    {
        addForce(direction.x*100, direction.y*100, direction.z*100 );
    }

    void PhysicalThing::addForce(Real dir_x, Real dir_y, Real dir_z)
    {
        if( mBody && mDynamic )
        {
            mBody->enable();
            mBody->addRelForce(dir_x*100, dir_y*100, dir_z*100);
        }
    }

    void PhysicalThing::addForceWorldSpace(const Vector3& direction)
    {
        addForceWorldSpace(direction.x, direction.y, direction.z );
    }

    void PhysicalThing::addForceWorldSpace(Real dir_x, Real dir_y, Real dir_z)
    {
        if( mBody && mDynamic )
        {
            mBody->enable();
            mBody->addForce(dir_x*100, dir_y*100, dir_z*100);
        }
    }

    void PhysicalThing::addForce(const Vector3& direction, const Vector3& atPosition)
    {
        addForce(direction.x, direction.y, direction.z,atPosition.x, atPosition.y, atPosition.z);
    }

    void PhysicalThing::addForce(Real dir_x, Real dir_y, Real dir_z,Real pos_x, Real pos_y, Real pos_z)
    {
        if( mBody && mDynamic )
        {
            mBody->enable();
            mBody->addForceAtRelPos(dir_x*100, dir_y*100, dir_z*100 ,pos_x, pos_y, pos_z);
        }
    }

    void PhysicalThing::addForceWorldSpace(const Vector3& direction, const Vector3& atPosition)
    {
        addForceWorldSpace(direction.x, direction.y, direction.z,atPosition.x, atPosition.y, atPosition.z);
    }

    void PhysicalThing::addForceWorldSpace(Real dir_x, Real dir_y, Real dir_z,Real pos_x, Real pos_y, Real pos_z)
    {
        if( mBody && mDynamic )
        {
            mBody->enable();
            mBody->addForceAtPos(dir_x*100, dir_y*100, dir_z*100,pos_x, pos_y, pos_z);
        }
    }

    void PhysicalThing::addTorque(const Vector3& direction)
    {
        addTorque(direction.x, direction.y, direction.z);
    }

    void PhysicalThing::addTorque(Real x, Real y, Real z)
    {
        if( mBody && mDynamic )
        {
            mBody->enable();
            mBody->addRelTorque(x*100, y*100, z*100);
        }
    }

    void PhysicalThing::addTorqueWorldSpace(const Vector3& direction)
    {
        addTorqueWorldSpace(direction.x, direction.y, direction.z);
    }

    void PhysicalThing::addTorqueWorldSpace(Real x, Real y, Real z)
    {
        if( mBody && mDynamic )
        {
            mBody->enable();
            mBody->addTorque(x*100, y*100, z*100);
        }
    }

    void PhysicalThing::setLinearVelocity(const Vector3& vel)
    {
        setLinearVelocity(vel.x, vel.y, vel.z);
    }

    void PhysicalThing::setLinearVelocity(Real x, Real y, Real z)
    {
        if( mBody && mDynamic )
        {
            mBody->enable();
            mBody->setLinearVel(x*100, y*100, z*100);
        }
    }

    void PhysicalThing::setAngularVelocity(const Vector3& vel)
    {
        setAngularVelocity(vel.x, vel.y, vel.z);
    }

    void PhysicalThing::setAngularVelocity(Real x, Real y, Real z)
    {
        if( mBody && mDynamic  )
        {
            mBody->enable();
            mBody->setAngularVel(x*100, y*100, z*100);
        }
    }

    const Vector3& PhysicalThing::getLinearVelocity(void)
    {
        if( mBody )
        {
            static Vector3 vel;
            const dReal* odeVel = mBody->getLinearVel();
            vel.x = odeVel[0] / 100;
            vel.y = odeVel[1] / 100;
            vel.z = odeVel[2] / 100;
            return vel;
        }
        return Vector3::ZERO;
    }

    const Vector3& PhysicalThing::getAngularVelocity(void)
    {
        if( mBody )
        {
            static Vector3 vel;
            const dReal* odeVel = mBody->getAngularVel();
            vel.x = odeVel[0] / 100;
            vel.y = odeVel[1] / 100;
            vel.z = odeVel[2] / 100;
            return vel;
        }

        return Vector3::ZERO;
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

