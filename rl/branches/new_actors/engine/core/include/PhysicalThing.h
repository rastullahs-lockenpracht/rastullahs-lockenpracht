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

#ifndef __PhysicalThing_H__
#define __PhysicalThing_H__

#include "CorePrerequisites.h"
#include <OgreOde.h>
namespace rl {

    class Actor;

    class _RlCoreExport PhysicalThing
    {
    public:
        PhysicalThing(OgreOde::Space* space, Actor* actor);
        ~PhysicalThing(void);

        // Called by the Actor
        void setPosition(Real x, Real y, Real z);
        void setOrientation(Real w, Real x, Real y, Real z);

        // Getters...
        Actor* getActor();
        OgreOde::Body* getBody();
        OgreOde::Geometry* getGeometry();
        OgreOde::Space* getSpace();
        void setSpace(OgreOde::Space* space);

        bool isDynamic();
        void setDynamic(bool dynamic);

        void stopDynamics();

        // Synchronize with Actor
        void update(void);

        // Forces
        void addForce(const Vector3& direction);
        void addForce(Real dir_x, Real dir_y, Real dir_z);
        void addForceWorldSpace(const Vector3& direction);
        void addForceWorldSpace(Real dir_x, Real dir_y, Real dir_z);
        void addForce(const Vector3& direction, const Vector3& atPosition);
        void addForce(Real dir_x, Real dir_y, Real dir_z,
            Real pos_x, Real pos_y, Real pos_z);
        void addForceWorldSpace(const Vector3& direction,
            const Vector3& atPosition);
        void addForceWorldSpace(Real dir_x, Real dir_y, Real dir_z,
            Real pos_x, Real pos_y, Real pos_z);
        void addTorque(const Vector3& direction);
        void addTorque(Real x, Real y, Real z);
        void addTorqueWorldSpace(const Vector3& direction);
        void addTorqueWorldSpace(Real x, Real y, Real z);

        bool testCollide(PhysicalThing* thing);

        // Geometry ( for Collision )
        void createCappedCylinderGeometry(Real radius, Real length,
            const Vector3& position = Vector3::ZERO,
            const Quaternion& orientation = Quaternion::IDENTITY );
        void createBoxGeometry( const Vector3& length,
            const Vector3& position = Vector3::ZERO,
            const Quaternion& orientation = Quaternion::IDENTITY );
        void createSphereGeometry( Real radius,
            const Vector3& position = Vector3::ZERO,
            const Quaternion& orientation = Quaternion::IDENTITY );

        // Mass ( for Dynamics )
        void createCappedCylinderMass(Real density, Real radius,
            Real length, const Vector3& position = Vector3::ZERO,
            const Quaternion& orientation = Quaternion::IDENTITY );
        void createBoxMass(Real density, const Vector3& length,
            const Vector3& position = Vector3::ZERO,
            const Quaternion& orientation = Quaternion::IDENTITY );
        void createSphereMass( Real density, Real radius,
            const Vector3& position = Vector3::ZERO,
            const Quaternion& orientation = Quaternion::IDENTITY );


        /** Sets the 'bounciness' of this object.
        * @remarks
        * Only applies if this object has both collision and dynamics enabled.
        * When 2 movable objects collide, the greatest bounce parameters 
        * from both objects apply, so even a non-bouncy object can
        * bounce if it hits a bouncy surface. 
        * @param restitutionValue Coeeficient of restitution 
        * 		(0 for no bounce, 1 for perfect bounciness)
        * @param velocityThreshold Velocity below which no bounce will occur; 
        * 		this is a dampening value to ensure small velocities do not
        * 		cause bounce.
        */
        void setBounceParameters(Real restitutionValue,	Real velocityThreshold);
        /** Gets the cefficient of restitution (bounciness) for this object. */
        Real getBounceRestitutionValue(void);
        /** Gets the bounce velocity threshold for this object. */
        Real getBounceVelocityThreshold(void);
        /** Sets the softness of this object, which determines how much it is allowed to 
        * penetrate other objects.
        * @remarks
        * 	This parameter only has meaning if collision and dynamics are enabled for this object.
        * 	@param softness Softness factor (0 is completely hard). Softness will be combined from
        * 		both objects involved in a collision to determine how much they will penetrate.
        */
        void setSoftness(Real softness);
        /** Gets the softness factor of this object. */
        Real getSoftness(void);
        /** Sets the Coulomb frictional coefficient for this object.
        * @remarks
        *  This coefficient affects how much an object will slip when it comes
        *  into contact with another object. 
        * @param friction The Coulomb friction coefficient, valid from 0 to Math::POS_INFINITY.
        *  0 means no friction, Math::POS_INFINITY means infinite friction ie no slippage.
        *  Note that friction between these 2 bounds is more CPU intensive so use with caution.
        */
        void setFriction(Real friction);
        /** Gets the Coulomb frictional coefficient for this object. */
        Real getFriction(void);
        void setSoftErp(Real erp);
        Real getSoftErp(void);


        // Speed

        /** Sets the current linear velocity of this object.
        * @remarks
        *  Only applicable if dynamics are enabled for this object. This method is useful
        *  for starting an object off at a particular speed rather than applying forces to get 
        *  it there.
        */ 
        void setLinearVelocity(const Vector3& vel);
        /** Sets the current linear velocity of this object.
        * @remarks
        *  Only applicable if dynamics are enabled for this object. This method is useful
        *  for starting an object off at a particular speed rather than applying forces to get 
        *  it there.
        */ 
        void setLinearVelocity(Real x, Real y, Real z);
        /** Gets the current linear velocity of this object.
        * @remarks
        *  Only applicable if dynamics are enabled for this object.
        * @returns Vector3 representing the velocity in units per second.
        */
        const Vector3 getLinearVelocity(void);
        /** Gets the current angular velocity of this object.
        * @remarks
        *  Only applicable if dynamics are enabled for this object.
        * @returns Vector3 representing the angular velocity in units per second around each axis.
        */
        const Vector3 getAngularVelocity(void);
        /** Sets the current angular velocity of this object
        * @remarks
        *  Only applicable if dynamics are enabled for this object. This method is useful
        *  for starting an object off rather than applying forces to get 
        *  it there.
        */ 
        void setAngularVelocity(const Vector3& vel);
        /** Sets the current angular velocity of this object.
        * @remarks
        *  Only applicable if dynamics are enabled for this object. This method is useful
        *  for starting an object off rather than applying forces to get 
        *  it there.
        */ 
        void setAngularVelocity(Real x, Real y, Real z);

    private:
        Real mBounceRestitution;
        Real mBounceVelocityThresh;
        Real mSoftness;
        Real mFriction;
        Real mSoftErp;
        
        // For Collision
        OgreOde::Geometry* mGeometry;
        // For Dynamics
        OgreOde::Body* mBody;
        // For Grouping
        OgreOde::Space* mSpace;
        // Assigned Actor
        Actor* mActor;    
    };

}


#endif
