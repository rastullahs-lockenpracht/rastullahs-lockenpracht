#include <OgreNewt_PlayerController.h>
#include <OgreNewt_Tools.h>
#include <OgreNewt_CollisionPrimitives.h>

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
    m_maxStepHeight = playerHeight / 5;
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

    // ----------- "up-vector-pin" -----------
    // get the plane of rotation
    Ogre::Vector3 oriChangeVec;
    Ogre::Radian oriChangeAngle;
    Ogre::Quaternion oriDiffFromLast = (ori * m_lastOri.Inverse());
    oriDiffFromLast.ToAngleAxis(oriChangeAngle, oriChangeVec);
    if( abs(oriChangeAngle.valueRadians()) > 1.0e-3 )
    {
        // correct angle
        addAngularRow(oriChangeAngle, oriChangeVec);

        // additional correction, see newton CustomPlayerController
        Ogre::Vector3 frontDir = ori*oriChangeVec;
        addAngularRow(Ogre::Radian(0.0f), oriChangeVec);
    }
    else
    {
        addAngularRow(Ogre::Radian(0.0f), ori.yAxis());
        addAngularRow(Ogre::Radian(0.0f), ori.xAxis());
    }

    // ----------- calculate torque -----------
    Ogre::Quaternion targetOri;
    targetOri.FromAngleAxis(m_heading, Ogre::Vector3::UNIT_Y);
    Ogre::Quaternion oriDiff = (targetOri * ori.Inverse());
    Ogre::Real turnOmega  = oriDiff.getYaw().valueRadians() / timestep;
    torqueAcc = ori.yAxis() * (torqueAcc.dotProduct(ori.yAxis()));
    Ogre::Vector3 torque = ori.yAxis() * (turnOmega - omega.y * inertia.y/timestep - torqueAcc.y);
    m_body->addTorque(torque);



    // ----------- find floor -----------
    Ogre::Vector3 p1 = pos - Ogre::Vector3::UNIT_Y*m_lastPlayerHeight;
    FindFloorRaycast findFloorCast(this, pos, p1, true);
    if( findFloorCast.getHitCount() > 0 )
    {
        // slope:
        if( Ogre::Math::ACos(findFloorCast.getFirstHit().mNormal.y) > m_maxSlope )
        
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
                Ogre::Vector3 hitPoint = pos + (p1-pos) * findFloorCast.getFirstHit().mDistance;
                Ogre::Vector3 hitOmega = findFloorCast.getFirstHit().mBody->getOmega();
                Ogre::Vector3 hitVel = findFloorCast.getFirstHit().mBody->getVelocity();
                Ogre::Vector3 hitPos;
                Ogre::Quaternion hitOri;
                findFloorCast.getFirstHit().mBody->getPositionOrientation(hitPos, hitOri);

                hitVel += hitOmega * (hitPoint - hitPos).length();
                desiredVel += hitVel;
            }





            // calculate force needed for desired velocity
//            m_body->

        }
    }
    

}


}	// end NAMESPACE OgreNewt

