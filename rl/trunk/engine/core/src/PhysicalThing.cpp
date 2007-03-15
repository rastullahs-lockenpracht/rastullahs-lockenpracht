/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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


#include <OgreSceneNode.h>

#include "Actor.h"
#include "Exception.h"
#include "MathUtil.h"
#include "MeshObject.h"
#include "PhysicalObject.h"

using namespace Ogre;
using namespace OgreNewt;
using namespace OgreNewt::CollisionPrimitives;

namespace rl
{
	PhysicalThing::PhysicalThing(
		PhysicsManager::GeometryType geomType, PhysicalObject* po, Real mass, bool hullModifier)
		:
		mActor(NULL),
        mBody(NULL),
        mUpVectorJoint(NULL),
        mPendingForce(Vector3::ZERO),
        mOverrideGravity(false),
        mGravity(Vector3::ZERO),
        mContactListener(NULL),
        mPoseCollisions(),
		mGeometryType(geomType),
		mPhysicalObject(po),
		mMass(mass),
		mHullModifier(hullModifier),
        mPhysicsController(NULL)
	{
	}

    PhysicalThing::~PhysicalThing()
	{
		// missing removal of list of collision objects here ...
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
			mActor->_getSceneNode()->getWorldPosition(),
			mActor->_getSceneNode()->getWorldOrientation());
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
        mBody->attachToNode(node);
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
						offset++;		// question: do we really need this here?
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
                coll = createCollision(size, NULL, name);
                mPoseCollisions.insert(make_pair(name, coll));
            }
            else
            {
                // collision primitiv is cached, retrieve it
                coll = it->second;
            }
            mBody->setCollision(coll);

            // Set body-position so, that node-position is invariant.
            setPosition(mActor->_getSceneNode()->_getDerivedPosition());
			setOrientation(mActor->_getSceneNode()->_getDerivedOrientation());
        }
        else
        {
            Throw(IllegalArgumentException, "Poses are only supported by MeshObjects.");
        }
    }

	OgreNewt::CollisionPtr PhysicalThing::createCollision(
		const AxisAlignedBox& aabb, Vector3* inertiaCoefficients,
		const String animName, Vector3* offset, Quaternion* orientation ) const
    {
		const Vector3 size( aabb.getSize() );
		OgreNewt::World* physWorld = PhysicsManager::getSingleton()._getNewtonWorld();
		Ogre::Vector3 object_offset = Ogre::Vector3::ZERO;
		Ogre::Quaternion object_orientation = Ogre::Quaternion::IDENTITY;

        CollisionPtr rval;

		/* inertiaCoefficients could be calculated like OgreNewt::MomentOfInertia 
			or even better is to use it ? */

		/* differentiate between the different collision primitives, because they all
		   need different offset and probably different orientation values.
		   Newton SDK is really nifty and helps here, because we can shift the origin
		   of the coordinate system of the primitiv we create into any position we
		   desire. Actually this is the bottom middle of our mesh - as the meshes are
		   always constructed like that.
	    */
		if (mGeometryType == PhysicsManager::GT_BOX)
        {
			// set offset/orientation when they are null
			if (! offset) {
				offset = &object_offset;
				*offset = aabb.getCenter();
			}
			if (! orientation)
				orientation = &object_orientation;

			// a box collision primitiv has got it's coordinate system at it's center, so we need to shift it
			rval = CollisionPtr(new OgreNewt::CollisionPrimitives::Box(
				physWorld,
				size,
				*orientation,
				*offset)
				);

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

			// set offset/orientation when they are null
			if (! offset) {
				offset = &object_offset;
				*offset = Vector3(0,radius,0);
			}
			if (! orientation)
				orientation = &object_orientation;

			// a sphere primitiv has got its coordinate system at its center, so shift it with radius
            rval = CollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(
				physWorld,
				Vector3(radius, radius, radius),
				*orientation,
				*offset)
				);
            
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

			// set offset/orientation when they are null
			if (! offset) {
				offset = &object_offset;
				*offset = Vector3(0,s.y,0);
			}
			if (! orientation)
				orientation = &object_orientation;

			// an ellipsoid primitiv has got its coordinate system at its center, so shift it with radius
            rval = CollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(
				physWorld, 
				s,
				*orientation,
				*offset)
				);

            if (inertiaCoefficients != NULL)
			{
				*inertiaCoefficients = Vector3(s.x*s.x, s.y*s.y, s.z*s.z);
            }
        }
		else if (mGeometryType == PhysicsManager::GT_CAPSULE)
		{
			double radius = std::max(size.x, size.z) / 2.0;
			double height = size.y;

			// set offset/orientation when they are null
			if (! offset) {
				offset = &object_offset;
				*offset = Vector3(0, size.y/2, 0);
			}
			if (! orientation) {
				orientation = &object_orientation;
				orientation->FromAngleAxis(Degree(90), Vector3::UNIT_Z);
			}

			// an capsule primitiv has got its coordinate system at its center, so shift it with radius
			// additionally it is x axis aligned, so rotate it 90 degrees around z axis
			rval = CollisionPtr(new OgreNewt::CollisionPrimitives::Capsule(
				physWorld, 
				radius, 
				height,
				*orientation,
				*offset)
				);
			
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
				MeshObject *tempMesh = NULL;
				
				if( size.x < PhysicsManager::NEWTON_GRID_WIDTH ||
					size.y < PhysicsManager::NEWTON_GRID_WIDTH ||
					size.z < PhysicsManager::NEWTON_GRID_WIDTH )
				{
					// Objekt zu klein!
					LOG_MESSAGE(Logger::CORE, " PhyiscalThing too small to create a convexhull, using 'box' instead! ");

					// set offset/orientation when they are null
					if (! offset) {
						offset = &object_offset;
						*offset = aabb.getCenter();
					}
					if (! orientation)
						orientation = &object_orientation;
					
					rval = CollisionPtr(new CollisionPrimitives::Box(
						physWorld, 
						size, 
						*orientation,
						*offset)
						);
				}
				else
				{
					// set offset/orientation when they are null
					if (! offset)
						offset = &object_offset;
					if (! orientation)
						orientation = &object_orientation;

					// the problem fixed and it's source:
					// entity is a MeshObject containing the basic state of the Mesh, but
					// this function should create the physical bounding convex hull for one of the
					// animated states. Therefore the convex hull must be created from a mesh
					// representing the animated state and not from a mesh containing the basic state

					// check if this is a 'animated' state we have to create the convex hull for ...

					if (animName != "") {
						// Duplicating the MeshObject and animate it into the animName pose
						tempMesh = dynamic_cast<MeshObject*>(mPhysicalObject)->createPosedCopy(animName);

						entity = tempMesh->getEntity();
					}

					// calculate the convex hull of the animated mesh
					rval = CollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(physWorld,
						entity, true, *orientation, *offset));

					// cleanup the temporary mesh
					delete tempMesh;
				}
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
                        
            Vector3 inertiaCoefficients;
			OgreNewt::CollisionPtr coll = createCollision(aabb, &inertiaCoefficients);

			OgreNewt::Body* body = new OgreNewt::Body(
                PhysicsManager::getSingleton()._getNewtonWorld(), coll);

			Ogre::Real mass = mMass;
			if (mass > 0.0 && mGeometryType != PhysicsManager::GT_MESH)
            {
                body->setMassMatrix(mass, mass*inertiaCoefficients);
            }

			body->setCustomForceAndTorqueCallback(PhysicsManager::genericForceCallback);

            setBody(body);
        }
	}

    PhysicsController* PhysicalThing::getPhysicsController() const
    {
        return mPhysicsController;
    }

    void PhysicalThing::setPhysicsController(PhysicsController* controller)
    {
        if (mPhysicsController)
        {
            // if there is an old controller, remove it
            mBody->setCustomForceAndTorqueCallback( PhysicsManager::genericForceCallback );
            setUpConstraint(Vector3::ZERO);
            mPhysicsController = NULL;
        }

        if(controller)
        {
            // prepare for control
            mPhysicsController = controller;
            mBody->setAutoFreeze(0);
            mBody->unFreeze();
            mBody->setLinearDamping(0.0f);
            mBody->setAngularDamping(Vector3::ZERO);

            mBody->setCustomForceAndTorqueCallback( PhysicsManager::controlledForceCallback );

            // Set up-vector, so force application doesn't let the char fall over
            setUpConstraint(Vector3::UNIT_Y);
        }
    }

    void PhysicalThing::setMaterialID(const OgreNewt::MaterialID* materialid)
    {
        mBody->setMaterialGroupID(materialid);
    }

    const OgreNewt::MaterialID* PhysicalThing::getMaterialID() const
    {
        return mBody->getMaterialGroupID();
    }

}
