#include "OgreNewt_PlayerController.h"
#include "OgreNewt_Tools.h"
#include "OgreNewt_Collision.h"
#include "OgreNewt_CollisionPrimitives.h"
#include "OgreNewt_World.h"

namespace OgreNewt
{

PlayerController::PlayerController(OgreNewt::Body * child) :
    CustomJoint(6, child, NULL),
    m_body(child)
{
    // initialize settable values
    m_forwardSpeed = 0;
    m_sideSpeed = 0;
    m_heading = Ogre::Radian(0);
    Ogre::Real playerHeight, playerRadius;
    getPlayerHeightAndRadius(playerHeight, playerRadius);
    m_maxStepHeight = playerHeight / 4;
    m_maxSlope = Ogre::Degree(45);
    m_restitution = 0;
    m_upVector = Ogre::Vector3::UNIT_Y;
    m_isInJumpState = false;

    Ogre::Vector3 pos;
    m_body->getPositionOrientation(pos, m_lastOri);



    // initialize sensor-shape parameters
    m_staticRadiusFactor = 1.125f;
    m_dynamicRadiusFactor = 1.5f;
    m_floorFinderRadiusFactor = 1.0f;
    m_maxPlayerHeightPaddFactor = 0.01f;
    m_sensorShapeSegments = 32;
    m_verticalSensorShape = NULL;
    m_horizontalSensorShape = NULL;
    m_dynamicsSensorShape = NULL;


    updateSensorShapes();
}

PlayerController::~PlayerController()
{
    if( m_verticalSensorShape )
        delete m_verticalSensorShape;
    if( m_horizontalSensorShape )
        delete m_horizontalSensorShape;
    if( m_dynamicsSensorShape )
        delete m_dynamicsSensorShape
;
}

void PlayerController::updateSensorShapes()
{
    Ogre::Real newPlayerRadius, newPlayerHeight, newSensorHeight;
    // is this the first time, this function gets calles?
    if( m_verticalSensorShape == NULL )
    {
        // initialize values, so they aren't arbitrary!
        m_lastPlayerRadius = newPlayerRadius = 0;
        m_lastPlayerHeight = newPlayerHeight = 0;
        m_lastSensorHeight = newSensorHeight = 0;
    }

    // calculate new Values:
    getPlayerHeightAndRadius(newPlayerHeight, newPlayerRadius);
    newPlayerHeight += newPlayerHeight * m_maxPlayerHeightPaddFactor;
    newSensorHeight = (newPlayerHeight - m_maxStepHeight) * 0.5f;


    // do we need to update the shapes?
    if( m_verticalSensorShape == NULL || 
        abs( newPlayerHeight - m_lastPlayerHeight ) > 0.04f ||
        abs( newSensorHeight - m_lastSensorHeight ) > 0.04f ||
        abs( newPlayerRadius - m_lastPlayerRadius ) > 0.04f )
    {
        // delete old ones
        if( m_verticalSensorShape )
            delete m_verticalSensorShape;
        if( m_horizontalSensorShape )
            delete m_horizontalSensorShape;
        if( m_dynamicsSensorShape )
            delete m_dynamicsSensorShape;

        m_lastPlayerRadius = newPlayerRadius;
        m_lastSensorHeight = newSensorHeight;
        m_lastPlayerHeight = newPlayerHeight;

        
        Ogre::Vector3 *dynamicsSensorPoints = new Ogre::Vector3[m_sensorShapeSegments*2];
        Ogre::Vector3 *verticalSensorPoints = new Ogre::Vector3[m_sensorShapeSegments*2];
        Ogre::Vector3 *horizontalSensorPoints = new Ogre::Vector3[m_sensorShapeSegments*2];

        for( int i = 0; i < m_sensorShapeSegments; i++)
        {
            Ogre::Real x, z;
            x = m_lastPlayerRadius * Ogre::Math::Cos( Ogre::Degree( 360.0f * i / m_sensorShapeSegments ) );
            z = m_lastPlayerRadius * Ogre::Math::Sin( Ogre::Degree( 360.0f * i / m_sensorShapeSegments ) );

            dynamicsSensorPoints[i].x = x * m_dynamicRadiusFactor;
            dynamicsSensorPoints[i].y = m_lastPlayerHeight * 0.45f;
            dynamicsSensorPoints[i].z = z * m_dynamicRadiusFactor;
            dynamicsSensorPoints[i + m_sensorShapeSegments].x =  dynamicsSensorPoints[i].x;
            dynamicsSensorPoints[i + m_sensorShapeSegments].y = -dynamicsSensorPoints[i].y;
            dynamicsSensorPoints[i + m_sensorShapeSegments].z =  dynamicsSensorPoints[i].z;
            verticalSensorPoints[i].x = x * m_floorFinderRadiusFactor;
            verticalSensorPoints[i].y = m_lastSensorHeight;
            verticalSensorPoints[i].z = z * m_floorFinderRadiusFactor;
            verticalSensorPoints[i + m_sensorShapeSegments].x =  verticalSensorPoints[i].x;
            verticalSensorPoints[i + m_sensorShapeSegments].y = -verticalSensorPoints[i].y;
            verticalSensorPoints[i + m_sensorShapeSegments].z =  verticalSensorPoints[i].z;
            horizontalSensorPoints[i].x = x * m_staticRadiusFactor;
            horizontalSensorPoints[i].y = m_lastSensorHeight;
            horizontalSensorPoints[i].z = z * m_staticRadiusFactor;
            horizontalSensorPoints[i + m_sensorShapeSegments].x =  horizontalSensorPoints[i].x;
            horizontalSensorPoints[i + m_sensorShapeSegments].y = -horizontalSensorPoints[i].y;
            horizontalSensorPoints[i + m_sensorShapeSegments].z =  horizontalSensorPoints[i].z;
        }

        m_verticalSensorShape = new OgreNewt::CollisionPrimitives::ConvexHull(m_body->getWorld(), verticalSensorPoints, 2*m_sensorShapeSegments);
        m_horizontalSensorShape = new OgreNewt::CollisionPrimitives::ConvexHull(m_body->getWorld(), horizontalSensorPoints, 2*m_sensorShapeSegments);
        m_dynamicsSensorShape = new OgreNewt::CollisionPrimitives::ConvexHull(m_body->getWorld(), dynamicsSensorPoints, 2*m_sensorShapeSegments);

        delete[] dynamicsSensorPoints;
        delete[] verticalSensorPoints;
        delete[] horizontalSensorPoints;
    }

}


void PlayerController::getPlayerHeightAndRadius(Ogre::Real &height, Ogre::Real &radius)
{
    Ogre::AxisAlignedBox aab = m_body->getAABB();
    height = aab.getMaximum().y - aab.getMinimum().y;

    Ogre::Real rx = aab.getMaximum().x - aab.getMinimum().x;
    Ogre::Real ry = aab.getMaximum().y - aab.getMinimum().y;
    radius = std::max(rx,ry);
}


bool PlayerController::convexStaticCastPreFilter(OgreNewt::Body *body)
{
    Ogre::Real mass;
    Ogre::Vector3 inertia;
    body->getMassMatrix(mass, inertia);

    return (mass == 0.0f && body != m_body);
}


bool PlayerController::convexDynamicCastPreFilter(OgreNewt::Body *body)
{
    Ogre::Real mass;
    Ogre::Vector3 inertia;
    body->getMassMatrix(mass, inertia);

    return (mass > 0.0f && body != m_body);
}


bool PlayerController::convexAllBodyCastPreFilter(OgreNewt::Body *body)
{
    return (body != m_body);
}


bool PlayerController::findFloorCastPreFilter(OgreNewt::Body *body)
{
    return (body != m_body);
}


void PlayerController::setVelocity(Ogre::Real forwardSpeed, Ogre::Real sideSpeed, Ogre::Degree heading)
{
    m_forwardSpeed = forwardSpeed;
    m_sideSpeed = sideSpeed;
    m_heading = heading;
    m_body->unFreeze();
}


void PlayerController::submitConstraint( Ogre::Real timestep, int threadindex )
{

    Ogre::Vector3 pos;
    Ogre::Quaternion ori;
    Ogre::Real mass;
    Ogre::Vector3 inertia;
    Ogre::Vector3 vel;
    Ogre::Vector3 omega;
    Ogre::Vector3 torqueAcc;

    m_body->getPositionOrientation(pos,ori);
    m_body->getMassMatrix(mass, inertia);
    vel = m_body->getVelocity();
    omega = m_body->getOmega();
    torqueAcc = m_body->getTorqueAcceleration();


    // ----- "up vector joint" -----
    Ogre::Quaternion yawOri;
    yawOri.FromAngleAxis(ori.getYaw(), Ogre::Vector3::UNIT_Y);
    Ogre::Vector3 yawX = yawOri * Ogre::Vector3::UNIT_X;
    Ogre::Vector3 yawZ = yawOri * Ogre::Vector3::UNIT_Z;
    Ogre::Radian relAngleErrorX, relAngleErrorZ;
    Ogre::Vector3 dirErrorX, dirErrorZ;
    ori.xAxis().getRotationTo(yawX, yawZ).ToAngleAxis(relAngleErrorX, dirErrorX);
    ori.zAxis().getRotationTo(yawZ, yawX).ToAngleAxis(relAngleErrorZ, dirErrorZ);
    if( abs(relAngleErrorX.valueRadians()) < 0.001f )
        addAngularRow(Ogre::Radian(0.0f), yawZ);
    else
        addAngularRow(relAngleErrorX, dirErrorX);
    if( abs(relAngleErrorZ.valueRadians()) < 0.001f )
        addAngularRow(Ogre::Radian(0.0f), yawX);
    else
        addAngularRow(relAngleErrorZ, dirErrorZ);


    // ----------- calculate torque -----------
    Ogre::Quaternion targetOri;
    targetOri.FromAngleAxis(m_heading, Ogre::Vector3::UNIT_Y);
    Ogre::Radian yawDiff = ((ori*Ogre::Vector3::UNIT_Z).getRotationTo(targetOri*Ogre::Vector3::UNIT_Z)).getYaw();
    Ogre::Real turnOmega = yawDiff.valueRadians() / timestep;
    Ogre::Vector3 torque = Ogre::Vector3::UNIT_Y * ((turnOmega - omega.y) * inertia.y/timestep - torqueAcc.y);
    m_body->addTorque(torque);



    // ----------- find floor -----------
    //! perhaps calculate SLOPE differently, a convexcast with a rotated box should do it, but we'll see if it is needed!
    Ogre::Vector3 findFloorCastP1 = pos - Ogre::Vector3::UNIT_Y*m_maxStepHeight;
    // go a bit up from current position
    Ogre::Vector3 findFloorCastP0 = pos + Ogre::Vector3::UNIT_Y*m_maxStepHeight;
    FindFloorRaycast findFloorCast(this, findFloorCastP0, findFloorCastP1, true);
    if( findFloorCast.getHitCount() > 0 )
    {
        // slope:
        if( Ogre::Math::ASin(findFloorCast.getFirstHit().mNormal.y) < m_maxSlope )
        {
            // try some more raycasts...
            FindFloorRaycast findFloorCast_0(this, findFloorCastP0+0.1*Ogre::Vector3::UNIT_X, findFloorCastP1+0.1*Ogre::Vector3::UNIT_X, true);
            FindFloorRaycast findFloorCast_1(this, findFloorCastP0-0.1*Ogre::Vector3::UNIT_X, findFloorCastP1-0.1*Ogre::Vector3::UNIT_X, true);
            FindFloorRaycast findFloorCast_2(this, findFloorCastP0+0.1*Ogre::Vector3::UNIT_Z, findFloorCastP1+0.1*Ogre::Vector3::UNIT_Z, true);
            FindFloorRaycast findFloorCast_3(this, findFloorCastP0-0.1*Ogre::Vector3::UNIT_Z, findFloorCastP1-0.1*Ogre::Vector3::UNIT_Z, true);
            if( findFloorCast_0.getHitCount() > 0 && Ogre::Math::ASin(findFloorCast_0.getFirstHit().mNormal.y) < m_maxSlope )
            {
                findFloorCastP0 += 0.1*Ogre::Vector3::UNIT_X;
                findFloorCastP1 += 0.1*Ogre::Vector3::UNIT_X;
                findFloorCast = findFloorCast_0;
            }
            else if( findFloorCast_1.getHitCount() > 0 && Ogre::Math::ASin(findFloorCast_1.getFirstHit().mNormal.y) < m_maxSlope )
            {
                findFloorCastP0 -= 0.1*Ogre::Vector3::UNIT_X;
                findFloorCastP1 -= 0.1*Ogre::Vector3::UNIT_X;
                findFloorCast = findFloorCast_1;
            }
            else if( findFloorCast_2.getHitCount() > 0 && Ogre::Math::ASin(findFloorCast_2.getFirstHit().mNormal.y) < m_maxSlope )
            {
                findFloorCastP0 += 0.1*Ogre::Vector3::UNIT_Z;
                findFloorCastP1 += 0.1*Ogre::Vector3::UNIT_Z;
                findFloorCast = findFloorCast_2;
            }
            else if( findFloorCast_3.getHitCount() > 0 && Ogre::Math::ASin(findFloorCast_3.getFirstHit().mNormal.y) < m_maxSlope )
            {
                findFloorCastP0 -= 0.1*Ogre::Vector3::UNIT_Z;
                findFloorCastP1 -= 0.1*Ogre::Vector3::UNIT_Z;
                findFloorCast = findFloorCast_3;
            }
        }
        
        if( Ogre::Math::ASin(findFloorCast.getFirstHit().mNormal.y) > m_maxSlope )
        
        {
            // desired velocity
            Ogre::Vector3 desiredVel = ori*(Ogre::Vector3::UNIT_Z * m_forwardSpeed + Ogre::Vector3::UNIT_Y*vel.y + Ogre::Vector3::UNIT_X * m_sideSpeed);

            if( m_isInJumpState )
                desiredVel = vel;
            // ------------------------ here should be an else!! -------------------------
            // only apply hitVel if we are not in the air, in the original this is different!
            else
            {

                // hit-body:
                Ogre::Vector3 hitPoint = findFloorCastP0 + (findFloorCastP1-findFloorCastP0) * findFloorCast.getFirstHit().mDistance;
                Ogre::Vector3 hitOmega = findFloorCast.getFirstHit().mBody->getOmega();
                Ogre::Vector3 hitVel = findFloorCast.getFirstHit().mBody->getVelocity();
                Ogre::Vector3 hitPos;
                Ogre::Quaternion hitOri;
                findFloorCast.getFirstHit().mBody->getPositionOrientation(hitPos, hitOri);

                hitVel += hitOmega * (hitPoint - hitPos).length();
                desiredVel += hitVel;
            }



            // look ahead for obstacles in along the horizontal of the desired velocity
            Ogre::Vector3 horizontalDesiredVel = desiredVel;
            horizontalDesiredVel.y = 0;





            // calculate force needed for desired velocity
            Ogre::Vector3 force = m_body->calculateInverseDynamicsForce(timestep, horizontalDesiredVel);
            Ogre::Vector3 forceAcc = m_body->getForceAcceleration();
            force -= forceAcc;
            force.y = 0;
            m_body->addForce(force);

        }
    }
    

}


}	// end NAMESPACE OgreNewt

