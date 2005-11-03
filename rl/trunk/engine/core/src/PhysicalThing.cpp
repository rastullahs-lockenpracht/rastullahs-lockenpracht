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
#include "PhysicsManager.h"
#include "Exception.h"

using namespace OgreNewt;
using namespace OgreNewt::CollisionPrimitives;

namespace rl
{
    PhysicalThing::PhysicalThing(OgreNewt::Body* body, const Vector3& offset,
        const Ogre::Quaternion& orientationBias) :
        mActor(0),
        mBody(body),
        mUpVectorJoint(0),
        mOffset(offset),
        mOrientationBias(orientationBias),
        mPendingForce(Vector3::ZERO),
        mOverrideGravity(false),
        mGravity(Vector3::ZERO)
    {
    }

    PhysicalThing::~PhysicalThing()
    {
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

    Actor *PhysicalThing::getActor(void) const
    {
        return mActor;
    }

    OgreNewt::Body* PhysicalThing::_getBody() const
    {
        return mBody;
    }

    void PhysicalThing::_update()
    {
        mBody->setPositionOrientation(mActor->_getSceneNode()->getWorldPosition() + mOffset,
            mOrientationBias.Inverse() * mActor->_getSceneNode()->getWorldOrientation());
    }

    void PhysicalThing::_setActor(Actor* actor)
    {
        mActor = actor;
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
        mPendingForce += force;
    }

    Ogre::Real PhysicalThing::getMass() const
    {
        Real mass;
        Vector3 inertia;
        mBody->getMassMatrix(mass, inertia);
        return mass;
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

        Vector3 position;
        Quaternion orientation;
        mBody->getPositionOrientation(position, orientation);
        ConvexHull* collision = new ConvexHull(PhysicsManager::getSingleton()._getNewtonWorld(),
            &vertices[0], vertices.size());
        mBody->setCollision(collision);
        mBody->setPositionOrientation(position, orientation);
    }
}

