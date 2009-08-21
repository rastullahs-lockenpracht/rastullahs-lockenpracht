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
    //! state of the player
    enum PlayerState
    {
        PS_ONLAND,
        PS_ONFREEFALL,
        PS_ONILLEGALRAMP
    };

    typedef std::vector<OgreNewt::Body*> BodyVector;

    //! constructor
    /*!
     * \param localFrame this is the player frame in global Space (x: side, y: up, -z forward)
    */
    PlayerController(OgreNewt::Body* child, Ogre::Real stairHeight, Ogre::Real kinematicCushion = 1.0f/64.0f, const Ogre::Matrix3& localFrame = Ogre::Matrix3::IDENTITY);
    virtual ~PlayerController();

    //! get controlled body
    OgreNewt::Body* getControlledBody() const {return m_body;}

    //! set the characters velocity, the -Speed-values can be negative, sideSpeed positiv means move to the right, heading is in absolute space
    void setVelocity(Ogre::Real forwardSpeed, Ogre::Real sideSpeed, Ogre::Radian heading);

    //! get currently set velocity
    void getVelocity(Ogre::Real &forwardSpeed, Ogre::Real& sideSpeed, Ogre::Radian& heading) const;

    //! set maximum step height (as absolute value)
    void setStairHeight(Ogre::Real stairHeight);

    //! get currently set maximum step height
    Ogre::Real getStairHeight() const;

    //! set kinematic cushion
    void setKinematicCushion(Ogre::Real cushion);

    //! get currently set kinematic cushioin
    Ogre::Real getKinematicCushion() const;

    //! set maximum slope
    void setMaxSlope(Ogre::Radian maxSlope);

    //! get currently set maximum slope
    Ogre::Radian getMaxSlope() const;

    //! set restitution (usually not needed!)
    void setRestitution(Ogre::Real restitution);

    //! get currently set restitution
    Ogre::Real getRestitution() const;

    //! retrieve player state
    PlayerState getPlayerState() const;

    //! set player state, use this only in order to reset the player
    void setPlayerState(PlayerState state);

    //! overwritten function from CustomJoint, must *NOT* be overwritten
    virtual void submitConstraint( Ogre::Real timestep, int threadindex );

    //! overwritten function from CustomJoint, must *NOT* be overwritten
    virtual void feedbackCollector( Ogre::Real timestep, int threadindex );

    //! udate player controller geometry, call this if the shape of the controlled body has changed...
    void updateGeometry();

protected:
    OgreNewt::Body *m_body; // we need this again, because in the joint it's constant!

    Ogre::Matrix4 m_localFrame;
    Ogre::Real m_forwardSpeed, m_sideSpeed, m_stairHeight, m_restitution, m_kinematicCushion, m_maxRadius;
    Ogre::Radian m_heading, m_maxSlope;

    PlayerState m_playerState;
    Ogre::Vector3 m_gravity;
    OgreNewt::ConvexCollisionPtr m_bodySensorShape;
    OgreNewt::ConvexCollisionPtr m_stairSensorShape;
    OgreNewt::ConvexCollisionPtr m_bodyFloorSensorShape;

    virtual bool convexStaticCastPreFilter(OgreNewt::Body *body, BodyVector filterBodies);
    virtual bool convexAllBodyCastPreFilter(OgreNewt::Body *body, BodyVector filterBodies);

    // parameters for the updateSenesorShapes-function, these are set in the constructor!
    Ogre::Real m_staticRadiusFactor, m_dynamicRadiusFactor, m_floorFinderRadiusFactor, m_maxPlayerHeightPaddFactor;


    // other parameters
    int m_sensorShapeSegments;
    int m_maxCollisionsIteration;
    int m_maxContactsCount;

    // variables used in submitConstraint
    Ogre::Quaternion m_lastOri;


    // kinematic motion: (called from feedbackCollector)
    void playerOnLand( Ogre::Real timestep, int threadindex );
    void playerOnFreeFall( Ogre::Real timestep, int threadindex );
    void playerOnIllegalRamp( Ogre::Real timestep, int threadindex );

    // calculate velocity (see if the player can move there etc)
    Ogre::Vector3 calculateVelocity(const Ogre::Vector3& targetVel, Ogre::Real timestep, const Ogre::Vector3& upDir, Ogre::Real elevation, int threadindex);
    

private:
    // we need overloaded versions of the raycasts...
    class StaticConvexCast : public OgreNewt::BasicConvexcast
    {
        public:
            StaticConvexCast(PlayerController *pc) : m_parent(pc) {}

            void go(const OgreNewt::ConvexCollisionPtr& col, const Ogre::Vector3& startpt,
                    const Ogre::Quaternion &colori, const Ogre::Vector3& endpt, int maxcontactscount, int thread, BodyVector filterBodies = BodyVector())
            {
                m_filterBodies = filterBodies;
                go(m_parent->getControlledBody()->getWorld(), col, startpt, colori, endpt, maxcontactscount, thread);
            }

            virtual bool userPreFilterCallback(OgreNewt::Body* body) {return m_parent->convexStaticCastPreFilter(body, m_filterBodies);}
        private:
            OgreNewt::PlayerController* m_parent;
            BodyVector m_filterBodies;
            using BasicConvexcast::go;
    };

    class AllBodyConvexCast : public OgreNewt::BasicConvexcast
    {
        public:
            AllBodyConvexCast(PlayerController *pc) : m_parent(pc) {}

            void go(const OgreNewt::ConvexCollisionPtr& col, const Ogre::Vector3& startpt,
                    const Ogre::Quaternion &colori, const Ogre::Vector3& endpt, int maxcontactscount, int thread, BodyVector filterBodies = BodyVector())
            {
                m_filterBodies = filterBodies;
                go(m_parent->getControlledBody()->getWorld(), col, startpt, colori, endpt, maxcontactscount, thread);
            }

            virtual bool userPreFilterCallback(OgreNewt::Body* body) {return m_parent->convexAllBodyCastPreFilter(body, m_filterBodies);}
        private:
            OgreNewt::PlayerController* m_parent;
            BodyVector m_filterBodies;
            using BasicConvexcast::go;
    };
};


}   // end NAMESPACE OgreNewt


#endif  // _INCLUDE_OGRENEWT_PLAYERCONTROLLER

