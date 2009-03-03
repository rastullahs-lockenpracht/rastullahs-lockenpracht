#include "OgreNewt_PlayerController.h"
#include "OgreNewt_Tools.h"
#include "OgreNewt_Collision.h"
#include "OgreNewt_CollisionPrimitives.h"
#include "OgreNewt_World.h"
#include <vector>
#include <iostream>

namespace OgreNewt
{

PlayerController::PlayerController(OgreNewt::Body * child) :
    CustomJoint(6, child, NULL),
    m_body(child)
{
    // initialize some non settable parameters
    m_maxCollisionsIteration = 8;
    m_maxContactsCount = 16;
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
    Ogre::Real rz = aab.getMaximum().z - aab.getMinimum().z;
    radius = std::max(rx,rz) / 2.0f;

    //! TODO: the aabb seems to be too big
// height *= 0.9f;
// radius *= 0.5f;
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

/*
// helper class
class HitBodyCache : public BasicConvexcast::ConvexcastContactInfo
{
    public:
        Ogre::Vector3 mVel;
        Ogre::Vector3 mOmega;
        HitBodyCache() {}
        HitBodyCache(const BasicConvexcast::ConvexcastContactInfo& info) :
            BasicConvexcast::ConvexcastContactInfo(info)
        {
            mVel = mBody->getVelocity();
            mOmega = mBody->getOmega();
        }
};

// helper class
class HitBodyVector : public std::vector<HitBodyCache>
{
    public:
        void getCollidingBodiesFromConvexcast(const BasicConvexcast& cast)
        {
            // find first contact with each body and cache it
            resize(0);
            for(int i = 0; i < size(); i++)
            {
                int j;
                for( j = 0; j < size(); j++ )
                {
                    if( cast.getInfoAt(i).mBody == at(j).mBody )
                        break;
                }
                if( j == size() )
                    push_back(HitBodyCache(cast.getInfoAt(0)));
            }
        }
};
*/

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






            // ----------------- DYNAMIC CONVEXCAST ---------------------

            // first cast directly in front of the player
            Ogre::Vector3 startCast = pos;
            Ogre::Vector3 endCast = startCast;
            DynamicConvexCast dynamicConvexCast(this);
            HitBodyVector hitBodyVec;
            hitBodyVec.reserve(m_maxContactsCount);
/*
            endCast = startCast + horizontalDesiredVel*timestep;
            dynamicConvexCast.go(m_dynamicsSensorShape, startCast, yawOri, endCast, m_maxContactsCount, threadindex);

            for(int iterations = 0; iterations < m_maxCollisionsIteration; iterations++)
            {
                bool velocCorrection;
                Ogre::Real timeToFirstContact;
                int numOfContacts;


                numOfContacts = dynamicConvexCast.getContactsCount();
                if( numOfContacts == 0 )
                    break;

                timeToFirstContact = (dynamicConvexCast.getInfoAt(0).mContactPoint - startCast).dotProduct(horizontalDesiredVel) / 
                                     horizontalDesiredVel.squaredLength();
                velocCorrection = false;


                // correct velocity if the body cannot be pushed and calculate impulse if it can be pushed
                for(int i = 0; i < numOfContacts; i++)
                {
                    Ogre::Real hitMass;
                    Ogre::Vector3 hitInertia;
                    Ogre::Vector3 hitNormal;
                    Body* hitBody = dynamicConvexCast.getInfoAt(i).mBody;
                    hitBody->getMassMatrix(hitMass, hitInertia);
                    hitNormal = dynamicConvexCast.getInfoAt(i).mContactNormal;
                    hitNormal.y = 0;
                    hitNormal.normalise();

                    if( !canPushBody(hitBody) )
                    {
                        Ogre::Real reboundVel, penetrationVel;
                        Ogre::Real penetration;
                        penetration = dynamicConvexCast.getInfoAt(i).mContactPenetration;
                        penetration = std::max(penetration, 0.1f);
                        penetrationVel = -0.5f/timestep * penetration;

                        reboundVel = horizontalDesiredVel.dotProduct(hitNormal) * (1.0f+m_restitution) + penetrationVel;
                        if( reboundVel < 0.0f )
                        {
                            velocCorrection = true;
                            horizontalDesiredVel -= hitNormal*reboundVel;
                        }
                    }
                    else // if( !canPushBody(body) )  -> can push body
                    {
                        Ogre::Real relVel, projVel, massWeigh, momentumDamper, playerNormalVel;
                        Ogre::Vector3 hitCenterOfMass, hitPos, hitContactPointVel;
                        Ogre::Quaternion hitOri;

                        hitBody->getPositionOrientation(hitPos, hitOri);
                        hitCenterOfMass = hitBody->getCenterOfMass();
                        // calculate hitContactPointVel
                        hitCenterOfMass = hitOri*hitCenterOfMass;
                        hitContactPointVel = hitBody->getOmega() * (dynamicConvexCast.getInfoAt(i).mContactPoint - hitCenterOfMass);
                        hitContactPointVel.y = 0.0f; //! WHY??
                        hitContactPointVel += hitBody->getVelocity();

                        massWeigh = mass / (mass + hitMass);
                        massWeigh = std::min(massWeigh, 0.5f);

                        projVel = hitContactPointVel.dotProduct(hitNormal);
                        playerNormalVel = horizontalDesiredVel.dotProduct(hitNormal);
                        relVel = playerNormalVel * massWeigh - projVel;
                        if( relVel < 0.0f )
                        {
                            momentumDamper = 0.1f;
                            velocCorrection = true;
                            // correct horizontalDesiredVel
                            horizontalDesiredVel -= hitNormal*( relVel * (1.0f - momentumDamper) + playerNormalVel*(1.0f - massWeigh) );

                            //apply impulse to hit body
                            hitBody->addImpulse(hitNormal*relVel*momentumDamper, hitCenterOfMass); // not the real CenterOffMass any more!
                        }
                    }
                }



                // now restore hit body state and apply a force to archive the hit impulse
                hitBodyVec.getCollidingBodiesFromConvexcast(dynamicConvexCast);
                for(int i = 0; i < hitBodyVec.size(); i++)
                {
                    Ogre::Real hitMass;
                    Ogre::Vector3 hitInertia;
                    Body* hitBody = hitBodyVec[i];
                    hitBody->getMassMatrix(hitMass, hitInertia);

                    if( canPushBody(hitBody) && mass > 1.0e-3f )
                    {
                        Ogre::Vector3 hitPos, force, torque;
                        Ogre::Quaternion hitOri;

                        hitBody->getPositionOrientation(hitPos, hitOri);
                        
                        // calculate the force and the torque to archive the desired push
                        force = (vel - hitBody->getVelocity())*mass/timestep - hitBody->getForceAcceleration();
                        hitBody->addForce(force);
                        
                        torque = (omega - hitBody->getOmega())/timestep * (hitOri*hitInertia) - hitBody->getTorqueAcceleration();
                        hitBody->addTorque(torque);
                    }
                }


                // has the horizontalDesiredVel changed, so we need a new cast?
                if( timeToFirstContact > 0.01*timestep && velocCorrection )
                {
                    endCast = startCast + horizontalDesiredVel*timestep;
                    dynamicConvexCast.go(m_dynamicsSensorShape, startCast, yawOri, endCast, m_maxContactsCount, threadindex);
                }
            }
*/

            // ----------------- STATIC CONVEXCAST ---------------------
            StaticConvexCast staticConvexCast(this);
            startCast.y += 0.5f*m_maxStepHeight;
            endCast = startCast + horizontalDesiredVel*timestep;
/*
            staticConvexCast.go(m_horizontalSensorShape, startCast, yawOri, endCast, m_maxContactsCount, threadindex);
            for(int iterations = 0; iterations < m_maxCollisionsIteration; iterations++)
            {
                bool velocCorrection;
                Ogre::Real timeToFirstContact;
                int numOfContacts;


                numOfContacts = staticConvexCast.getContactsCount();
                if( numOfContacts == 0 )
                    break;

                timeToFirstContact = (staticConvexCast.getInfoAt(0).mContactPoint - startCast).dotProduct(horizontalDesiredVel) / 
                                     horizontalDesiredVel.squaredLength();
                velocCorrection = false;


                // correct velocity (like body that can't be pushed above!)
                for(int i = 0; i < numOfContacts; i++)
                {
                    Ogre::Real hitMass;
                    Ogre::Vector3 hitInertia;
                    Ogre::Vector3 hitNormal;
                    Ogre::Real reboundVel, penetrationVel;
                    Ogre::Real penetration;

                    Body* hitBody = staticConvexCast.getInfoAt(i).mBody;
                    hitBody->getMassMatrix(hitMass, hitInertia);
                    hitNormal = staticConvexCast.getInfoAt(i).mContactNormal;
                    hitNormal.y = 0;
                    hitNormal.normalise();

                    penetration = staticConvexCast.getInfoAt(i).mContactPenetration;
                    penetration = std::max(penetration, 0.1f);
                    penetrationVel = -0.5f/timestep * penetration;

                    reboundVel = horizontalDesiredVel.dotProduct(hitNormal) * (1.0f+m_restitution) + penetrationVel;
                    if( reboundVel < 0.0f )
                    {
                        velocCorrection = true;
                        horizontalDesiredVel -= hitNormal*reboundVel;
                    }
                }


                // has the horizontalDesiredVel changed, so we need a new cast?
                if( timeToFirstContact > 0.01*timestep && velocCorrection )
                {
                    endCast = startCast + horizontalDesiredVel*timestep;
                    staticConvexCast.go(m_horizontalSensorShape, startCast, yawOri, endCast, m_maxContactsCount, threadindex);
                }
            }
*/


            // ----------------- STEP RECOGNITION AND GROUND TRACKING ---------------------
            // now determine the ground tracking, predict the destination position
            if( m_isInJumpState )
            {
                // player of in the air look ahead for the land
                startCast = pos + horizontalDesiredVel*timestep + Ogre::Vector3::UNIT_Y*m_maxStepHeight*0.5f;
                endCast = startCast - Ogre::Vector3::UNIT_Y*m_maxStepHeight;

                staticConvexCast.go(m_verticalSensorShape, startCast, yawOri, endCast, 1, threadindex);
                
//! TODO: remove this... bug in newton?    
Ogre::Real distanceToFirstHit = staticConvexCast.getDistanceToFirstHit();
if( distanceToFirstHit > 0.01f )
{

                // found land
                if( staticConvexCast.getContactsCount() > 0 )
                {
                    Ogre::Real dist, correctionVel;
                    dist = - m_maxStepHeight * (0.5f - 1.0f*staticConvexCast.getDistanceToFirstHit());
                    correctionVel = dist/timestep - vel.y;
                    addLinearRow(pos, pos, Ogre::Vector3::UNIT_Y);
                    setRowAcceleration(correctionVel/timestep);
                    m_isInJumpState = false;
                }
}
            }
            else // m_isInJumpState
            {
                // player is moving on the ground, look ahead for stair steps
                startCast = pos + horizontalDesiredVel*timestep + Ogre::Vector3::UNIT_Y*m_maxStepHeight;
                endCast = startCast - Ogre::Vector3::UNIT_Y*m_maxStepHeight*2.0f;

                AllBodyConvexCast allBodyConvexCast(this);
                allBodyConvexCast.go(m_verticalSensorShape, startCast, yawOri, endCast, 1, threadindex);

                if( allBodyConvexCast.getContactsCount() == 0 )
                {
                    //m_isInJumpState = true;
                }
                else
                {
                    m_isInJumpState = false;


                    Ogre::Real distanceToFirstHit = allBodyConvexCast.getDistanceToFirstHit();
                    // found "step"

                    if( distanceToFirstHit < 0.01f )
                    {
                        // something when wrong because the vertical sensor shape is colliding
                        // at origin of the predicted destination
                        // this should never happens as a precaution set the vertical body velocity to zero
                        Ogre::Vector3 vel = m_body->getVelocity();
                        vel.y = 0.0f;
                        m_body->setVelocity(vel);
                    }
                    else
                    {
                        Ogre::Real dist, correctionVel;
                        // "the player will hit surface, make shure it is grounded" (?)
                        dist = - m_maxStepHeight * (1.0f - 3.0f*distanceToFirstHit);
/*
                        // check if the contact violates the maxSlope constraint
                        if( Ogre::Math::ASin( allBodyConvexCast.getInfoAt(0).mContactNormal.y ) < m_maxSlope )
                        {
                            // we are hitting stiff slope
                            // cast a ray at the hit point to verify this is a face normal and not and edge of vertex normal
                            // that can be pointing in the wrong direction
                            Ogre::Vector3 hitPos, localNormal, origin, retCollisionLocalNormal, p0, p1;
                            Ogre::Real retCollisionDist;
                            int retCollisionColId;
                            Ogre::Quaternion hitOri, hitOriInv;
                            const OgreNewt::Collision* collision;
                            
                            Body* hitBody = allBodyConvexCast.getInfoAt(0).mBody;
                            hitBody->getPositionOrientation(hitPos, hitOri);
                            
                            hitOriInv = hitOri.Inverse();
                            localNormal = hitOriInv*allBodyConvexCast.getInfoAt(0).mContactNormal;
                            origin = hitOriInv*(allBodyConvexCast.getInfoAt(0).mContactPoint - hitPos);
                            
                            collision = hitBody->getCollision();

                            p0 = origin + localNormal*0.1f;
                            p1 = origin - localNormal*0.1f;

                            retCollisionDist = CollisionTools::CollisionRayCast(collision, p0, p1, retCollisionLocalNormal, retCollisionColId);
                            if( retCollisionDist < 1.0f ) // this is always true
                            {
                                if( retCollisionLocalNormal.dotProduct(localNormal) < 0.9f ) // this is really an illegal slope
                                {
                                    horizontalDesiredVel = Ogre::Vector3::ZERO;
                                    dist= 0.0f; // this sets correctionVel for steps to 0
                                }
                            }
                            else
                            {
                                dist= 0.0f; // this sets correctionVel for steps to 0
                            }
                        }
*/

                        // move up for step

                        correctionVel = dist / timestep - vel.y;
std::cout << "distToFirstHit: " << distanceToFirstHit << "    \tdist: " << dist << "    \tvel.y: " << vel.y << "    \tCorrectionVel: " << correctionVel << std::endl;
                        addLinearRow(pos, pos, Ogre::Vector3::UNIT_Y);
                        setRowAcceleration( correctionVel / timestep );
                    }
                }
            }


            // calculate force needed for desired velocity
            horizontalDesiredVel.y = 0.0f;
            Ogre::Vector3 force = m_body->calculateInverseDynamicsForce(timestep, horizontalDesiredVel);
            Ogre::Vector3 forceAcc = m_body->getForceAcceleration();
            force -= forceAcc;
            force.y = 0;
            m_body->addForce(force);

        }
    }
    

}


}	// end NAMESPACE OgreNewt

