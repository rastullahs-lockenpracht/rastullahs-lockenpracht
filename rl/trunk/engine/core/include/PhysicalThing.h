#ifndef __PhysicalThing_H__
#define __PhysicalThing_H__

#include "CorePrerequisites.h"

class dSpace;
class dBody;
class dGeom;

struct dxSpace;		
struct dxBody;		
struct dxGeom;		

typedef struct dxSpace *dSpaceID;
typedef struct dxBody *dBodyID;
typedef struct dxGeom *dGeomID;

namespace rl {

    class Actor;

    class _RlCoreExport PhysicalThing
    {
    public:
        PhysicalThing( dSpace* space, Actor* actor );
        ~PhysicalThing( void );

        // Called by the Actor
        void setPosition( Real x, Real y, Real z );
        void setOrientation( Real w, Real x, Real y, Real z );

        // Getters...
        Actor* getActor( void );
        dBodyID getBodyID( void );
        dGeomID getGeomID( void );
        dSpace* getSpace( void );
        void setSpace( dSpace* space );

        bool isColliding( void );
        void setColliding( bool collide );
        bool isDynamic( void );
        void setDynamic( bool dynamic );

        void stopDynamics();

        // Synchronize with Actor
        void update( void );

        // Forces
        void addForce(const Vector3& direction);
        void addForce(Real dir_x, Real dir_y, Real dir_z);
        void addForceWorldSpace(const Vector3& direction);
        void addForceWorldSpace(Real dir_x, Real dir_y, Real dir_z);
        void addForce(const Vector3& direction, const Vector3& atPosition);
        void addForce(Real dir_x, Real dir_y, Real dir_z,Real pos_x, Real pos_y, Real pos_z);
        void addForceWorldSpace(const Vector3& direction, const Vector3& atPosition);
        void addForceWorldSpace(Real dir_x, Real dir_y, Real dir_z,Real pos_x, Real pos_y, Real pos_z);
        void addTorque(const Vector3& direction);
        void addTorque(Real x, Real y, Real z);
        void addTorqueWorldSpace(const Vector3& direction);
        void addTorqueWorldSpace(Real x, Real y, Real z);

        bool testCollide(SceneQuery::WorldFragment* wf);
        bool testCollide(PhysicalThing* thing);

        // Geometry ( for Collision )
        void createCappedCylinderGeometry( Real radius, Real length, const Vector3& position = Vector3::ZERO, const Quaternion& orientation = Quaternion::IDENTITY );
        void createBoxGeometry( const Vector3& length, const Vector3& position = Vector3::ZERO, const Quaternion& orientation = Quaternion::IDENTITY );
        void createSphereGeometry( Real radius, const Vector3& position = Vector3::ZERO, const Quaternion& orientation = Quaternion::IDENTITY );
        // TODO TriMeshGeometry

        // Mass ( for Dynamics )
        void createCappedCylinderMass( Real density, Real radius, Real length, const Vector3& position = Vector3::ZERO, const Quaternion& orientation = Quaternion::IDENTITY );
        void createBoxMass( Real density, const Vector3& length, const Vector3& position = Vector3::ZERO, const Quaternion& orientation = Quaternion::IDENTITY );
        void createSphereMass( Real density, Real radius, const Vector3& position = Vector3::ZERO, const Quaternion& orientation = Quaternion::IDENTITY );
        // TODO TriMeshMass


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
        const Vector3& getLinearVelocity(void);
        /** Gets the current angular velocity of this object.
        * @remarks
        *  Only applicable if dynamics are enabled for this object.
        * @returns Vector3 representing the angular velocity in units per second around each axis.
        */
        const Vector3& getAngularVelocity(void);
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
        // Nifty physics attributes
        Real mFriction;
        Real mBounceRestitution;
        Real mBounceVelocityThresh;
        Real mSoftness;
        Real mSoftErp;

        bool mDynamic;
        bool mColliding;

        bool testPlaneBoundsCollide(std::list<Plane>* planes);
        // needed for Quaternion to dMatrix3 Conversion
        void setGeomRotation( const Quaternion& q );
        void setMassRotation( const Quaternion& q );

        // For Collision
        dGeom* mGeom;
        // For Dynamics
        dBody* mBody;
        // For Grouping
        dSpace* mSpace;
        // Assigned Actor
        Actor* mActor;    
    };

}


#endif
