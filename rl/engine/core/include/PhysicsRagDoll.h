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

#ifndef __PhysicsRagdoll_H__
#define __PhysicsRagdoll_H__

#include <tinyxml2.h>

#include "CorePrerequisites.h"
#include "PhysicsManager.h"
#include "XmlProcessor.h"

#ifdef __APPLE__
#include <OgreNewt/OgreNewt.h>
#else
#include <OgreNewt.h>
#endif

namespace rl
{

    class Actor;

    /** PhysicsRagDoll creates a ragdoll for an (Mesh-)Actor in the world. All currently acting animations are stopped.
     * The Actor must already be placed in the scene.
     * @warnig starting any animation while it is controlled by the ragdoll will cause errors (strange deformation of
     * the mesh ingame)
     */
    class PhysicsRagDoll : private XmlProcessor
    {
    public:
        /**
         * Main bone class, it represents a single bone (rigid body) in the ragdoll and is linked to a bone in the
         * skeleton of the mesh
         */
        class RagBone
        {
        public:
            /** The type of the collision used for the bone, usually BS_CONVEXHULL
             */
            enum BoneShape
            {
                BS_BOX,
                BS_ELLIPSOID,
                BS_CYLINDER,
                BS_CAPSULE,
                BS_CONE,
                BS_CONVEXHULL
            };

            /** The constructor, creates a body for the bone, which is then controlled by this class (eg the orientation
             * of the bone will be set to the orientation of the body)
             * @param creator the parent ragdoll
             * @param world pointer to the OgreNewt-World
             * @param parent the parent bone, if set NULL, it is assumed, that this is the root bone
             * @param ogreBone the Ogre-Bone that'll be controlled by this RagBone
             * @param mesh only used to create a convexhull-collision
             * @param dir only used when creating an ellipsoid, a cylinder or a cone
             * @param shape the type of the collision to create
             * @param size this value has different meanings for the convexhull-collision and for the others:
             *          - box and ellipsoid: the size as expected
             *          - capsule, cone and cylinder: only size.x and size.y are used
             *          - convexhull: size.x is used as minimum weight of vertexes used to create the hull
             *  @param mass the mass of the bone
             */
            RagBone(PhysicsRagDoll* creator, OgreNewt::World* world, RagBone* parent, Ogre::Bone* ogreBone,
                Ogre::MeshPtr mesh, Ogre::Vector3 dir, RagBone::BoneShape shape, Ogre::Vector3 size, Ogre::Real mass,
                Actor* parentActor);

            /** the destructor
             */
            ~RagBone();

            //! get the newton body, can be NULL
            OgreNewt::Body* getBody()
            {
                return mBody;
            }

            //! set the limits (only used for the hinge-joint)
            void setLimits(Ogre::Real limit1, Ogre::Real limit2)
            {
                mLimit1 = limit1;
                mLimit2 = limit2;
            }

            //! get limit
            Ogre::Real getLimit1()
            {
                return mLimit1;
            }
            //! get limit
            Ogre::Real getLimit2()
            {
                return mLimit2;
            }

            //! get the ogre bone
            Ogre::Bone* getOgreBone()
            {
                return mOgreBone;
            }

            //! hinge callback function for newton
            static void _hingeCallback(OgreNewt::BasicJoints::Hinge* me);

            //! get the parent bone, can be NULL
            RagBone* getParent()
            {
                return mParent;
            }

            //! get the RagDoll
            PhysicsRagDoll* getParentRagDoll()
            {
                return mDoll;
            }

            //! set the orientation and the offset manually (used for the root bone)
            void setOffset(Ogre::Quaternion orient, Ogre::Vector3 pos)
            {
                mOffsetOrient = orient;
                mOffsetPos = pos;
            }

            //! get offset orientation (used for the root bone)
            Ogre::Quaternion getOffsetOrient()
            {
                return mOffsetOrient;
            }
            //! get offset position (used for the root bone)
            Ogre::Vector3 getOffsetPos()
            {
                return mOffsetPos;
            }

        private:
            //! create a convexhull from a mesh
            OgreNewt::ConvexCollisionPtr _makeConvexHull(
                OgreNewt::World* world, Ogre::MeshPtr mesh, Ogre::Real minWeight);

            //! the ragdoll to that this bone belongs
            PhysicsRagDoll* mDoll;

            //! the parent bone, can be NULL
            RagBone* mParent;

            //! the OgreNewt body
            OgreNewt::Body* mBody;

            //! the corresponding ogre bone
            Ogre::Bone* mOgreBone;

            //! limits.
            Ogre::Real mLimit1;
            //! limits.
            Ogre::Real mLimit2;

            //! the offset position for the root bone
            Ogre::Vector3 mOffsetPos;
            //! the offset orientation for the root bone
            Ogre::Quaternion mOffsetOrient;
        };

        /** constructor for the RagDoll, create it with new and delete it if you don't need it any more...
         * @param ragdollResource the name of the xml-file with the ragdoll definition
         * @param actor an Actor that is already attached to the scene and contains a MeshObject
         */
        PhysicsRagDoll(const Ogre::String& ragdollResource, Actor* actor);

        //! destructor
        ~PhysicsRagDoll();

        //! set a different main node
        void setSceneNode(Ogre::SceneNode* node)
        {
            mNode = node;
        }

        //! set the initial velocity and Omega
        void setInheritedVelOmega(const Ogre::Vector3& vel, const Ogre::Vector3& omega);
        void setPositionOrientation(const Ogre::Vector3& pos, const Ogre::Quaternion& orient);

        //! set the Material id
        void setMaterialID(const OgreNewt::MaterialID* materialid);

        //! get the body of the root bone
        RagBone* getRootBone();

        //! callback function for newton
        static void _placementCallback(
            OgreNewt::Body* me, const Ogre::Quaternion& orient, const Ogre::Vector3& pos, int threadindex);

    private:
        //! the type of the joint
        enum JointType
        {
            JT_BALLSOCKET,
            JT_HINGE
        };

        /** Function for adding one bone.
         * The parameters are passed to the constructor of the RagBone
         */
        RagBone* _addBone(OgreNewt::World* world, RagBone* parent, Ogre::Vector3 dir, RagBone::BoneShape shape,
            Ogre::Vector3 size, Ogre::Real mass, Ogre::Bone* ogrebone, Actor* parentActor);

        //! recursive function for creating bones and joints
        RagBone* _addAllBones(RagBone* parent, tinyxml2::XMLElement* boneElem, Actor* parentActor);

        //! add a joint between two bodies
        void _joinBones(PhysicsRagDoll::JointType type, RagBone* parent, RagBone* child, Ogre::Vector3 pos,
            Ogre::Vector3 pin, Ogre::Real limit1, Ogre::Real limit2, OgreNewt::World* world);

        // we need a multimap here, cause there could be multiple ragbones without bodies
        typedef std::multimap<OgreNewt::Body*, RagBone*> RagBoneMap;
        typedef std::multimap<OgreNewt::Body*, RagBone*>::iterator RagBoneMapIterator;

        //! a list with all bones
        RagBoneMap mRagBonesMap;

        //! the root bone
        RagBone* mRootBone;

        //! the scene node
        Ogre::SceneNode* mNode;

        //! the mesh for this character.
        Ogre::MeshPtr mMesh;

        //! the skeleton instance.
        Ogre::SkeletonInstance* mSkeleton;

        //! the OgreNewt world.
        OgreNewt::World* mWorld;
    };
}

#endif
