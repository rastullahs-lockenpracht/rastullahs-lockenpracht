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

#ifndef __PhysicalThing_H__
#define __PhysicalThing_H__

#include "CorePrerequisites.h"
#include "PhysicsContactListener.h"
#include "PhysicsManager.h"

#include <OgreNewt.h>

namespace rl {

	class Actor;
	class MeshObject;

	/** PhysicalThing contains the physical representation of an ActorControlledObject.
	 * It utilizes a default collision primitiv derived from the default mesh, but it
	 * can also handle keyframe animation states of the ActorControlledObject.
	 */
    class _RlCoreExport PhysicalThing
    {
    public:
		/** Constructor storing all passed values.
		 * It simply initializes all internal variables to meaningful values
		 * and stores the values passed by the parameters.
		 * @param geomType GeometryType defining collision primitiv type
		 *				   (box, sphere, etc.)
		 * @param po PhysicalObject that utilizes this PhysicalThing object
		 * @param mass Real containing the mass of the physical object in kg
		 * 
		 */
		PhysicalThing(
			GeometryType geomType,
			PhysicalObject* po, 
			Ogre::Real mass, 
			bool hullModifier = false);

        /** default virtual destructor
		 * It is needed otherwise SWIG is not happy. It frees up the allocated 
		 * OgreNewt::Body and unregisteres the object from PhysicsManager.
		 */
        virtual ~PhysicalThing();

		/** returns the position of the OgreNewt::Body object
		 */
        Ogre::Vector3 getPosition() const;
		/** sets the position of the OgreNewt::Body object
		 * Since the physic engine alters position, using this function is bad.
		 * Use addForce to manipulate the body instead.
		 * @param pos Ogre::Vector3 containing new position in physic world space
		 */
        void setPosition(const Ogre::Vector3& pos);
		/** sets the position of the OgreNewt::Body object
		 * Since the physic engine alters position, using this function is bad.
		 * Use addForce to manipulate the body instead.
		 * @param x Ogre::Real containing new x position in physic world space
		 * @param y Ogre::Real containing new y position in physic world space
		 * @param z Ogre::Real containing new z position in physic world space
		 */
        void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);

		/** returns the orientation of the OgreNewt::Body object
		 */
        Ogre::Quaternion getOrientation() const;
		/** sets the position of the OgreNewt::Body object
		 * Since the physic engine alters position, using this function is bad.
		 * Use addForce to manipulate the body instead.
		 * @param orientation Ogre::Quaternion containing new orientation in physic world space
		 */
        void setOrientation(const Ogre::Quaternion& orienation);
		/** sets the position of the OgreNewt::Body object
		 * Since the physic engine alters position, using this function is bad.
		 * Use addForce to manipulate the body instead.
		 * @param w Ogre::Real containing new w orientation in physic world space
		 * @param x Ogre::Real containing new x orientation in physic world space
		 * @param y Ogre::Real containing new y orientation in physic world space
		 * @param z Ogre::Real containing new z orientation in physic world space
		 */
        void setOrientation(Ogre::Real w, Ogre::Real x, Ogre::Real y, Ogre::Real z);

        void setVelocity(const Ogre::Vector3& velocity);
        Ogre::Vector3 getVelocity();

        // Sets the vector, that will always point up.
        void setUpConstraint(const Ogre::Vector3& upVector = Ogre::Vector3::UNIT_Y);
        Ogre::Vector3 getUpConstraint() const;
        void clearUpConstraint();

        // Sets whether to use default gravity, or override it with its own
        void setGravityOverride(bool override, const Ogre::Vector3& gravity);
        void setGravityOverride(bool override,
            Ogre::Real x = 0.0f, Ogre::Real y = 0.0f, Ogre::Real z = 0.0f);

        Actor* getActor() const;
		OgreNewt::Body* _getBody() const;
        
        void _update();
        void _setActor(Actor* actor);
        void _attachToSceneNode(Ogre::SceneNode* node);
		void _attachToBone(MeshObject* object, const std::string& boneName);
        void _detachFromSceneNode(Ogre::SceneNode* node);

		/** ForceCallback for mBody (OgreNewt::Body). */
        void onApplyForceAndTorque();
        void addForce(const Ogre::Vector3& force);

        void freeze();
        void unfreeze();

        Ogre::Real getMass() const;
        void setMass(Ogre::Real mass);

		void createPhysicsProxy(Ogre::SceneNode* node);
        void updatePhysicsProxy();

        /** Called to update the collision of the physical thing, in order to adapt
         *  to a new animation state.
         *  @warning This is only applicable to ConvexHullCollisions.
         *  @throw IllegalStateException, if PhysicalThing does not represent a ConvexHullCollision
         */
        void updateCollisionHull();

        /** Called to update the collision primitiv to a new animation state.
		 * Meshes can have different keyframe animations, which can vary in size and orientation.
		 * To update the collision primitiv from one such keyframe to another, this function can
		 * be used.
		 * @param name String containing name of animation pose to update to.
         */
        void fitToPose(const Ogre::String& name);

        void setContactListener(PhysicsContactListener* listener);
        PhysicsContactListener* getContactListener() const;

        /** Retrieves the PhysicsController of this object.
         * @returns the responsible PhysicsController object reacting on force/torque updates
         */
        PhysicsController* getPhysicsController() const;
        /** Sets the PhysicsController for this object.
         * When the given controller object is NULL, then this removes only
         * removes the previous controller object.
         * @param controller is the responsible PhysicsController object reacting on force/torque updates
         * @param materialid of the material to use for this physical object
         */
        void setPhysicsController(PhysicsController* controller);

        /** Sets the material id for this OgreNewt Body.
         * @param materialid to set to.
         */
        void setMaterialID(const OgreNewt::MaterialID* materialid);
        /** Retrieves the material id for the OgreNewt Body.
         * @returns materialid of the OgreNewt body.
         */
        const OgreNewt::MaterialID* getMaterialID() const;

    protected:
		void prepareUserControl(OgreNewt::MaterialID* material);
        void unprepareUserControl();

    private:
        Actor* mActor;
		//! The newton body object this physical thing works with
        OgreNewt::Body* mBody;
		//! an upjoint to keep the body from falling over
        OgreNewt::BasicJoints::UpVector* mUpVectorJoint;
		//! the pending force to apply at the body
        Ogre::Vector3 mPendingForce;
		/** whether we override gravity or not
		 question: is this really necessary ?
		 */
        bool mOverrideGravity;
		/** gravity vector ?
		 question: is this really necessary ?
		 */
        Ogre::Vector3 mGravity;
		//! a contact listener called on collision with other objects
        PhysicsContactListener* mContactListener;

		//! typedefinition for easing variable declaration
        typedef std::map<Ogre::String, OgreNewt::CollisionPtr> CollisionMap;
		//! ??
        CollisionMap mPoseCollisions;

		//! the objects mass
		Ogre::Real mMass;
		//! the collision primitivs type (box, sphere, ellipsoid, etc.)
		GeometryType mGeometryType;
		//! the associated tangible object ?
		PhysicalObject* mPhysicalObject;
		bool mHullModifier;

        //! the responsible PhysicsController object reacting on force/torque updates
        PhysicsController* mPhysicsController;

		/** returns the geometrytype of the collision primitiv.
		 * That is normally box, sphere, ellipsoid, capsule, convexhull or mesh.
		 * question: is mesh also userdefined ?
		 */
		GeometryType getGeometryType() const;
		void setBody(OgreNewt::Body* body);

        OgreNewt::CollisionPtr createCollision(PhysicalObject* po, Ogre::Vector3& inertia) const;
	};
}

#endif
