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

#include "PhysicalThing.h"

#include "OgreNoMemoryMacros.h"
#include <ode/ode.h>
#include "OgreMemoryMacros.h"
#include "OgreLogManager.h"
#include "MeshObject.h"
#include "Actor.h"
#include "PhysicsManager.h"
#include "Exception.h"

using namespace OgreOde;

namespace rl
{
	PhysicalThing::PhysicalThing( Geometry* geo, 
        const Vector3& offsetPosition,
        const Ogre::Quaternion& orientationRotation )
        :   mBounceRestitution(0.8f),
            mBounceVelocityThresh(0.0005f),
            mSoftness(0.0f),
            mFriction(Utility::Infinity),
            mSoftErp(0.0f),
            mGeometry(geo),
            mActor(NULL),
            mOffset(offsetPosition),
            mOrientationBias(orientationRotation),
			mEntityInformer(NULL)
	{
        if (getBody())
        {
            getBody()->setUserData(reinterpret_cast<unsigned long>(this));
        }
    }

    PhysicalThing::~PhysicalThing()
    {
        delete mGeometry;
        mGeometry = 0;
    }

    void PhysicalThing::setPosition(Real x, Real y, Real z)
    {
        mGeometry->setPosition(Vector3(x,y,z));
    }

    void PhysicalThing::setOrientation( Real w, Real x, Real y, Real z )
    {
        mGeometry->setOrientation(Quaternion(w,x,y,z)); 
    }

    bool PhysicalThing::isDynamic(void)
    {
        return (mGeometry->getBody() != 0) && mGeometry->getBody()->isAwake();
    }

    void PhysicalThing::setDynamic(bool dynamic)
    {
        if (mGeometry->getBody())
        {
            if(dynamic)
                mGeometry->getBody()->wake();
            else
                mGeometry->getBody()->sleep();
        }
    }

    Actor *PhysicalThing::getActor(void)
    {
        return mActor;
    }

    Body* PhysicalThing::getBody(void)
    {
        return mGeometry->getBody();
    }

    Geometry* PhysicalThing::getGeometry( void )
    {
        return mGeometry;
    }

    void PhysicalThing::addForce(const Vector3& direction)
    {
        addForce(direction.x, direction.y, direction.z);
    }

    void PhysicalThing::addForce(Real dir_x, Real dir_y, Real dir_z)
    {
        if (mGeometry->getBody())
        {
            mGeometry->getBody()->wake();
            mGeometry->getBody()->addRelativeForce(
                Vector3(dir_x, dir_y, dir_z));
        }
    }

    void PhysicalThing::addForceWorldSpace(const Vector3& direction)
    {
        addForceWorldSpace(direction.x, direction.y, direction.z );
    }

    void PhysicalThing::addForceWorldSpace(Real dir_x, Real dir_y, Real dir_z)
    {
        if (mGeometry->getBody())
        {
            mGeometry->getBody()->wake();
            mGeometry->getBody()->addForce(Vector3(dir_x, dir_y, dir_z));
        }
    }

    void PhysicalThing::addForce(const Vector3& dir, const Vector3& pos)
    {
        addForce(dir.x, dir.y, dir.z, pos.x, pos.y, pos.z);
    }

    void PhysicalThing::addForce(Real dir_x, Real dir_y, Real dir_z,
        Real pos_x, Real pos_y, Real pos_z)
    {
        if (mGeometry->getBody())
        {
            mGeometry->getBody()->wake();
            mGeometry->getBody()->addRelativeForceAtRelative(
                Vector3(dir_x, dir_y, dir_z),
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
        if (mGeometry->getBody())
        {
            mGeometry->getBody()->wake();
            mGeometry->getBody()->addForceAt(Vector3(dir_x, dir_y, dir_z),
                Vector3(pos_x, pos_y, pos_z));
        }
    }

    void PhysicalThing::addTorque(const Vector3& direction)
    {
        addTorque(direction.x, direction.y, direction.z);
    }

    void PhysicalThing::addTorque(Real x, Real y, Real z)
    {
        if (mGeometry->getBody())
        {
            mGeometry->getBody()->wake();
            mGeometry->getBody()->addRelativeTorque(Vector3(x, y, z));
        }
    }

    void PhysicalThing::addTorqueWorldSpace(const Vector3& direction)
    {
        addTorqueWorldSpace(direction.x, direction.y, direction.z);
    }

    void PhysicalThing::addTorqueWorldSpace(Real x, Real y, Real z)
    {
        if (mGeometry->getBody())
        {
            mGeometry->getBody()->wake();
            mGeometry->getBody()->addTorque(Vector3(x, y, z));
        }
    }

    void PhysicalThing::setLinearVelocity(const Vector3& vel)
    {
        setLinearVelocity(vel.x, vel.y, vel.z);
    }

    void PhysicalThing::setLinearVelocity(Real x, Real y, Real z)
    {
        if (mGeometry->getBody())
        {
            mGeometry->getBody()->wake();
            mGeometry->getBody()->setLinearVelocity(Vector3(x, y, z));
        }
    }

    void PhysicalThing::setAngularVelocity(const Vector3& vel)
    {
        setAngularVelocity(vel.x, vel.y, vel.z);
    }

    void PhysicalThing::setAngularVelocity(Real x, Real y, Real z)
    {
        if (mGeometry->getBody())
        {
            mGeometry->getBody()->wake();
            mGeometry->getBody()->setAngularVelocity(Vector3(x, y, z));
        }
    }

    const Vector3 PhysicalThing::getLinearVelocity(void)
    {
        Vector3 rval(Vector3::ZERO);
        if (mGeometry->getBody())
        {
            rval = mGeometry->getBody()->getLinearVelocity();
        }
        return rval;
    }

    const Vector3 PhysicalThing::getAngularVelocity(void)
    {
        Vector3 rval(Vector3::ZERO);
        if (mGeometry->getBody())
        {
            rval = mGeometry->getBody()->getAngularVelocity();
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
    
    void PhysicalThing::_update()
    {
        mGeometry->setPosition(
            mActor->_getSceneNode()->getWorldPosition() + mOffset);
        mGeometry->setOrientation(
            mActor->_getSceneNode()->getWorldOrientation() * mOrientationBias);
    }
    
    void PhysicalThing::_setActor(Actor* actor)
    {
        mActor = actor;
		if (mGeometry != NULL)
			mGeometry->setUserData(reinterpret_cast<unsigned long>(actor));
    }
    
    void PhysicalThing::_attachToSceneNode(Ogre::SceneNode* node)
    {
        if (mGeometry->getBody())
        {
            node->attachObject(mGeometry->getBody());
        }
    }

	void PhysicalThing::_attachToBone(MeshObject* object, const std::string& boneName )
	{
		Entity* attachTarget = object->getEntity();
		Body* body = mGeometry->getBody();
		attachTarget->attachObjectToBone(boneName, body);
	}
    
    void PhysicalThing::_detachFromSceneNode(Ogre::SceneNode* node)
    {
        if (mGeometry->getBody())
        {
            node->detachObject(mGeometry->getBody());
        }
    }

	void PhysicalThing::createEntityInformer(MeshObject* object)
	{
		delete mEntityInformer;
		mEntityInformer = NULL;

		Entity* attachTarget = object->getEntity();
		mEntityInformer = new EntityInformer(attachTarget);
	}

	BoxGeometry* PhysicalThing::createBoxGeometry(Space* space, Bone* bone)
	{
		//if (mEntityInformer != NULL)
			return mEntityInformer->createOrientedBox(bone->getHandle(), space);

		//return new BoxGeometry(space);
	}

	

}

