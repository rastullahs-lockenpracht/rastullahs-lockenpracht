/* 
    OgreNewt Library

    Ogre implementation of Newton Game Dynamics SDK

    OgreNewt basically has no license, you may use any or all of the library however you desire... I hope it can help you in any way.

        by melven

*/
#ifndef _INCLUDE_OGRENEWT_PLAYERCONTROLLER
#define _INCLUDE_OGRENEWT_PLAYERCONTROLLER


#include "OgreNewt_Prerequisites.h"
#include "OgreNewt_Joint.h"
#include "OgreNewt_RayCast.h"
#include "OgreNewt_Body.h"


namespace OgreNewt
{

//! PlayerController
/*!
    this class implements a player-controller based on the code of the CustomPlayerController-class in the Newton-CustomJoints library
*/
class _OgreNewtExport PlayerController : public CustomJoint
{
public:
    //! constructor
    PlayerController(OgreNewt::Body* child);
    virtual ~PlayerController();

    //! get controlled body
    OgreNewt::Body* getControlledBody() const {return m_body;}

    //! set the characters velocity, the -Speed-values can be negative, sideSpeed positiv means move to the right, heading is in absolute space
    void setVelocity(Ogre::Real forwardSpeed, Ogre::Real sideSpeed, Ogre::Degree heading);

    //! get currently set velocity
    void getVelocity(Ogre::Real &forwardSpeed, Ogre::Real& sideSpeed, Ogre::Radian& heading) const;

    //! set maximum step height (as absolute value)
    void setMaxStepHeight(Ogre::Real maxStepHeight);

    //! get currently set maximum step height
    Ogre::Real getMaxStepHeight() const;

    //! set maximum slope
    void setMaxSlope(Ogre::Radian maxSlope);

    //! get currently set maximum slope
    Ogre::Radian getMaxSlope() const;

    //! set restitution (usually not needed!)
    void setRestitution(Ogre::Real restitution);

    //! get currently set restitution
    Ogre::Real getRestitution() const;

    // not supported yet
    // set up-vector pin
    //void setPinVector(Ogre::Vector3 upVector);
    // get currently set up-vector pin
    //Ogre::Vector3 getPinVector() const;

    //! is the body currently in the air (jumpin?)
    bool isInJumpState() const;

    //! can be overwritten to specify if a body can be pushed away by this character
    virtual bool canPushBody(const OgreNewt::Body *hitBody) const {return true;}

    //! overwritten function from CustomJoint, must *NOT* be overwritten
    virtual void submitConstraint( Ogre::Real timestep, int threadindex );

protected:
    OgreNewt::Body *m_body; // we need this again, because in the joint it's constant!

    Ogre::Real m_forwardSpeed, m_sideSpeed, m_maxStepHeight, m_restitution;
    Ogre::Radian m_heading, m_maxSlope;
    Ogre::Vector3 m_upVector;

    bool m_isInJumpState;
    OgreNewt::ConvexCollisionPtr m_verticalSensorShape;
    OgreNewt::ConvexCollisionPtr m_horizontalSensorShape;
    OgreNewt::ConvexCollisionPtr m_dynamicsSensorShape;

    virtual bool convexStaticCastPreFilter(OgreNewt::Body *body);
    virtual bool convexDynamicCastPreFilter(OgreNewt::Body *body);
    virtual bool convexAllBodyCastPreFilter(OgreNewt::Body *body);
    virtual bool findFloorCastPreFilter(OgreNewt::Body *body);

    // helper function, used internally
    void getPlayerHeightAndRadius(Ogre::Real &height, Ogre::Real &radius);

    // parameters for the updateSenesorShapes-function, these are set in the constructor!
    Ogre::Real m_staticRadiusFactor, m_dynamicRadiusFactor, m_floorFinderRadiusFactor, m_maxPlayerHeightPaddFactor;
    int m_sensorShapeSegments;
    // internal values, only used in updateSensorShapes!
    Ogre::Real m_lastPlayerHeight, m_lastPlayerRadius, m_lastSensorHeight;
    // helper function
    void updateSensorShapes();


    // other parameters
    int m_maxCollisionsIteration;
    int m_maxContactsCount;

    // variables used in submitConstraint
    Ogre::Quaternion m_lastOri;

private:
    // we need overloaded versions of the raycasts...
    class FindFloorRaycast : public OgreNewt::BasicRaycast
    {
        public:
            FindFloorRaycast(PlayerController *pc, const Ogre::Vector3& startpt, const Ogre::Vector3& endpt, bool sorted) :
                    BasicRaycast(pc->getControlledBody()->getWorld(), startpt, endpt, sorted),
                    m_parent(pc)
                {}

            virtual bool userPreFilterCallback(OgreNewt::Body* body) {return m_parent->findFloorCastPreFilter(body);}
        private:
            OgreNewt::PlayerController* m_parent;
    };

    class StaticConvexCast : public OgreNewt::BasicConvexcast
    {
        public:
            StaticConvexCast(PlayerController *pc) : m_parent(pc) {}

            void go(const OgreNewt::ConvexCollisionPtr& col, const Ogre::Vector3& startpt,
                    const Ogre::Quaternion &colori, const Ogre::Vector3& endpt, int maxcontactscount, int thread)
            {
                go(m_parent->getControlledBody()->getWorld(), col, startpt, colori, endpt, maxcontactscount, thread);
            }

            virtual bool userPreFilterCallback(OgreNewt::Body* body) {return m_parent->convexStaticCastPreFilter(body);}
        private:
            OgreNewt::PlayerController* m_parent;
            using BasicConvexcast::go;
    };

    class DynamicConvexCast : public OgreNewt::BasicConvexcast
    {
        public:
            DynamicConvexCast(PlayerController *pc) : m_parent(pc) {}

            void go(const OgreNewt::ConvexCollisionPtr& col, const Ogre::Vector3& startpt,
                    const Ogre::Quaternion &colori, const Ogre::Vector3& endpt, int maxcontactscount, int thread)
            {
                go(m_parent->getControlledBody()->getWorld(), col, startpt, colori, endpt, maxcontactscount, thread);
            }

            virtual bool userPreFilterCallback(OgreNewt::Body* body) {return m_parent->convexDynamicCastPreFilter(body);}
        private:
            OgreNewt::PlayerController* m_parent;
            using BasicConvexcast::go;
    };

    class AllBodyConvexCast : public OgreNewt::BasicConvexcast
    {
        public:
            AllBodyConvexCast(PlayerController *pc) : m_parent(pc) {}

            void go(const OgreNewt::ConvexCollisionPtr& col, const Ogre::Vector3& startpt,
                    const Ogre::Quaternion &colori, const Ogre::Vector3& endpt, int maxcontactscount, int thread)
            {
                go(m_parent->getControlledBody()->getWorld(), col, startpt, colori, endpt, maxcontactscount, thread);
            }

            virtual bool userPreFilterCallback(OgreNewt::Body* body) {return m_parent->convexAllBodyCastPreFilter(body);}
        private:
            OgreNewt::PlayerController* m_parent;
            using BasicConvexcast::go;
    };
};


}   // end NAMESPACE OgreNewt


#endif  // _INCLUDE_OGRENEWT_PLAYERCONTROLLER

