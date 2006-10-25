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
        PhysicsManager::getSingleton().setPhysicsController( this, NULL );
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

	void PhysicalThing::setOffset(const Vector3& offset)
	{
		mOffset = offset;
        mBody->setOffset(offset);
	}

    Actor *PhysicalThing::getActor(void) const
    {
        return mActor;
    }

    OgreNewt::Body* PhysicalThing::_getBody() const
    {
        return mBody;
    }

	void PhysicalThing::setBody(OgreNewt::Body* body)
    {
        mBody = body;
		mBody->setUserData(mActor);
    }

    void PhysicalThing::_update()
    {
        mBody->setPositionOrientation(
            mActor->_getSceneNode()->getWorldPosition() + mBody->getOffset(),
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
        RlAssert(node,
            "Actor has to be placed in the scene in order to update its collision hull.");
        

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
		Vector3 offset;
		Quaternion orientationBias;
		CollisionPtr coll;
        
        if (mPhysicalObject->isMeshObject())
        {
            MeshObject* meshObject = dynamic_cast<MeshObject*>(mPhysicalObject);
            AxisAlignedBox size = meshObject->getPoseSize(name);

            // Do we already have a collision for the wanted pose?
            CollisionMap::iterator it = mPoseCollisions.find(name);
            if (it == mPoseCollisions.end())
            {
                // No, so create it and put it into the map
                coll = createCollision(size, &offset, &orientationBias);
                mPoseCollisions.insert(make_pair(name, coll));
            }
            else
            {
                // Yes
                coll = it->second;
                offset = size.getCenter();
                orientationBias = Quaternion::IDENTITY;
            }
            setOffset(offset);
            mBody->setCollision(coll);
            mOrientationBias = orientationBias;

            // Set body-position so, that node-position is invariant.
            setPosition(mActor->_getSceneNode()->_getDerivedPosition() +
                mActor->_getSceneNode()->_getDerivedOrientation() * offset);
        }
        else
        {
            Throw(IllegalArgumentException, "Poses are only supported by MeshObjects.");
        }
    }

	OgreNewt::CollisionPtr PhysicalThing::createCollision(
		const AxisAlignedBox& aabb, Vector3* offset, 
		Quaternion* orientation, Vector3* inertiaCoefficients) const
    {
		const Vector3 size = aabb.getMaximum() - aabb.getMinimum();

		if (offset != NULL)
		{
			*offset = aabb.getCenter();
		}

		OgreNewt::World* physWorld = PhysicsManager::getSingleton()._getNewtonWorld();

        CollisionPtr rval;

		if (mGeometryType == PhysicsManager::GT_BOX)
        {
            rval = CollisionPtr(new CollisionPrimitives::Box(physWorld, size));
			if (inertiaCoefficients != NULL)
			{
                *inertiaCoefficients = Vector3(
					size.x*size.x/6.0f,
                    size.y*size.y/6.0f,
                    size.z*size.z/6.0f);
			}
        }
        else if (mGeometryType == PhysicsManager::GT_SPHERE)
        {
            double radius = std::max(size.x, std::max(size.y, size.z)) / 2.0;
            rval = CollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(physWorld,
                Vector3(radius, radius, radius)));
            
			if (inertiaCoefficients != NULL)
			{
				*inertiaCoefficients = Vector3(radius*radius, radius*radius, radius*radius);
			}
        }
        else if (mGeometryType == PhysicsManager::GT_ELLIPSOID)
        {
            // set the size x/z values to the maximum
            Vector3 s(size/2.0);
            s.x = std::max(s.x, s.z);
            s.z = s.x;
            rval = CollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(physWorld, s));

            if (inertiaCoefficients != NULL)
			{
				*inertiaCoefficients = Vector3(s.x*s.x, s.y*s.y, s.z*s.z);
            }
        }
		else if (mGeometryType == PhysicsManager::GT_CAPSULE)
		{
			double radius = std::max(size.x, size.z) / 2.0;
			double height = size.y;
			
			Quaternion orientCaps;
			orientCaps.FromAngleAxis(Degree(90), Vector3::UNIT_Z);

			Vector3 offsetCaps(-size.y/2, 0, 0);

			rval = CollisionPtr(new OgreNewt::CollisionPrimitives::Capsule(
						physWorld, 
						radius, 
						height,
						orientCaps,
						offsetCaps));
			//Alte Variante
			//coll = CollisionPtr(new CollisionPrimitives::Capsule(mWorld, radius, size.y));
			if (offset != NULL)
			{
				*offset = offsetCaps;
			}

			if (orientation != NULL)
			{
				*orientation = orientCaps;
			}

			if (inertiaCoefficients != NULL)
			{
				double sradius = radius*radius;
				*inertiaCoefficients = Vector3(sradius, size.y*size.y, sradius);
			}
		}
        else if (mGeometryType == PhysicsManager::GT_CONVEXHULL)
        {
            if (mPhysicalObject->isMeshObject())
            {
				Entity* entity = dynamic_cast<MeshObject*>(mPhysicalObject)->getEntity();

				if( size.x < PhysicsManager::NEWTON_GRID_WIDTH ||
					size.y < PhysicsManager::NEWTON_GRID_WIDTH ||
					size.z < PhysicsManager::NEWTON_GRID_WIDTH )
				{
					// Objekt zu klein!
					LOG_MESSAGE(Logger::CORE, " PhyiscalThing too small to create a convexhull, using 'box' instead! ");
					rval = CollisionPtr(new CollisionPrimitives::Box(physWorld, size));
					
					if (inertiaCoefficients != NULL)
					{
						*inertiaCoefficients = Vector3(
						size.x*size.x/6.0f,
						size.y*size.y/6.0f,
						size.z*size.z/6.0f);
					}
				}
				else
				{
					
					rval = CollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(physWorld,
						entity, true));

					if (offset != NULL)
					{
						*offset = Vector3::ZERO;
					}
				}
            }
            else
            {
                Throw(IllegalArgumentException, "ConvexHull not possible for non-mesh objects.");
            }
        }
        else if (mGeometryType == PhysicsManager::GT_MESH)
        {
            if (mPhysicalObject->isMeshObject())
            {
                Entity* entity = dynamic_cast<MeshObject*>(mPhysicalObject)->getEntity();
                rval = CollisionPtr(new OgreNewt::CollisionPrimitives::TreeCollision(physWorld,
                    entity, false, true));

                if (offset != NULL)
                {
                    *offset = Vector3::ZERO;
                }
            }
            else
            {
                Throw(IllegalArgumentException,
                    "TreeCollision not possible for non-mesh objects.");
            }
        }
        else
        {
            Throw(IllegalArgumentException, "unknown geometry type.");
        }

		if (mHullModifier)
        {
            return CollisionPtr(new CollisionPrimitives::HullModifier(physWorld, rval));
        }

        return rval;
    }

	void PhysicalThing::createPhysicsProxy(SceneNode* node)
	{
		if (mBody == NULL) 
		{
            const AxisAlignedBox& aabb = mPhysicalObject->getDefaultSize();
                        
			Quaternion orientationBias;
            Vector3 inertiaCoefficients;
			Vector3 offset;
			OgreNewt::CollisionPtr coll = createCollision(aabb, &offset,
                &orientationBias, &inertiaCoefficients);

			OgreNewt::Body* body = new OgreNewt::Body(
                PhysicsManager::getSingleton()._getNewtonWorld(), coll);

			Ogre::Real mass = mMass;
			if (mass > 0.0 && mGeometryType != PhysicsManager::GT_MESH)
            {
                body->setMassMatrix(mass, mass*inertiaCoefficients);
            }

			body->setCustomForceAndTorqueCallback(PhysicsManager::genericForceCallback);

            setBody(body);
            setOffset(offset);
			mOrientationBias = orientationBias;
        }
	}

	void PhysicalThing::prepareUserControl(OgreNewt::MaterialID* material)
    {
        mBody->setMaterialGroupID(material);
        mBody->setAutoFreeze(0);
        mBody->unFreeze();
        mBody->setLinearDamping(0.0f);
        mBody->setAngularDamping(Vector3::ZERO);

        mBody->setCustomForceAndTorqueCallback( PhysicsManager::controlledForceCallback );

        // Set up-vector, so force application doesn't let the char fall over
        setUpConstraint(Vector3::UNIT_Y);
    }

    void PhysicalThing::unprepareUserControl()
    {
		mBody->setMaterialGroupID(PhysicsManager::getSingleton()._getDefaultMaterialID());
        mBody->setCustomForceAndTorqueCallback( PhysicsManager::genericForceCallback );
        setUpConstraint(Vector3::ZERO);
    }
}
