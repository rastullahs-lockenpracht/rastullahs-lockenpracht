/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h" //precompiled header

#include "PhysicalThing.h"

#include "Actor.h"
#include "Exception.h"
#include "MathUtil.h"
#include "MeshObject.h"
#include "PhysicalObject.h"
#include "PhysicsRagDoll.h"

using namespace Ogre;
using namespace OgreNewt;

namespace rl
{
    PhysicalThing::PhysicalThing(GeometryType geomType, PhysicalObject* po, Real mass, bool hullModifier)
        : mActor(NULL)
        , mBody(NULL)
        , mUpVectorJoint(NULL)
        , mPendingForce(Vector3::ZERO)
        , mOverrideGravity(false)
        , mGravity(Vector3::ZERO)
        , mContactListener(NULL)
        , mPoseCollisions()
        , mGeometryType(geomType)
        , mPhysicalObject(po)
        , mMass(mass)
        , mHullModifier(hullModifier)
        , mPhysicsController(NULL)
        , mRagDoll(NULL)
    {
    }

    PhysicalThing::~PhysicalThing()
    {
        destroyPhysicsProxy();
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
        if (mRagDoll)
            mRagDoll->setPositionOrientation(pos, quat);
        else
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
        if (mRagDoll)
            mRagDoll->setPositionOrientation(pos, orientation);
        else
            mBody->setPositionOrientation(pos, orientation);
    }

    void PhysicalThing::setVelocity(const Vector3& vel)
    {
        mBody->setVelocity(vel);
    }

    Vector3 PhysicalThing::getVelocity()
    {
        return mBody->getVelocity();
    }

    Actor* PhysicalThing::getActor(void) const
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
        mBody->setUserData(Ogre::Any(mActor));
    }

    void PhysicalThing::_update()
    {
        if (mBody && mActor)
        {
            if (mRagDoll)
            {
                mRagDoll->setPositionOrientation(
                    mActor->_getSceneNode()->_getDerivedPosition(), mActor->_getSceneNode()->_getDerivedOrientation());
            }
            else
            {
                mBody->setPositionOrientation(
                    mActor->_getSceneNode()->_getDerivedPosition(), mActor->_getSceneNode()->_getDerivedOrientation());
            }
            mActor->_update(Actor::UF_ALL & ~Actor::UF_PHYSICAL_THING);
        }
    }

    void PhysicalThing::_setActor(Actor* actor)
    {
        mActor = actor;
        if (mBody != NULL)
        {
            mBody->setUserData(Ogre::Any(mActor));
        }
    }

    void PhysicalThing::_attachToSceneNode(Ogre::SceneNode* node)
    {
        if (mRagDoll)
            mRagDoll->setSceneNode(node);
        else
            mBody->attachNode(node);
    }

    void PhysicalThing::_attachToBone(MeshObject* object, const std::string& boneName)
    {
        // Entity* attachTarget = object->getEntity();
        // Body* body = mGeometry->getBody();
        // attachTarget->attachObjectToBone(boneName, body);
    }

    void PhysicalThing::_detachFromSceneNode(Ogre::SceneNode* node)
    {
        ///\todo Wie implementieren?
    }

    void PhysicalThing::setUpConstraint(const Vector3& upVector)
    {
        RlAssert(!mRagDoll,
            "PhysicalThing::setUpConstraint: using UpConstraint not possible while a RagDoll controls this "
            "PhysicalThing!");

        if (!mUpVectorJoint)
        {
            mUpVectorJoint = new OgreNewt::BasicJoints::UpVector(mBody->getWorld(), mBody, upVector);
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
        if (mUpVectorJoint)
        {
            delete mUpVectorJoint;
            mUpVectorJoint = NULL;
        }
    }

    void PhysicalThing::onApplyForceAndTorque(float timestep)
    {
        Vector3 gravity = mOverrideGravity ? mGravity : PhysicsManager::getSingleton().getGravity();
        mBody->addForce(mPendingForce + gravity * getMass());
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
        // effect on ragdoll?

        if (mBody && !mRagDoll)
        {
            Vector3 inertia;
            Real oldMass;
            mBody->getMassMatrix(oldMass, inertia);
            mMass = mass;
            if (oldMass > 0)
                inertia = inertia / oldMass * mass;

            mBody->setMassMatrix(mass, inertia);
        }
        mMass = mass;
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
            "PhysicalThing::updateCollisionHull: Actor has to be placed in the scene in order to update its collision "
            "hull.");
        RlAssert(!mRagDoll,
            "PhysicalThing::updateCollisionHull: PhysicalThing must not be controlled by a RagDoll in order to update "
            "its collision hull.");

        Vector3 position;
        Quaternion orientation;
        mBody->getPositionOrientation(position, orientation);
        CollisionPtr collision = PhysicsManager::getSingleton().createCollision(entity, mGeometryType, "",
            Vector3::ZERO, Quaternion::IDENTITY, 0, NULL, NULL,
            true); // don't cache
        if (collision)
        {
            mBody->setCollision(collision);
            mBody->setPositionOrientation(position, orientation);
        }

        /*
                if (mGeometryType == GT_CONVEXHULL)
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
                    CollisionPtr collision(new
           CollisionPrimitives::ConvexHull(PhysicsManager::getSingleton()._getNewtonWorld(), &vertices[0],
           vertices.size())); mBody->setCollision(collision);
                }
                else if (mGeometryType == GT_MESH)
                {
                    CollisionPtr collision(new CollisionPrimitives::TreeCollision(
                        PhysicsManager::getSingleton()._getNewtonWorld(), entity, true));
                    mBody->setCollision(collision);
                }

                mBody->setPositionOrientation(position, orientation);
        */
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

    void PhysicalThing::fitToPose(const Ogre::String& animName)
    {
        if (mRagDoll)
            Throw(IllegalArgumentException,
                "PhysicalThing::fitToPose cannot be used while the PhysicalThing is controlled by a RagDoll!");

        CollisionPtr coll;

        if (mPhysicalObject->isMeshObject())
        {
            MeshObject* meshObject = dynamic_cast<MeshObject*>(mPhysicalObject);
            // AxisAlignedBox size = meshObject->getPoseSize(name);

            // Do we already have a collision for the wanted pose?
            CollisionMap::iterator it = mPoseCollisions.find(animName);
            if (it == mPoseCollisions.end())
            {
                Entity* entity = meshObject->getEntity();
                MeshObject* tempMesh = NULL;

                // the problem fixed and it's source:
                // entity is a MeshObject containing the basic state of the Mesh, but
                // this function should create the physical bounding convex hull for one of the
                // animated states. Therefore the convex hull must be created from a mesh
                // representing the animated state and not from a mesh containing the basic state

                // check if this is a 'animated' state we have to create the convex hull for ...

                if (animName != "" && meshObject->hasAnimation(animName))
                {
                    // Duplicating the MeshObject and animate it into the animName pose
                    tempMesh = meshObject->createPosedCopy(animName);

                    entity = tempMesh->getEntity();
                }

                // create the collision primitive of the animated mesh
                coll = PhysicsManager::getSingleton().createCollision(entity, mGeometryType, animName);

                // cleanup the temporary mesh
                delete tempMesh;

                // No, so create it and put it into the map
                mPoseCollisions.insert(make_pair(animName, coll));
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

    void PhysicalThing::destroyPhysicsProxy()
    {
        setPhysicsController(NULL);
        clearUpConstraint();
        mPoseCollisions.clear();
        if (mRagDoll)
        {
            delete mRagDoll;
            mRagDoll = NULL;
            mBody = NULL;
        }
        else if (mBody)
        {
            delete mBody;
            mBody = NULL;
        }
    }

    void PhysicalThing::createPhysicsProxy()
    {
        RlAssert(!mRagDoll,
            "PhysicalThing::createPhysicsProxy: cannot create physics proxy while the PhysicalThing is controlled by a "
            "RagDoll!");
        if (!mBody)
        {
            Vector3 inertia, centerOfMass;
            OgreNewt::CollisionPtr coll = createCollision(mPhysicalObject, inertia, centerOfMass);

            OgreNewt::Body* body = new OgreNewt::Body(PhysicsManager::getSingleton()._getNewtonWorld(), coll);
            body->setMaterialGroupID(PhysicsManager::getSingleton().getMaterialID("default"));

            Ogre::Real mass = mMass;
            if (mass > 0.0 && mGeometryType != GT_MESH)
            {
                body->setMassMatrix(mass, inertia);
                body->setCenterOfMass(centerOfMass);
            }

            body->setCustomForceAndTorqueCallback(PhysicsManager::genericForceCallback);

            setBody(body);

            if (mActor)
            {
                if (mActor->_getSceneNode())
                    _attachToSceneNode(mActor->_getSceneNode());
            }
        }
    }

    OgreNewt::CollisionPtr PhysicalThing::createCollision(
        PhysicalObject* po, Vector3& inertia, Vector3& centerOfMass) const
    {
        OgreNewt::CollisionPtr coll;

        // there is a difference between a meshobject and a 'normal' object
        // because a meshobject has got a mesh entity and therefore a it is
        // likely that there will be more than one object with those collision
        // primitives, so they need to get cached.

        if (mPhysicalObject->isMeshObject())
        {
            Entity* entity = static_cast<MeshObject*>(mPhysicalObject)->getEntity();
            coll = PhysicsManager::getSingleton().createCollision(
                entity, mGeometryType, "", Vector3::ZERO, Quaternion::IDENTITY, mMass, &inertia, &centerOfMass);
        }
        else
        {
            const AxisAlignedBox& aabb = mPhysicalObject->getDefaultSize();
            coll = PhysicsManager::getSingleton().createCollision("", // @todo: can we use a name here?
                aabb, mGeometryType, Vector3::ZERO, Quaternion::IDENTITY, mMass, &inertia, &centerOfMass);
        }

        return coll;
    }

    void PhysicalThing::updatePhysicsProxy()
    {
        if (mBody && !mRagDoll)
        {
            mPoseCollisions.clear();

            Vector3 inertia, centerOfMass;

            // update the collision
            mBody->setCollision(createCollision(mPhysicalObject, inertia, centerOfMass));
            if (mMass > 0.0 && mGeometryType != GT_MESH)
            {
                mBody->setMassMatrix(mMass, inertia);
                mBody->setCenterOfMass(centerOfMass);
            }
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
            mBody->setCustomForceAndTorqueCallback(PhysicsManager::genericForceCallback);
            setUpConstraint(Vector3::ZERO);
            mPhysicsController = NULL;
        }

        if (controller)
        {
            // prepare for control
            mPhysicsController = controller;
            // mBody->setAutoFreeze(0);
            mBody->unFreeze();
            mBody->setLinearDamping(0.0f);
            mBody->setAngularDamping(Vector3::ZERO);

            mBody->setCustomForceAndTorqueCallback(PhysicsManager::controlledForceCallback);

            // Set up-vector, so force application doesn't let the char fall over
            setUpConstraint(Vector3::UNIT_Y);
        }
    }

    void PhysicalThing::setMaterialID(const OgreNewt::MaterialID* materialid)
    {
        if (mRagDoll)
            mRagDoll->setMaterialID(materialid);
        else
            mBody->setMaterialGroupID(materialid);
    }

    const OgreNewt::MaterialID* PhysicalThing::getMaterialID() const
    {
        return mBody->getMaterialGroupID();
    }

    void PhysicalThing::createPhysicsProxy_RagDoll()
    {
        if (!mRagDoll)
        {
            if (mBody)
            {
                Throw(OperationNotSupportedException,
                    "PhysicalThing::createPhysicsProxy_RagDoll: there is already a body connected to this "
                    "PhysicalThing!");
            }

            if (!mPhysicalObject->isMeshObject())
                Throw(IllegalArgumentException,
                    "PhysicalThing::createPhysicsProxy_RagDoll needs a Mesh-Actor as argument!");
            MeshObject* meshObj = static_cast<MeshObject*>(mPhysicalObject);

            String ragdollRes = meshObj->getMeshName();
            ragdollRes = ragdollRes.substr(0, ragdollRes.find(".mesh")) + "_ragdoll.xml";
            mRagDoll = new PhysicsRagDoll(ragdollRes, mActor);
            mBody = NULL;
            if (mRagDoll->getRootBone())
                mBody = mRagDoll->getRootBone()->getBody();

            if (!mBody)
            {
                delete mRagDoll;
                mRagDoll = NULL;
                Throw(NullPointerException, "Error creating the ragdoll!");
            }
        }
    }
}
