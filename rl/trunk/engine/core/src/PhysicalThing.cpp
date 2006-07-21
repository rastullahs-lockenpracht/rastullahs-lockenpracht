/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */

#include "PhysicalThing.h"
#include "OgreLogManager.h"
#include "MeshObject.h"
#include "Actor.h"
#include "Exception.h"
#include "PhysicalObject.h"
#include "MathUtil.h"

using namespace OgreNewt;
using namespace OgreNewt::CollisionPrimitives;

namespace rl
{
	PhysicalThing::PhysicalThing(
		PhysicsManager::GeometryTypes geomType, PhysicalObject* po, Real mass, bool hullModifier) 
		:
		mActor(NULL),
        mBody(NULL),
        mUpVectorJoint(NULL),
        mOffset(Vector3::ZERO),
		mOrientationBias(Quaternion::IDENTITY),
        mPendingForce(Vector3::ZERO),
        mOverrideGravity(false),
        mGravity(Vector3::ZERO),
        mContactListener(NULL),
        mPoseCollisions(),
		mGeometryType(geomType),
		mPhysicalObject(po),
		mMass(mass),
		mHullModifier(hullModifier)
	{
	}

    PhysicalThing::~PhysicalThing()
	{
		delete mBody;
		mBody = NULL;
    }

    Ogre::Vector3 PhysicalThing::getPosition() const
    {
        Quaternion quat;
        Vector3 pos;
        mBody->getPositionOrientation(pos, quat);
        return pos;
    }

    void PhysicalThing::setPosition(Real x, Real y, Real z)
    {
        setPosition(Vector3(x, y, z));
    }

    void PhysicalThing::setPosition(const Vector3& pos)
    {
        Quaternion quat;
        Vector3 oldPos;
        mBody->getPositionOrientation(oldPos, quat);
        mBody->setPositionOrientation(pos, quat);
    }

    Ogre::Quaternion PhysicalThing::getOrientation() const
    {
        Quaternion quat;
        Vector3 pos;
        mBody->getPositionOrientation(pos, quat);
        return quat;
    }

    void PhysicalThing::setOrientation(Real w, Real x, Real y, Real z)
    {
        setOrientation(Quaternion(w, x, y, z));
    }

    void PhysicalThing::setOrientation(const Quaternion& orientation)
    {
        Quaternion oldOrientation;
        Vector3 pos;
        mBody->getPositionOrientation(pos, oldOrientation);
        mBody->setPositionOrientation(pos, orientation);
    }

    void PhysicalThing::setVelocity(const Vector3& vel)
    {
        mBody->setVelocity(vel);
    }

	void PhysicalThing::_setOrientationBias(const Ogre::Quaternion& orientation)
	{
		mOrientationBias = orientation;
	}

	void PhysicalThing::_setOffset(const Vector3& offset)
	{
		mOffset = offset;
        mBody->setOffset(offset);
	}

	PhysicalObject* PhysicalThing::_getPhysicalObject() const
	{
		return mPhysicalObject;
	}

    Actor *PhysicalThing::getActor(void) const
    {
        return mActor;
    }

    OgreNewt::Body* PhysicalThing::_getBody() const
    {
        return mBody;
    }

	void PhysicalThing::_setBody(OgreNewt::Body* body)
    {
        mBody = body;
		mBody->setUserData(mActor);
    }

    void PhysicalThing::_update()
    {
        mBody->setPositionOrientation(mActor->_getSceneNode()->getWorldPosition() + mBody->getOffset(),
            mOrientationBias.Inverse() * mActor->_getSceneNode()->getWorldOrientation());
		mActor->_update(Actor::UF_ALL & ~Actor::UF_PHYSICAL_THING);
    }

    void PhysicalThing::_setActor(Actor* actor)
    {
        mActor = actor;
		if (mBody != NULL)
			mBody->setUserData(actor);
    }

    void PhysicalThing::_attachToSceneNode(Ogre::SceneNode* node)
    {
        mBody->attachToNode(node, mOffset, mOrientationBias);
    }

    void PhysicalThing::_attachToBone(MeshObject* object, const std::string& boneName )
    {
        //Entity* attachTarget = object->getEntity();
        //Body* body = mGeometry->getBody();
        //attachTarget->attachObjectToBone(boneName, body);
    }

    void PhysicalThing::_detachFromSceneNode(Ogre::SceneNode* node)
    {
        ///\todo Wie implementieren?
    }

    void PhysicalThing::setUpConstraint(const Vector3& upVector)
    {
        if (!mUpVectorJoint)
        {
            mUpVectorJoint = new OgreNewt::BasicJoints::UpVector(
                mBody->getWorld(), mBody, upVector);
        }
        else
        {
            mUpVectorJoint->setPin(upVector);
        }
    }

    Ogre::Vector3 PhysicalThing::getUpConstraint() const
    {
        Vector3 rval = Vector3::ZERO;
        if (mUpVectorJoint)
        {
            rval = mUpVectorJoint->getPin();
        }
        return rval;
    }

    void PhysicalThing::clearUpConstraint()
    {
        delete mUpVectorJoint;
        mUpVectorJoint = NULL;
    }

    void PhysicalThing::onApplyForceAndTorque()
    {
        Vector3 gravity = mOverrideGravity ?
            mGravity : PhysicsManager::getSingleton().getGravity();
        mBody->addForce(mPendingForce + gravity*getMass());
        mPendingForce = Vector3::ZERO;
    }

    void PhysicalThing::addForce(const Ogre::Vector3& force)
    {
        mBody->unFreeze();
        mPendingForce += force;        
    }

    Ogre::Real PhysicalThing::getMass() const
    {
        return mMass;
    }

    void PhysicalThing::setMass(Ogre::Real mass)
    {
        mMass = mass;
        mBody->setMass(mass);
    }

	PhysicsManager::GeometryTypes PhysicalThing::_getGeometryType() const
	{
		return mGeometryType;
	}

	bool PhysicalThing::getHullModifier() const
	{
		return mHullModifier;
	}

    void PhysicalThing::setGravityOverride(bool override, const Vector3& gravity)
    {
        mOverrideGravity = override;
        mGravity = mOverrideGravity ? gravity : Vector3::ZERO;
    }

    void PhysicalThing::setGravityOverride(bool override, Real x, Real y, Real z)
    {
        setGravityOverride(override, Vector3(x, y, z));
    }

    void PhysicalThing::updateCollisionHull()
    {
        Entity* entity = static_cast<Entity*>(mActor->_getMovableObject());
        entity->_updateAnimation();
        Node* node = entity->getParentNode();
        RlAssert(node, "Actor has to be placed in the scene in order to update its collision hull.");
        

        Vector3 position;
        Quaternion orientation;
        mBody->getPositionOrientation(position, orientation);

		if (mGeometryType == PhysicsManager::GT_CONVEXHULL)
		{
			Matrix4 transform = node->_getFullTransform().inverse();

			std::vector<Vector3> vertices;
			vertices.reserve(100);

			bool sharedAdded = false;
			// loop over subentities and retrieve vertex positions
			size_t subentityCount = entity->getNumSubEntities();
			for (size_t i = 0; i < subentityCount; ++i)
			{
				SubEntity* subEntity = entity->getSubEntity(i);
				VertexData* vdata = 0;
				if (subEntity->getSubMesh()->useSharedVertices && !sharedAdded)
				{
					vdata = entity->_getSkelAnimVertexData();
					sharedAdded = true;
				}
				else if (!subEntity->getSubMesh()->useSharedVertices)
				{
					vdata = subEntity->_getSkelAnimVertexData();
				}

				if (vdata)
				{
					size_t vcount = vdata->vertexCount;
					VertexDeclaration* vdecl = vdata->vertexDeclaration;
					const VertexElement* velem = vdecl->findElementBySemantic( Ogre::VES_POSITION );

					HardwareVertexBufferSharedPtr vbuffer =
						vdata->vertexBufferBinding->getBuffer(velem->getSource());
					unsigned char* vptr = static_cast<unsigned char*>(
						vbuffer->lock(HardwareBuffer::HBL_READ_ONLY ));

					//loop through vertex data...
					size_t start = vdata->vertexStart;
					for (size_t j = start; j < (start + vcount); ++j)
					{
						float* vpos;
						//get offset to Position data!
						unsigned char* offset = vptr + (j * vbuffer->getVertexSize());
						velem->baseVertexPointerToElement(offset, &vpos);
						Vector3 v = Vector3(vpos[0], vpos[1], vpos[2]);
						//Positions in world space. So we need to transform them back.
						vertices.push_back(transform * v);
						offset++;
					}

					vbuffer->unlock();
				}
			}
	        CollisionPtr collision(new ConvexHull(PhysicsManager::getSingleton()._getNewtonWorld(),
			    &vertices[0], vertices.size()));
		    mBody->setCollision(collision);
		}
		else if (mGeometryType == PhysicsManager::GT_MESH)
		{
	        CollisionPtr collision(new TreeCollision(
				PhysicsManager::getSingleton()._getNewtonWorld(), entity, false));
		    mBody->setCollision(collision);
		}

        mBody->setPositionOrientation(position, orientation);
    }

    void PhysicalThing::freeze()
    {
        mBody->freeze();
    }

    void PhysicalThing::unfreeze()
    {
        mBody->unFreeze();
    }

    void PhysicalThing::setContactListener(PhysicsContactListener* listener)
    {
        mContactListener = listener;
    }

    PhysicsContactListener* PhysicalThing::getContactListener() const
    {
        return mContactListener;
    }

    void PhysicalThing::fitToPose(const Ogre::String& name)
    {
        AxisAlignedBox def_size = mPhysicalObject->getDefaultSize();
        
        // Do we already have a collision for the wanted pose?
        CollisionMap::iterator it = mPoseCollisions.find(name);

        AxisAlignedBox size = mPhysicalObject->getPoseSize(name);

        CollisionPtr coll;
        if (it == mPoseCollisions.end())
        {
            // No, so create it and put it into the map
            coll = PhysicsManager::getSingleton()._createCollision(this, size);
            if (getHullModifier())
            {
                coll = CollisionPtr(new CollisionPrimitives::HullModifier(
                    PhysicsManager::getSingleton()._getNewtonWorld(), coll));
            }
            mPoseCollisions.insert(make_pair(name, coll));
        }
        else
        {
            // Yes
            coll = it->second;
        }

        mBody->setCollision(coll);

        // Align bottoms of new and old collisions
        Real yoffset = size.getCenter().y - mOffset.y;
        Quaternion orientation;
        Vector3 pos;
        mBody->getPositionOrientation(pos, orientation);
        Vector3 newpos = pos + Vector3(0, yoffset, 0);
        mBody->setPositionOrientation(newpos, orientation);

        Vector3 oldoffset = mOffset;
        // Adjust the node offset to fit the new form.
        _setOffset(size.getCenter());

        Logger::getSingleton().log(Logger::CORE, Logger::LL_MESSAGE,
            mActor->getName() + ": fit_to_pose " + name +
            "\nDefaultSize: " +
                StringConverter::toString(def_size.getMinimum()) + " / " +
                StringConverter::toString(def_size.getMaximum()) +
            String("\nold pos: ") + StringConverter::toString(pos) +
            String("\nnew pos: ") + StringConverter::toString(newpos) +
            String("\nnew size: ") +
                StringConverter::toString(size.getMinimum()) + " / " +
                StringConverter::toString(size.getMaximum()) +
            String("\nold offset: ") + StringConverter::toString(oldoffset) +
            String("\nnew offset: ") + StringConverter::toString(mOffset));
    }
}
