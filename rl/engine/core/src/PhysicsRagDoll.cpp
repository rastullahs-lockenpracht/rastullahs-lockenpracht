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

#include "Actor.h"
#include "MeshObject.h"
#include "PhysicalThing.h"
#include "PhysicsRagDoll.h"

#include <sstream>

namespace rl
{
    PhysicsRagDoll::RagBone::RagBone(PhysicsRagDoll* creator, OgreNewt::World* world, PhysicsRagDoll::RagBone* parent,
        Ogre::Bone* ogreBone, Ogre::MeshPtr mesh, Ogre::Vector3 dir, PhysicsRagDoll::RagBone::BoneShape shape,
        Ogre::Vector3 size, Ogre::Real mass, Actor* parentActor)
    {
        mDoll = creator;
        mParent = parent;
        mOgreBone = ogreBone;

        OgreNewt::ConvexCollisionPtr col;

        // in the case of the cylindrical primitives, they need to be rotated to align the main axis with the direction
        // vector.
        Ogre::Quaternion orient = Ogre::Quaternion::IDENTITY;
        Ogre::Vector3 pos = Ogre::Vector3::ZERO;
        Ogre::Matrix3 rot;

        if (dir == Ogre::Vector3::UNIT_Y)
        {
            rot.FromEulerAnglesXYZ(Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(90));
            orient.FromRotationMatrix(rot);
        }

        if (dir == Ogre::Vector3::UNIT_Z)
        {
            rot.FromEulerAnglesXYZ(Ogre::Degree(0), Ogre::Degree(90), Ogre::Degree(0));
            orient.FromRotationMatrix(rot);
        }

        // make the rigid body.
        switch (shape)
        {
        case PhysicsRagDoll::RagBone::BS_BOX:
            col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box(world, size, 0));
            break;

        case PhysicsRagDoll::RagBone::BS_CAPSULE:
            col = OgreNewt::ConvexCollisionPtr(
                new OgreNewt::CollisionPrimitives::Capsule(world, size.y, size.x, 0, orient, pos));
            break;

        case PhysicsRagDoll::RagBone::BS_CONE:
            col = OgreNewt::ConvexCollisionPtr(
                new OgreNewt::CollisionPrimitives::Cone(world, size.y, size.x, 0, orient, pos));
            break;

        case PhysicsRagDoll::RagBone::BS_CYLINDER:
            col = OgreNewt::ConvexCollisionPtr(
                new OgreNewt::CollisionPrimitives::Cylinder(world, size.y, size.x, 0, orient, pos));
            break;

        case PhysicsRagDoll::RagBone::BS_ELLIPSOID:
            col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(world, size, 0));
            break;

        case PhysicsRagDoll::RagBone::BS_CONVEXHULL:
            col = _makeConvexHull(world, mesh, size.x);
            break;

        default:
            col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box(world, size, 0));
            break;
        }

        if (col)
        {
            if (col->getNewtonCollision() == NULL)
            {
                col.reset();
            }
        }

        if (!col)
        {
            LOG_WARNING(Logger::CORE, " error creating collision for '" + ogreBone->getName() + "', still continuing.");
            mBody = NULL;
        }
        else
        {
            mBody = new OgreNewt::Body(world, col);
            mBody->setUserData(Ogre::Any(parentActor));
            mBody->setStandardForceCallback();
            const OgreNewt::MaterialID* ragdollMat = PhysicsManager::getSingleton().createMaterialID("default");
            mBody->setMaterialGroupID(ragdollMat);

            Ogre::Vector3 inertia;
            Ogre::Vector3 com;
            col->calculateInertialMatrix(inertia, com);

            mBody->setMassMatrix(mass, inertia * mass);
            mBody->setCenterOfMass(com);

            mBody->setCustomTransformCallback(PhysicsRagDoll::_placementCallback);

            mOgreBone->setManuallyControlled(true);
        }
    }

    PhysicsRagDoll::RagBone::~RagBone()
    {
        mOgreBone->setManuallyControlled(false);
        if (mBody)
            delete mBody;
    }

    void PhysicsRagDoll::RagBone::_hingeCallback(OgreNewt::BasicJoints::Hinge* me)
    {
        PhysicsRagDoll::RagBone* bone = Ogre::any_cast<PhysicsRagDoll::RagBone*>(me->getUserData());

        Ogre::Degree angle = me->getJointAngle();
        Ogre::Degree lim1(bone->getLimit1());
        Ogre::Degree lim2(bone->getLimit2());

        if (angle < lim1)
        {
            Ogre::Real accel = me->calculateStopAlpha(lim1);
            me->setCallbackAccel(accel);
        }

        if (angle > lim2)
        {
            Ogre::Real accel = me->calculateStopAlpha(lim2);
            me->setCallbackAccel(accel);
        }
    }

    OgreNewt::ConvexCollisionPtr PhysicsRagDoll::RagBone::_makeConvexHull(
        OgreNewt::World* world, Ogre::MeshPtr mesh, Ogre::Real minWeight)
    {
        std::vector<Ogre::Vector3> vertexVector;

        // for this bone, gather all of the vertices linked to it, and make an individual convex hull.
        std::string boneName = mOgreBone->getName();
        unsigned int boneIndex = mOgreBone->getHandle();

        Ogre::Matrix4 invMatrix;
        invMatrix.makeInverseTransform(-mOgreBone->_getBindingPoseInversePosition(),
            Ogre::Vector3::UNIT_SCALE / mOgreBone->_getBindingPoseInverseScale(),
            mOgreBone->_getBindingPoseInverseOrientation().Inverse());

        unsigned int num_sub = mesh->getNumSubMeshes();

        for (unsigned int i = 0; i < num_sub; i++)
        {
            Ogre::SubMesh* submesh = mesh->getSubMesh(i);
            Ogre::SubMesh::BoneAssignmentIterator bai = submesh->getBoneAssignmentIterator();

            Ogre::VertexDeclaration* v_decl;
            const Ogre::VertexElement* p_elem;
            float* v_Posptr;
            size_t v_count;
            Ogre::VertexData* v_data = NULL;

            if (submesh->useSharedVertices)
            {
                v_data = mesh->sharedVertexData;
                v_count = v_data->vertexCount;
                v_decl = v_data->vertexDeclaration;
                p_elem = v_decl->findElementBySemantic(Ogre::VES_POSITION);
            }
            else
            {
                v_data = submesh->vertexData;
                v_count = v_data->vertexCount;
                v_decl = v_data->vertexDeclaration;
                p_elem = v_decl->findElementBySemantic(Ogre::VES_POSITION);
            }

            size_t start = v_data->vertexStart;
            // pointer
            Ogre::HardwareVertexBufferSharedPtr v_sptr = v_data->vertexBufferBinding->getBuffer(p_elem->getSource());
            unsigned char* v_ptr = static_cast<unsigned char*>(v_sptr->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            unsigned char* v_offset;

            while (bai.hasMoreElements())
            {
                Ogre::VertexBoneAssignment vba = bai.getNext();
                if (vba.boneIndex == boneIndex)
                {
                    // found a vertex that is attached to this bone.
                    if (vba.weight >= minWeight)
                    {
                        // get offset to Position data!
                        v_offset = v_ptr + (vba.vertexIndex * v_sptr->getVertexSize());
                        p_elem->baseVertexPointerToElement(v_offset, &v_Posptr);

                        Ogre::Vector3 vert;
                        vert.x = *v_Posptr;
                        v_Posptr++;
                        vert.y = *v_Posptr;
                        v_Posptr++;
                        vert.z = *v_Posptr;

                        // apply transformation in to local space.
                        vert = invMatrix * vert;

                        vertexVector.push_back(vert);

                        Ogre::LogManager::getSingletonPtr()->logMessage(
                            "  vertex found! id:" + Ogre::StringConverter::toString(vba.vertexIndex));
                    }
                }
            }

            v_sptr->unlock();
        }

        // okay, we have gathered all verts for this bone.  make a convex hull!
        unsigned int numVerts = vertexVector.size();
        Ogre::Vector3* verts = new Ogre::Vector3[numVerts];
        unsigned int j = 0;
        while (!vertexVector.empty())
        {
            verts[j] = vertexVector.back();
            vertexVector.pop_back();
            j++;
        }

        //////////////////////////////////////////////////////////////////////////////////
        OgreNewt::ConvexCollisionPtr col;
        if (numVerts > 0)
            col = OgreNewt::ConvexCollisionPtr(
                new OgreNewt::CollisionPrimitives::ConvexHull(world, verts, numVerts, 0));

        delete[] verts;

        return col;
    }

    PhysicsRagDoll::PhysicsRagDoll(const Ogre::String& ragdollResource, Actor* actor)
        : mRootBone(NULL)
        , mNode(NULL)
        , mWorld(NULL)
        , mMesh()
        , mSkeleton(NULL)
    {
        mRootBone = NULL;
        mNode = actor->_getSceneNode();
        mWorld = PhysicsManager::getSingleton()._getNewtonWorld();
        if (!actor->getControlledObject()->isMeshObject())
            Throw(IllegalArgumentException, "PhysicsRagDoll::PhysicsRagDoll needs a Mesh-Actor as argument!");

        MeshObject* meshObj = static_cast<MeshObject*>(actor->getControlledObject());

        // stop all animations
        meshObj->stopAllAnimations();

        // get the skeleton.
        mSkeleton = meshObj->getEntity()->getSkeleton();

        // get the mesh.
        mMesh = meshObj->getEntity()->getMesh();

        LOG_MESSAGE(Logger::CORE, "Loading ragdoll '" + ragdollResource + "'");

        tinyxml2::XMLDocument* doc = loadDocument(ragdollResource);
        if (!doc)
        {
            LOG_ERROR(Logger::CORE, "Ragdoll resource '" + ragdollResource + "' not found");
        }
        else
        {

            tinyxml2::XMLElement* dataDocumentContent = doc->RootElement();
            mRootBone = _addAllBones(NULL, getChildNamed(dataDocumentContent, "Bone"), actor);
        }
    }

    PhysicsRagDoll::RagBone* PhysicsRagDoll::_addAllBones(
        PhysicsRagDoll::RagBone* parent, tinyxml2::XMLElement* boneElement, Actor* parentActor)
    {
        // get the information for the bone represented by this element.
        Ogre::Vector3 dir = getAttributeValueAsVector3(boneElement, "dir");
        Ogre::Real length = getAttributeValueAsReal(boneElement, "length");

        Ogre::Vector3 size = getAttributeValueAsVector3(boneElement, "size");

        Ogre::String skeleton_bone = getAttributeValueAsStdString(boneElement, "skeleton_bone");
        Ogre::Bone* ogrebone = mSkeleton->getBone(skeleton_bone);

        Ogre::String shapestr = getAttributeValueAsStdString(boneElement, "shape");
        PhysicsRagDoll::RagBone::BoneShape shape = PhysicsRagDoll::RagBone::BS_BOX;

        if (shapestr == "box")
            shape = PhysicsRagDoll::RagBone::BS_BOX;

        if (shapestr == "capsule")
            shape = PhysicsRagDoll::RagBone::BS_CAPSULE;

        if (shapestr == "cylinder")
            shape = PhysicsRagDoll::RagBone::BS_CYLINDER;

        if (shapestr == "cone")
            shape = PhysicsRagDoll::RagBone::BS_CONE;

        if (shapestr == "ellipsoid")
            shape = PhysicsRagDoll::RagBone::BS_ELLIPSOID;

        if (shapestr == "hull")
            shape = PhysicsRagDoll::RagBone::BS_CONVEXHULL;

        Ogre::Real mass = getAttributeValueAsReal(boneElement, "mass");

        ///////////////////////////////////////////////////////////////////////////////
        RagBone* me = _addBone(mWorld, parent, dir, shape, size, mass, ogrebone, parentActor);
        ///////////////////////////////////////////////////////////////////////////////

        // position the bone.
        Ogre::Quaternion boneorient = mNode->_getDerivedOrientation() * ogrebone->_getDerivedOrientation();
        Ogre::Vector3 bonepos;
        if (shape != PhysicsRagDoll::RagBone::BS_CONVEXHULL)
            bonepos
                = mNode->_getFullTransform() * ogrebone->_getDerivedPosition() + (boneorient * (dir * (length * 0.5f)));
        else
            bonepos = mNode->_getFullTransform() * ogrebone->_getDerivedPosition();

        if (me->getBody())
            me->getBody()->setPositionOrientation(bonepos, boneorient);

        // set offsets
        if (!parent)
        {
            Ogre::Quaternion offsetorient = (boneorient.Inverse()) * mNode->_getDerivedOrientation();
            Ogre::Vector3 offsetpos = boneorient.Inverse() * (mNode->_getDerivedPosition() - bonepos);
            me->setOffset(offsetorient, offsetpos);
        }

        // get the joint to connect this bone with it's parent.
        if (parent && me->getBody())
        {
            tinyxml2::XMLElement* jointElement = getChildNamed(boneElement, "Joint");
            if (!jointElement)
            {
                // error!
                LOG_ERROR(Logger::CORE, " Joint not found while creating Ragdoll! ");
                return me;
            }

            Ogre::Vector3 jointpin = getAttributeValueAsVector3(jointElement, "pin");
            Ogre::String jointtypestr = getAttributeValueAsStdString(jointElement, "type");
            PhysicsRagDoll::JointType jointtype = PhysicsRagDoll::JT_BALLSOCKET;

            if (jointtypestr == "ballsocket")
                jointtype = PhysicsRagDoll::JT_BALLSOCKET;

            if (jointtypestr == "hinge")
                jointtype = PhysicsRagDoll::JT_HINGE;

            Ogre::Real limit1 = getAttributeValueAsReal(jointElement, "limit1");
            Ogre::Real limit2 = getAttributeValueAsReal(jointElement, "limit2");

            Ogre::Vector3 jpos = mNode->_getFullTransform() * ogrebone->_getDerivedPosition();
            Ogre::Vector3 jpin
                = (mNode->_getDerivedOrientation() * parent->getOgreBone()->_getDerivedOrientation()) * jointpin;

            _joinBones(jointtype, parent, me, jpos, jpin, limit1, limit2, mWorld);
        }

        LOG_MESSAGE(Logger::CORE, " added bone from '" + ogrebone->getName() + "'.");

        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////
        // add all children of this bone.
        for (tinyxml2::XMLNode* cur = boneElement->FirstChild(); cur; cur = cur->NextSibling())
        {
            tinyxml2::XMLElement* elem = cur->ToElement();
            if (elem && hasNodeName(elem, "Bone"))
            {
                _addAllBones(me, elem, parentActor);
            }
        }

        return me;
    }

    PhysicsRagDoll::~PhysicsRagDoll()
    {
        for (RagBoneMapIterator it = mRagBonesMap.begin(); it != mRagBonesMap.end(); it++)
        {
            RagBone* bone = it->second;
            delete bone;
        }
    }

    PhysicsRagDoll::RagBone* PhysicsRagDoll::_addBone(OgreNewt::World* world, PhysicsRagDoll::RagBone* parent,
        Ogre::Vector3 dir, RagBone::BoneShape shape, Ogre::Vector3 size, Ogre::Real mass, Ogre::Bone* ogrebone,
        Actor* parentActor)
    {
        PhysicsRagDoll::RagBone* bone
            = new PhysicsRagDoll::RagBone(this, world, parent, ogrebone, mMesh, dir, shape, size, mass, parentActor);

        mRagBonesMap.insert(std::make_pair(bone->getBody(), bone));

        return bone;
    }

    void PhysicsRagDoll::_joinBones(PhysicsRagDoll::JointType type, RagBone* parent, RagBone* child, Ogre::Vector3 pos,
        Ogre::Vector3 pin, Ogre::Real limit1, Ogre::Real limit2, OgreNewt::World* world)
    {
        pin.normalise();
        OgreNewt::Joint* joint = NULL;

        switch (type)
        {
        case PhysicsRagDoll::JT_BALLSOCKET:
            joint = new OgreNewt::BasicJoints::BallAndSocket(world, child->getBody(), parent->getBody(), pos);
            ((OgreNewt::BasicJoints::BallAndSocket*)joint)->setLimits(pin, Ogre::Degree(limit1), Ogre::Degree(limit2));
            break;

        case PhysicsRagDoll::JT_HINGE:
            joint = new OgreNewt::BasicJoints::Hinge(world, child->getBody(), parent->getBody(), pos, pin);
            ((OgreNewt::BasicJoints::Hinge*)joint)->setCallback(RagBone::_hingeCallback);
            joint->setUserData(Ogre::Any(child));
            child->setLimits(limit1, limit2);
            break;
        }
    }

    void PhysicsRagDoll::_placementCallback(
        OgreNewt::Body* me, const Ogre::Quaternion& orient, const Ogre::Vector3& pos, int threadindex)
    {
        Actor* parentActor = Ogre::any_cast<Actor*>(me->getUserData());
        PhysicsRagDoll* doll = parentActor->getPhysicalThing()->getRagDoll();
        PhysicsRagDoll::RagBoneMapIterator it = doll->mRagBonesMap.find(me);
        if (it == doll->mRagBonesMap.end())
        {
            LOG_ERROR(Logger::CORE, "Could not find ragdoll-bone in PhysicsRagDoll::_placementCallback!");
            return;
        }
        PhysicsRagDoll::RagBone* bone = it->second;

        // is this the root bone?
        if (!bone->getParent())
        {
            Ogre::Quaternion finalorient = (orient * bone->getOffsetOrient());
            Ogre::Vector3 finalpos = pos + (orient * bone->getOffsetPos());

            doll->mNode->setPosition(finalpos);
            doll->mNode->setOrientation(finalorient);
        }
        else
        {
            // standard bone, calculate the local orientation between it and it's parent.
            Ogre::Quaternion parentorient;
            Ogre::Vector3 parentpos;

            if (bone->getParent()->getBody())
                bone->getParent()->getBody()->getPositionOrientation(parentpos, parentorient);
            else
                parentorient = bone->getParent()->getOgreBone()->getOrientation();

            Ogre::Quaternion localorient = orient;
            if (bone->getOgreBone()->getInheritOrientation())
                localorient = parentorient.Inverse() * localorient;

            bone->getOgreBone()->setOrientation(localorient);
        }
    }

    void PhysicsRagDoll::setInheritedVelOmega(const Ogre::Vector3& vel, const Ogre::Vector3& omega)
    {
        // find main position.
        Ogre::Vector3 mainpos = mNode->_getDerivedPosition();

        for (RagBoneMapIterator it = mRagBonesMap.begin(); it != mRagBonesMap.end(); it++)
        {
            Ogre::Vector3 pos;
            Ogre::Quaternion orient;

            if (it->second->getBody())
            {
                it->second->getBody()->getPositionOrientation(pos, orient);
                it->second->getBody()->setVelocity(vel + omega.crossProduct(pos - mainpos));
            }
        }
    }

    void PhysicsRagDoll::setPositionOrientation(const Ogre::Vector3& pos, const Ogre::Quaternion& orient)
    {
        Ogre::Vector3 oldPos = mNode->_getDerivedPosition();
        Ogre::Quaternion oldOri = mNode->_getDerivedOrientation();
        Ogre::Quaternion oldOriInv = oldOri.Inverse();

        for (RagBoneMapIterator it = mRagBonesMap.begin(); it != mRagBonesMap.end(); it++)
        {
            OgreNewt::Body* body = it->second->getBody();
            if (body)
            {
                Ogre::Vector3 boneOldPos;
                Ogre::Quaternion boneOldOri;
                body->getPositionOrientation(boneOldPos, boneOldOri);

                // get old position and orientation in local space
                Ogre::Vector3 boneOldLocalPos = oldOriInv * (boneOldPos - oldPos);
                Ogre::Quaternion boneOldLocalOri = oldOriInv * boneOldOri;

                // calculate and set new position in orientation
                body->setPositionOrientation(pos + orient * boneOldLocalPos, orient * boneOldLocalOri);
                body->unFreeze();
            }
        }
        mNode->setPosition(pos);
        mNode->setOrientation(orient);
    }

    void PhysicsRagDoll::setMaterialID(const OgreNewt::MaterialID* materialid)
    {
        for (RagBoneMapIterator it = mRagBonesMap.begin(); it != mRagBonesMap.end(); it++)
        {
            if (it->second->getBody())
                it->second->getBody()->setMaterialGroupID(materialid);
        }
    }

    PhysicsRagDoll::RagBone* PhysicsRagDoll::getRootBone()
    {
        return mRootBone;
    }
}
