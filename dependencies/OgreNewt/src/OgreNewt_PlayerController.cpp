#include "OgreNewt_PlayerController.h"
#include "OgreNewt_Tools.h"
#include "OgreNewt_Collision.h"
#include "OgreNewt_CollisionPrimitives.h"
#include "OgreNewt_World.h"

#include <vector>
#include <iostream>

namespace OgreNewt
{

PlayerController::PlayerController(OgreNewt::Body * child, Ogre::Real stairHeight, Ogre::Real kinematicCushion, const Ogre::Matrix3& localFrame) :
    CustomJoint(6, child, NULL),
    m_body(child)
{
    // initialize some non settable parameters
    m_maxCollisionsIteration = 8;
    m_maxContactsCount = 16;
    m_sensorShapeSegments = 32;

#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
    m_bodySensorShape = NULL;
    m_stairSensorShape = NULL;
    m_bodyFloorSensorShape = NULL;
#endif


    // initialize settable values
    m_forwardSpeed = 0;
    m_sideSpeed = 0;
    m_heading = Ogre::Radian(0);
    m_restitution = 0;
    m_kinematicCushion = std::max(kinematicCushion, 1.0f/64.0f);
    m_maxSlope = Ogre::Degree(30);
    m_stairHeight = stairHeight;
    m_playerState = PS_ONFREEFALL;
    m_localFrame = localFrame;


    updateGeometry();

}


void PlayerController::updateGeometry()
{
    m_localFrame.setScale( Ogre::Vector3::UNIT_SCALE );
    m_localFrame.setTrans(  m_body->getCenterOfMass() );
    Ogre::AxisAlignedBox playerAabb = m_body->getCollision()->getAABB( m_localFrame.extractQuaternion() );
    Ogre::Real playerHeight = playerAabb.getSize().y;
    Ogre::Real playerRadius = std::max(playerAabb.getSize().x, playerAabb.getSize().z);
    
    m_maxRadius = playerRadius + m_kinematicCushion;


#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
    // delete old ones
    if( m_bodySensorShape )
        delete m_bodySensorShape;
    if( m_stairSensorShape )
        delete m_stairSensorShape;
    if( m_bodyFloorSensorShape )
        delete m_bodyFloorSensorShape;
#endif

    std::vector<Ogre::Vector3> bodySensorPoints, stairSensorPoints, bodyFloorSensorPoints;
    bodySensorPoints.resize(m_sensorShapeSegments*2);
    stairSensorPoints.resize(m_sensorShapeSegments*2);
    bodyFloorSensorPoints.resize(m_sensorShapeSegments*2);

    Ogre::Real h0, h1, startHeight;
    h0 = playerAabb.getMinimum().y;
    h1 = playerAabb.getMaximum().y - m_stairHeight;
    startHeight = h0 + m_stairHeight;


    Ogre::Quaternion localOri = m_localFrame.extractQuaternion();
    for( int i = 0; i < m_sensorShapeSegments; i++)
    {
        Ogre::Real x, z, fx, fz;
        x = Ogre::Math::Cos( Ogre::Degree( (360.0f * i) / m_sensorShapeSegments ) );
        z = Ogre::Math::Sin( Ogre::Degree( (360.0f * i) / m_sensorShapeSegments ) );

        fx = playerRadius * x;
        fz = playerRadius * z;

        x = m_maxRadius * x;
        z = m_maxRadius * z;

        bodySensorPoints[i] = localOri * Ogre::Vector3(x, h0, z);
        bodySensorPoints[i + m_sensorShapeSegments] = localOri * Ogre::Vector3(x, h1, z);

        stairSensorPoints[i] = localOri * Ogre::Vector3(fx, h0, fz);
        stairSensorPoints[i + m_sensorShapeSegments] = localOri * Ogre::Vector3(fx, h1, fz);

        bodyFloorSensorPoints[i] = localOri * Ogre::Vector3(x, h0, z);
        bodyFloorSensorPoints[i + m_sensorShapeSegments] = localOri * Ogre::Vector3(x, startHeight, z);

    }


    m_bodySensorShape = ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(m_body->getWorld(), &bodySensorPoints[0], 2*m_sensorShapeSegments, 0));
    m_stairSensorShape = ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(m_body->getWorld(), &stairSensorPoints[0], 2*m_sensorShapeSegments, 0));
    m_bodyFloorSensorShape = ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(m_body->getWorld(), &bodyFloorSensorPoints[0], 2*m_sensorShapeSegments, 0));

}


PlayerController::~PlayerController()
{
#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
    if( m_bodySensorShape )
        delete m_bodySensorShape;
    if( m_stairSensorShape )
        delete m_stairSensorShape;
    if( m_bodyFloorSensorShape )
        delete m_bodyFloorSensorShape;
#endif
}



bool PlayerController::convexStaticCastPreFilter(OgreNewt::Body *body, BodyVector filterBodies)
{
    Ogre::Real mass;
    Ogre::Vector3 inertia;
    body->getMassMatrix(mass, inertia);

    //if( mass != 0.0f || body == m_body )
    if( body == m_body )
        return false;

    for( int i = 0; i < filterBodies.size(); i++ )
    {
        if( body == filterBodies[i] )
            return false;
    }

    return true;
}


bool PlayerController::convexAllBodyCastPreFilter(OgreNewt::Body *body, BodyVector filterBodies)
{
    if( body == m_body )
        return false;

    for( int i = 0; i < filterBodies.size(); i++ )
    {
        if( body == filterBodies[i] )
            return false;
    }

    return true;
}


void PlayerController::setVelocity(Ogre::Real forwardSpeed, Ogre::Real sideSpeed, Ogre::Radian heading)
{
    m_forwardSpeed = forwardSpeed;
    m_sideSpeed = sideSpeed;
    m_heading = heading;
    m_body->unFreeze();
}


void PlayerController::getVelocity(Ogre::Real& forwardSpeed, Ogre::Real& sideSpeed, Ogre::Radian& heading) const
{
    forwardSpeed = m_forwardSpeed;
    sideSpeed = m_sideSpeed;
    heading = m_heading;
}


void PlayerController::setStairHeight(Ogre::Real stairHeight)
{
    m_stairHeight = stairHeight;
    updateGeometry();
}


Ogre::Real PlayerController::getStairHeight() const
{
    return m_stairHeight;
}


void PlayerController::setKinematicCushion(Ogre::Real cushion)
{
    m_kinematicCushion = std::max(cushion, 1.0f/64.0f);
    updateGeometry();
}


Ogre::Real PlayerController::getKinematicCushion() const
{
    return m_kinematicCushion;
}


void PlayerController::setMaxSlope(Ogre::Radian maxSlope)
{
    m_maxSlope = maxSlope;
}


Ogre::Radian PlayerController::getMaxSlope() const
{
    return m_maxSlope;
}


void PlayerController::setRestitution(Ogre::Real restitution)
{
    m_restitution = restitution;
}


Ogre::Real PlayerController::getRestitution() const
{
    return m_restitution;
}


PlayerController::PlayerState PlayerController::getPlayerState() const
{
    return m_playerState;
}

void PlayerController::setPlayerState(PlayerController::PlayerState state)
{
    std::cout << "\n old player state: " << m_playerState << "    new player state: " << state << "\n";
    m_playerState = state;
}


// helper class
class HitBodyVector : public std::vector<Body*>
{
    public:
        void getCollidingBodiesFromConvexcast(const BasicConvexcast& cast)
        {
            // find first contact with each body and cache it
            resize(0);
            for(int i = 0; i < size(); i++)
            {
                Body* body = cast.getInfoAt(i).mBody;
                int j;
                for( j = 0; j < size(); j++ )
                {
                    if( body == at(j) )
                        break;
                }
                if( j == size() )
                    push_back(body);
            }
        }
};


void PlayerController::submitConstraint( Ogre::Real timestep, int threadindex )
{

    Ogre::Vector3 pos;
    Ogre::Quaternion ori;
    Ogre::Real invMass;
    Ogre::Vector3 invInertia;
    Ogre::Vector3 frontDir, upDir, strafeDir;

    m_body->getInvMass(invMass, invInertia);
    m_body->getPositionOrientation(pos,ori);

    Ogre::Quaternion localFrameRotation = m_localFrame.extractQuaternion();
    frontDir = localFrameRotation * Ogre::Vector3::NEGATIVE_UNIT_Z;
    upDir = localFrameRotation * Ogre::Vector3::UNIT_Y;
    strafeDir = localFrameRotation * Ogre::Vector3::UNIT_X;



    // save the gravity before the collision force is applied
    m_gravity = m_body->getForceAcceleration();
    m_gravity *= invMass;


    // if the body has rotated by some amount, there will be a plane of rotation
    Ogre::Vector3 realUp = ori*(localFrameRotation*Ogre::Vector3::UNIT_Y);
    Ogre::Vector3 lateralDir = realUp.crossProduct(upDir);
    Ogre::Real mag = lateralDir.length();
    if( mag > 1.0e-3f)
    {
        // if the side vector is not zero, it means the body has rotated
        lateralDir /= mag;
        Ogre::Radian angle = Ogre::Math::ASin(mag);
        // add an angular constraint to correct the error angle
        addAngularRow(angle, lateralDir);


        // in theory only one correction is needed, but this produces instability as the body may move sideway.
        // a lateral correction prevent this from happening.
        Ogre::Vector3 frontDir = lateralDir.crossProduct(upDir);
        addAngularRow(Ogre::Radian(0), lateralDir);
    }
    else
    {
        // if the angle error is very small then two angular correction along the plane axis do the trick
        addAngularRow( Ogre::Radian(0), strafeDir );
        addAngularRow( Ogre::Radian(0), frontDir );
    }


    
    // calculate/set velocity (more complicated velocity calculation is done in the "kinematic motion" functions (feedbackCollector, playerOnLand, playerOnFreeFall, playerOnIllegalSlope
    if( m_playerState == PS_ONLAND )
    {
        Ogre::Vector3 vel = m_body->getVelocity();
        Ogre::Vector3 desiredVel = frontDir* m_forwardSpeed + upDir * (vel.dotProduct(upDir)) + strafeDir * m_sideSpeed;
        
        m_body->setVelocity(desiredVel);
    }
}


void PlayerController::feedbackCollector( Ogre::Real timestep, int threadIndex )
{
    Ogre::Vector3 pos, frontDir, upDir, targetFrontDir, targetOmega;
    Ogre::Quaternion ori, localFrameRotation;
    Ogre::Radian turnAngle, turnOmega;

    localFrameRotation = m_localFrame.extractQuaternion();

    m_body->getPositionOrientation(pos, ori);
    frontDir = ori*(localFrameRotation*Ogre::Vector3::NEGATIVE_UNIT_Z);
    upDir = ori*(localFrameRotation*Ogre::Vector3::UNIT_Y);

    targetFrontDir = localFrameRotation*Ogre::Quaternion(m_heading, Ogre::Vector3::UNIT_Y)*Ogre::Vector3::NEGATIVE_UNIT_Z;
    turnAngle = Ogre::Radian(Ogre::Math::ASin( std::min( std::max( frontDir.crossProduct(targetFrontDir).dotProduct(upDir), -1.0f), 1.0f)));
    turnOmega = turnAngle / timestep;

    targetOmega = turnOmega.valueRadians() * (localFrameRotation*Ogre::Vector3::UNIT_Y);
    m_body->setOmega(targetOmega);




    switch( m_playerState )
    {
        case PS_ONFREEFALL:
            playerOnFreeFall(timestep, threadIndex);
            break;
        case PS_ONLAND:
            playerOnLand(timestep, threadIndex );
            break;
        case PS_ONILLEGALRAMP:
            playerOnIllegalRamp(timestep, threadIndex);
            break;
    }

}


Ogre::Vector3 PlayerController::calculateVelocity(const Ogre::Vector3& targetVel_, Ogre::Real timestep, const Ogre::Vector3& upDir, Ogre::Real elevation, int threadindex)
{
    int contactCount;
    Ogre::Real hitParam, timestepInv(1.0f/timestep);
    Ogre::Vector3 pos, targetVel(targetVel_), destination;
    Ogre::Quaternion ori;
    StaticConvexCast staticConvexCast(this);

    m_body->getPositionOrientation(pos, ori);
    pos += upDir*elevation;
    destination = pos + timestep * targetVel;


    // see if it hit an obstacle
    staticConvexCast.go(m_bodySensorShape, pos, ori, destination, m_maxContactsCount, threadindex);
    contactCount = staticConvexCast.getContactsCount();
    for( int iterations = 0; iterations < m_maxCollisionsIteration && contactCount; iterations++ )
    {
        int flag;
        Ogre::Real time;
        flag = 0;

        // if the player will hit an obstacle in this direction then we need to change the velocity direction
        // we will declare a collision at the player origin.
        Ogre::Vector3 step;
        step = destination - pos;
        Ogre::Vector3 obstaclePos;
        obstaclePos = pos + step*staticConvexCast.getDistanceToFirstHit();

        // calculate the travel time, and subtract from time remaining
        time = staticConvexCast.getDistanceToFirstHit() * step.dotProduct(targetVel) / targetVel.squaredLength();
        for(int i = 0; i < contactCount; i++ )
        {
            Ogre::Real reboundVel;
            Ogre::Real penetrationVel;
            BasicConvexcast::ConvexcastContactInfo info;
            info = staticConvexCast.getInfoAt(i);

            // flatten contact normal
            info.mContactNormal -= upDir * upDir.dotProduct(info.mContactNormal);
            info.mContactNormal.normalise();

            // calculate the reflection velocity
            penetrationVel = -0.5f*timestepInv*std::min( info.mContactPenetration, 0.1f );
            reboundVel = targetVel.dotProduct(info.mContactNormal) * (1.0f + m_restitution) + penetrationVel;
            if( reboundVel < 0.0f )
            {
                flag = 1;
                targetVel -= reboundVel * info.mContactNormal;
            }
        }

        contactCount = 0;
        if( (time > 1.0e-2f * timestep) && flag )
        {
            destination = pos + targetVel * timestep;
            staticConvexCast.go(m_bodySensorShape, pos, ori, destination, m_maxContactsCount, threadindex);
            contactCount = staticConvexCast.getContactsCount();
        }

    }

    return targetVel;
}


void PlayerController::playerOnFreeFall( Ogre::Real timestep, int threadIndex )
{
    Ogre::Vector3 pos, castTarget, castStart;
    Ogre::Quaternion ori, localFrameRotation;
    Ogre::Vector3 upDir, frontDir;
    Ogre::Vector3 targetVel;
    Ogre::Real dist;
    StaticConvexCast staticConvexCast(this);

    
    m_body->getPositionOrientation(pos, ori);
    upDir = ori*(localFrameRotation*Ogre::Vector3::UNIT_Y);
    frontDir = ori*(localFrameRotation*Ogre::Vector3::NEGATIVE_UNIT_Z);

    targetVel = m_body->getVelocity();
    // make sure the velocity will not penetrate other bodies:
    targetVel = calculateVelocity(targetVel, timestep, upDir, m_stairHeight, threadIndex);

    // player of in free fall look ahead for land
    dist = upDir.dotProduct(targetVel*timestep);

    castStart = pos - upDir*m_kinematicCushion;
    castTarget = castStart + upDir*dist;
    staticConvexCast.go(m_bodyFloorSensorShape, castStart, ori, castTarget, 1, threadIndex);
    if( staticConvexCast.getContactsCount() > 0 )
    {
std::cout << "\n found contact.\n";
        // player is about to land, snap position to the ground
        Ogre::Vector3 newPos = pos + upDir * staticConvexCast.getDistanceToFirstHit() * dist;
        m_body->setPositionOrientation(newPos, ori);

        Ogre::Vector3 contactNormal = staticConvexCast.getInfoAt(0).mContactNormal;
        if( upDir.angleBetween( contactNormal ) > m_maxSlope )
        {
            setPlayerState(PS_ONILLEGALRAMP);
        }
        else
        {
            setPlayerState(PS_ONLAND);
            targetVel -= upDir * upDir.dotProduct(targetVel);
        }
    }

    m_body->setVelocity(targetVel);
}



void PlayerController::playerOnIllegalRamp( Ogre::Real timestep, int threadIndex )
{
    Ogre::Vector3 pos, realPos, castStart, castTarget;
    Ogre::Quaternion ori, localFrameRotation;
    Ogre::Vector3 upDir, frontDir;
    Ogre::Vector3 targetVel, step;
    StaticConvexCast staticConvexCast(this);
    AllBodyConvexCast allBodyConvexCast(this);

    localFrameRotation = m_localFrame.extractQuaternion();
    
    m_body->getPositionOrientation(pos, ori);
    upDir = ori*(localFrameRotation*Ogre::Vector3::UNIT_Y);
    frontDir = ori*(localFrameRotation*Ogre::Vector3::NEGATIVE_UNIT_Z);


    targetVel = m_body->getVelocity();

    //apply free fall gravity force to the body along the ramp
    castStart = pos + upDir*m_stairHeight;
    castTarget = pos - upDir*m_stairHeight;
    staticConvexCast.go(m_bodyFloorSensorShape, castStart, ori, castTarget, m_maxContactsCount, threadIndex);
    if( staticConvexCast.getContactsCount() > 0 )
    {
        targetVel -= m_gravity*timestep;
        Ogre::Vector3 floorNormal = staticConvexCast.getInfoAt(0).mContactNormal;
        Ogre::Vector3 gravityForce = m_gravity - floorNormal*(floorNormal.dotProduct(m_gravity));
        targetVel -= floorNormal * floorNormal.dotProduct(m_gravity) + gravityForce * timestep;
    }

    // make sure the body velocity will not penetrate other bodies
    targetVel = calculateVelocity(targetVel, timestep, upDir, m_stairHeight, threadIndex);


    step = targetVel * timestep;
    castStart = pos + step + upDir*(m_stairHeight - m_kinematicCushion);
    castTarget = castStart - upDir * 2.0f * m_stairHeight;
    allBodyConvexCast.go(m_bodyFloorSensorShape, castStart, ori, castTarget, 1, threadIndex);
    if( allBodyConvexCast.getContactsCount() == 0 )
    {
        setPlayerState(PS_ONFREEFALL);
    }
    else
    {
        Ogre::Real dist = allBodyConvexCast.getDistanceToFirstHit();
        if( dist <= 1.0e-3f )
            ;
        else
        {
            bool isFuturePosInRamp;
            Ogre::Vector3 newPos;
            
            newPos = castStart + (castTarget - castStart)*dist - step + upDir * m_kinematicCushion;
            m_body->setPositionOrientation(pos, ori);

            Ogre::Vector3 floorNormal = allBodyConvexCast.getInfoAt(0).mContactNormal;

            isFuturePosInRamp = upDir.angleBetween(floorNormal) > m_maxSlope;
            if( !isFuturePosInRamp )
            {
                setPlayerState(PS_ONLAND);
            }
        }
    }

    m_body->setVelocity(targetVel);
}


void PlayerController::playerOnLand( Ogre::Real timestep, int threadIndex )
{
    Ogre::Vector3 pos, realPos, castStart, castTarget;
    Ogre::Quaternion ori, localFrameRotation;
    Ogre::Vector3 upDir, frontDir;
    Ogre::Vector3 targetVel, step;
    Ogre::Real distanceToFirstHit;
    AllBodyConvexCast allBodyConvexCast(this);

    localFrameRotation = m_localFrame.extractQuaternion();
    
    m_body->getPositionOrientation(pos, ori);
    upDir = ori*(localFrameRotation*Ogre::Vector3::UNIT_Y);
    frontDir = ori*(localFrameRotation*Ogre::Vector3::NEGATIVE_UNIT_Z);


    targetVel = m_body->getVelocity();


    // subtract gravity contribution
    targetVel -= m_gravity*timestep;
    // make sure the body velocity will not penetrate other bodies
    targetVel = calculateVelocity(targetVel, timestep, upDir, m_stairHeight, threadIndex);

    step = targetVel * timestep;

    castStart = pos + step + upDir * (m_stairHeight - m_kinematicCushion);
    castTarget = castStart - upDir * 2.0f * m_stairHeight;
    allBodyConvexCast.go(m_bodyFloorSensorShape, castStart, ori, castTarget, 1, threadIndex);
    distanceToFirstHit = allBodyConvexCast.getDistanceToFirstHit();
    if( allBodyConvexCast.getContactsCount() == 0 )
    {
        setPlayerState(PS_ONFREEFALL);
    }
    else
    {
        if( distanceToFirstHit <= 1.0e-3f )
            ;
        else
        {
            bool isFuturePosInRamp;

            Ogre::Vector3 floorNormal = allBodyConvexCast.getInfoAt(0).mContactNormal;

            isFuturePosInRamp = upDir.angleBetween(floorNormal) > m_maxSlope;
            if( !isFuturePosInRamp )
            {
                AllBodyConvexCast allBodyConvexCast2(this);
                Ogre::Vector3 castStart2, castTarget2;
                
                castStart2 = pos + upDir*(m_stairHeight - m_kinematicCushion);
                castTarget2 = castStart2 - upDir*2.0f*m_stairHeight;
                BodyVector filterBodies;
                filterBodies.push_back(allBodyConvexCast.getInfoAt(0).mBody);
                
                allBodyConvexCast2.go(m_bodyFloorSensorShape, castStart2, ori, castTarget2, 1, threadIndex, filterBodies);

                if( distanceToFirstHit < allBodyConvexCast2.getDistanceToFirstHit() && allBodyConvexCast2.getContactsCount() > 0 )
                {
                    setPlayerState(PS_ONILLEGALRAMP);
                }
                else
                {
                    // the player hit the edge of a forbidden ramp

                    int iterations, contactCount = 1;
                    Ogre::Vector3 savedTargetVel;
                    savedTargetVel = targetVel;
                    for( iterations = 0; iterations < m_maxCollisionsIteration && contactCount; iterations++ )
                    {
                        Ogre::Real projectVel;
                        Ogre::Vector3 castStart3, castTarget3;
                        AllBodyConvexCast allBodyConvexCast3(this);

                        floorNormal -= upDir*floorNormal.dotProduct(upDir);
                        floorNormal.normalise();

                        projectVel = targetVel.dotProduct(floorNormal);
                        targetVel -= floorNormal * projectVel;

                        step = targetVel * timestep;
                        castStart3 = pos + step + upDir*(m_stairHeight - m_kinematicCushion);
                        castTarget3 = castStart3 - upDir*2.0f * m_stairHeight;

                        allBodyConvexCast3.go(m_bodyFloorSensorShape, castStart3, ori, castTarget3, 1, threadIndex, filterBodies);
                        contactCount = allBodyConvexCast3.getContactsCount();
                        if( contactCount > 0 )
                        {
                            distanceToFirstHit = allBodyConvexCast3.getDistanceToFirstHit();
                            castStart = castStart3;
                            castTarget = castTarget3;

                            floorNormal = allBodyConvexCast3.getInfoAt(0).mContactNormal;
                            filterBodies.push_back(allBodyConvexCast3.getInfoAt(0).mBody);
                            
                            if( upDir.angleBetween( floorNormal ) < m_maxSlope )
                                contactCount = 1;
                            else
                                contactCount = 0;
                        }
                    }


                    if( iterations >= m_maxCollisionsIteration )
                    {
                        Ogre::Vector3 targetVel1 = calculateVelocity(savedTargetVel, timestep, upDir, 0.0f, threadIndex);
                        Ogre::Vector3 err = targetVel1 - targetVel;
                        if( err.squaredLength() < 1.0e-6f )
                        {
                            setPlayerState(PS_ONILLEGALRAMP);
                        }
                        else
                        {
                            targetVel = targetVel1;
                            distanceToFirstHit = 0.0f;
                            step = Ogre::Vector3::ZERO;
                            pos -= upDir*m_kinematicCushion;
                        }
                    }
                    
                }
            }

            Ogre::Vector3 newPos = pos + (castTarget - castStart)*distanceToFirstHit + upDir*m_kinematicCushion;
            newPos -= step;
            m_body->setPositionOrientation(newPos, ori);

        }
    }


    m_body->setVelocity(targetVel);
}

}   // end NAMESPACE OgreNewt

